#include "Sound.h"
#include "../_system/_Windows.h"
#include "../system/Event.h"
#include "../data/Memory.h"


static thread soundThread;

#ifdef _WIN32
static HANDLE hEvent;

static IMMDevice* soundDevice;
static IMMDeviceEnumerator* soundDeviceEnumerator;
static UINT32 soundBufferSize;

static IAudioClient* soundClient;
static IAudioRenderClient* soundRenderClient;
static IChannelAudioVolume* soundVolume;
static IAudioClock* soundClock;

static float masterVolume;
static bool mute = false;

#elif __ANDROID__

static SLObjectItf engineObject;
static SLEngineItf engine;

static SLObjectItf outputMixObject;
static SLVolumeItf volumeItf;

static SLDataSource audioSource;
static SLDataLocator_BufferQueue bufferQueue;
static SLDataSink audioSink;
static SLDataLocator_OutputMix locatorOutputMix;

static SLObjectItf playerObject;
static SLPlayItf playItf;
static SLBufferQueueItf bufferQueueItf;
static SLBufferQueueState bufferQueueState;

static SLDataFormat_PCM format;

static constexpr unsigned MAX_NUMBER_INTERFACES = 2;

static SLboolean required[MAX_NUMBER_INTERFACES];
static SLInterfaceID iidArray[MAX_NUMBER_INTERFACES];

static constexpr unsigned BUFFER_LEN = 2048;

static Event<true,false> soundEvent;

short buffer[BUFFER_LEN / 2];

SLmillibel maxLevel;

static int bufferCount = 0;

#endif


void Sound::SetMute(bool _mute) {
#ifdef _WIN32
	float volumes[2];
	mute = _mute;
	if (mute) {
		volumes[0] = 0.f;
		volumes[1] = 0.f;
	} else {
		volumes[0] = masterVolume;
		volumes[1] = masterVolume;
	}
	soundVolume->SetAllVolumes(2, volumes,nullptr);
#elif __ANDROID__
	(*volumeItf)->SetMute(volumeItf,(SLboolean)_mute);
#endif
}
void Sound::SetMasterVolume(float _volume) {
#ifdef _DEBUG
	if (_volume<0.f || _volume>1.f);
#endif
#ifdef _WIN32
	masterVolume = _volume;
	if (!mute) {
		float volumes[2];
		volumes[0] = masterVolume;
		volumes[1] = masterVolume;
		soundVolume->SetAllVolumes(2, volumes, nullptr);
	}
#elif __ANDROID__
	if (_volume == 0.f) {
		(*volumeItf)->SetVolumeLevel(volumeItf, SL_MILLIBEL_MIN);
	} else {
		(*volumeItf)->SetVolumeLevel(volumeItf, maxLevel - (SLmillibel)(log10f(1.f / _volume) * 10000));//1000mdb = 1db
	}
#endif
}

void Sound::Decode(SoundSource* _data) {
#ifdef _DEBUG
	if (playing);
	if (paused);
#endif
	soundMutex.lock();
	data = _data;
	pos = 0;
	playing = false;
	paused = false;
	soundMutex.unlock();
}
SoundSource* Sound::GetData()const {
#ifdef _DEBUG
	if (!data);
#endif
	return data; 
}
#ifdef _DEBUG
Sound::Sound() :volume(1.f), playing(false), paused(false), data(nullptr) { sounds.InsertLast(this); }
#else
Sound::Sound() : volume(1.f), playing(false) { sounds.InsertLast(this); }
#endif

void Sound::SetVolume(float _volume) { volume = _volume; }
float Sound::GetVolume()const { return volume; }
unsigned Sound::GetLoop()const {
#ifdef _DEBUG
	if (!playing&&!paused);
#endif
	return loop; 
}
unsigned Sound::GetLoopCount()const {
#ifdef _DEBUG
	if (!playing && !paused);
#endif
	return loopCount; 
}

unsigned Sound::GetPos() const {
#ifdef _DEBUG
	if (!data);
#endif
	return pos; 
}

Sound::~Sound() {
	soundMutex.lock();
	sounds.EraseIndex(sounds.Search(this));
	soundMutex.unlock();
}

void Sound::Play(unsigned _loop) {
#ifdef _DEBUG
	if (!data);
	if (playing);
	if (paused);
#endif
	soundMutex.lock();
	loop = _loop;
	loopCount = 0;
	playing = true;

	soundMutex.unlock();
}
void Sound::SetPos(unsigned _pos) {
#ifdef _DEBUG
	if (!data);
	if (_pos >= data->size);
#endif
	soundMutex.lock();
	pos = _pos;
	soundMutex.unlock();
}
void Sound::Stop() {
#ifdef _DEBUG
	if (!playing && !paused);
#endif
	soundMutex.lock();

	playing = false;
	paused = false;
	pos = 0;
	soundMutex.unlock();
}
void Sound::Pause() {
#ifdef _DEBUG
	if (paused);
	if (!playing && !paused);
#endif
	soundMutex.lock();
	playing = false;
	paused = true;
	soundMutex.unlock();
}
void Sound::Resume() {
#ifdef _DEBUG
	if (playing);
	if (!playing && !paused);
#endif
	soundMutex.lock();
	playing = true;
	paused = false;
	soundMutex.unlock();
}
#ifdef _WIN32
void Sound::ThreadFunc() {
	soundClient->Start();
	while (!soundExit) {
		unsigned soundRemain;

		unsigned padding;
		soundClient->GetCurrentPadding(&padding);
		const unsigned soundDataSize = soundBufferSize - padding;

		if (soundDataSize > 0) {

			const unsigned soundDataSizeMulBit = soundDataSize * 2;//채널 수를 곱함.
			const unsigned soundDataSizeByte = soundDataSizeMulBit * 2;//비트 수를 곱함.
			short* bufferData;
			soundRenderClient->GetBuffer(soundDataSize, (BYTE * *)& bufferData);//soundDataSize는 적용할 데이터 크기(곱 채널,비트수하면 바이트수 나옴.)
			Memory::Set(bufferData, (short)0, (size_t)soundDataSizeMulBit);

			for (auto sound : sounds) {
				if (!sound->playing)continue;
				sound->soundMutex.lock();
				const short* const sData = (short*)sound->data->rawData + sound->pos / 2;
				sound->pos += soundDataSizeByte;
				if (sound->data->size - 1 <= sound->pos) {
					soundRemain = (sound->data->size - (sound->pos - soundDataSizeByte)) / 2;
					sound->loopCount++;
					sound->pos = 0;
					if (sound->loopCount == sound->loop) {
						sound->playing = false;
						sound->loopCount = 0;
					}
					if (sound->volume < 1.f) {
						for (size_t j = 0; j < soundRemain; j++) {
							const short soundShortData = (short)((float)sData[j] * sound->volume);
							const int totalShortSoundData = (int)bufferData[j] + (int)soundShortData;
							if (totalShortSoundData > SHRT_MAX) bufferData[j] = SHRT_MAX;
							else if (totalShortSoundData < SHRT_MIN) bufferData[j] = SHRT_MIN;
							else bufferData[j] += soundShortData;
						}
					} else {
						for (size_t j = 0; j < soundRemain; j++) {
							const int totalShortSoundData = (int)bufferData[j] + (int)sData[j];
							if (totalShortSoundData > SHRT_MAX) bufferData[j] = SHRT_MAX;
							else if (totalShortSoundData < SHRT_MIN) bufferData[j] = SHRT_MIN;
							else bufferData[j] += sData[j];
						}
					}
				} else {
					if (sound->volume < 1.f) {
						for (size_t j = 0; j < soundDataSizeMulBit; j++) {
							const short soundShortData = (short)((float)sData[j] * sound->volume);
							const int totalShortSoundData = (int)bufferData[j] + (int)soundShortData;
							if (totalShortSoundData > SHRT_MAX) bufferData[j] = SHRT_MAX;
							else if (totalShortSoundData < SHRT_MIN) bufferData[j] = SHRT_MIN;
							else bufferData[j] += soundShortData;
						}
					} else {
						for (size_t j = 0; j < soundDataSizeMulBit; j++) {
							const int totalShortSoundData = (int)bufferData[j] + (int)sData[j];
							if (totalShortSoundData > SHRT_MAX) bufferData[j] = SHRT_MAX;
							else if (totalShortSoundData < SHRT_MIN) bufferData[j] = SHRT_MIN;
							else bufferData[j] += sData[j];
						}
					}
				}
				sound->soundMutex.unlock();
			}
			if (soundExit)break;
			soundRenderClient->ReleaseBuffer(soundDataSize, 0);

		}
		WaitForSingleObject(hEvent, INFINITE);
	}
	CloseHandle(hEvent);
	soundClient->Stop();
	soundClient->Release();
	soundRenderClient->Release();
	soundVolume->Release();
	soundClock->Release();
	soundDevice->Release();
	soundDeviceEnumerator->Release();
	CoUninitialize();
	sounds.Free();
}
#elif __ANDROID__
void Sound::ThreadFunc() {
	unsigned soundRemain;
	while (!soundExit) {
		while (bufferCount < 2) {
			Memory::Set(buffer, 0, BUFFER_LEN / 2);

			for (auto sound : sounds) {
				if (!sound->playing)continue;
				sound->soundMutex.lock();
				const short* const sData = (short*)sound->data->rawData + sound->pos / 2;
				sound->pos += BUFFER_LEN;
				if (sound->data->size - 1 <= sound->pos) {
					soundRemain = (sound->data->size - (sound->pos - BUFFER_LEN)) / 2;
					sound->loopCount++;
					sound->pos = 0;
					if (sound->loopCount == sound->loop) {
						sound->playing = false;
						sound->loopCount = 0;
					}
					if (sound->volume < 1.f) {
						for (size_t j = 0; j < soundRemain; j++) {
							const short soundShortData = (short)((float)sData[j] * sound->volume);
							const int totalShortSoundData = (int)buffer[j] + (int)soundShortData;
							if (totalShortSoundData > SHRT_MAX) buffer[j] = SHRT_MAX;
							else if (totalShortSoundData < SHRT_MIN) buffer[j] = SHRT_MIN;
							else buffer[j] += soundShortData;
						}
					}
					else {
						for (size_t j = 0; j < soundRemain; j++) {
							const int totalShortSoundData = (int)buffer[j] + (int)sData[j];
							if (totalShortSoundData > SHRT_MAX) buffer[j] = SHRT_MAX;
							else if (totalShortSoundData < SHRT_MIN) buffer[j] = SHRT_MIN;
							else buffer[j] += sData[j];
						}
					}
				}
				else {

					if (sound->volume < 1.f) {
						for (size_t j = 0; j < BUFFER_LEN / 2; j++) {
							const short soundShortData = (short)((float)sData[j] * sound->volume);
							const int totalShortSoundData = (int)buffer[j] + (int)soundShortData;
							if (totalShortSoundData > SHRT_MAX) buffer[j] = SHRT_MAX;
							else if (totalShortSoundData < SHRT_MIN) buffer[j] = SHRT_MIN;
							else buffer[j] += soundShortData;
						}
					}
					else {
						for (size_t j = 0; j < BUFFER_LEN / 2; j++) {
							const int totalShortSoundData = (int)buffer[j] + (int)sData[j];
							if (totalShortSoundData > SHRT_MAX)buffer[j] = SHRT_MAX;
							else if (totalShortSoundData < SHRT_MIN)buffer[j] = SHRT_MIN;
							else buffer[j] += sData[j];
						}
					}
				}
				sound->soundMutex.unlock();
			}
			(*bufferQueueItf)->Enqueue(bufferQueueItf, buffer, BUFFER_LEN);

			bufferCount++;
		}
		if (soundExit)break;
		soundEvent.Wait();
		bufferCount--;
	}
	(*playItf)->SetPlayState(playItf, SL_PLAYSTATE_STOPPED);
	(*playerObject)->Destroy(playerObject);
	(*outputMixObject)->Destroy(outputMixObject);
	(*engineObject)->Destroy(engineObject);
	sounds.Free();
}
void Sound::BufferQueueCallback(SLBufferQueueItf caller, void* pContext) {
	soundEvent.Set();
}
#endif
void Sound::Init(size_t _maxSoundLen, unsigned _samplingRate/* = 44100*/) {//반드시 _samplingRate, 2채널 16비트여야함.
	sounds.Alloc(_maxSoundLen);
#ifdef _WIN32
	CoInitializeEx(0, COINIT_MULTITHREADED);

	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&soundDeviceEnumerator);
	hr = soundDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &soundDevice);
	hr = soundDevice->Activate(__uuidof(IAudioClient), CLSCTX_INPROC_SERVER, nullptr, (void**)&soundClient);

	WAVEFORMATEXTENSIBLE soundFormat;
	soundFormat.Format.cbSize = 22;
	soundFormat.Format.nChannels = 2;
	soundFormat.Format.wBitsPerSample = 16;
	soundFormat.Format.nBlockAlign = (soundFormat.Format.nChannels*soundFormat.Format.wBitsPerSample) / 8;
	soundFormat.Format.nSamplesPerSec = _samplingRate;
	soundFormat.Format.nAvgBytesPerSec = soundFormat.Format.nBlockAlign*soundFormat.Format.nSamplesPerSec;
	soundFormat.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	soundFormat.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
	soundFormat.Samples.wValidBitsPerSample = 16;
	soundFormat.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;

	//100000 == 0.01 second;
	hr = soundClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_RATEADJUST | AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 100000, 0,
		(WAVEFORMATEX*)&soundFormat, nullptr);
	hr = soundClient->GetBufferSize(&soundBufferSize);
	hr = soundClient->GetService(__uuidof(IAudioRenderClient), (void**)&soundRenderClient);
	hr = soundClient->GetService(__uuidof(IChannelAudioVolume), (void**)&soundVolume);
	hr = soundClient->GetService(__uuidof(IAudioClock), (void**)&soundClock);

	SetMasterVolume(1.f);

	hEvent = CreateEvent(nullptr, false, false, nullptr);

	hr = soundClient->SetEventHandle(hEvent);
#elif __ANDROID__
	slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);

	(*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);

	(*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engine);

	(*engine)->CreateOutputMix(engine, &outputMixObject,0,nullptr,nullptr);

	(*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

	bufferQueue.locatorType = SL_DATALOCATOR_BUFFERQUEUE;
	bufferQueue.numBuffers = 2;

	
	format.formatType = SL_DATAFORMAT_PCM;
	format.numChannels = 2;
	format.samplesPerSec = SL_SAMPLINGRATE_44_1;
	format.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
	format.containerSize = 16;
	format.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
	format.endianness = SL_BYTEORDER_LITTLEENDIAN;

	audioSource.pFormat = (void*)&format;
	audioSource.pLocator = (void*)&bufferQueue;

	locatorOutputMix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	locatorOutputMix.outputMix = outputMixObject;

	audioSink.pLocator = (void*)&locatorOutputMix;
	audioSink.pFormat = nullptr;
	
	iidArray[0] = SL_IID_BUFFERQUEUE;
	required[0] = SL_BOOLEAN_TRUE;
	iidArray[1] = SL_IID_VOLUME;
	required[1] = SL_BOOLEAN_TRUE;

	(*engine)->CreateAudioPlayer(engine, &playerObject, &audioSource, &audioSink, 2, iidArray, required);

	(*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);

	(*playerObject)->GetInterface(playerObject, SL_IID_VOLUME, (void*)& volumeItf);

	(*volumeItf)->GetMaxVolumeLevel(volumeItf, &maxLevel);

	(*playerObject)->GetInterface(playerObject, SL_IID_PLAY, (void*)&playItf);

	(*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, (void*)&bufferQueueItf);

	(*bufferQueueItf)->RegisterCallback(bufferQueueItf, BufferQueueCallback, nullptr);

	(*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PLAYING);
#endif
	soundThread = thread(Sound::ThreadFunc);
}
void Sound::Release() {
	soundExit = true;
	soundThread.join();
}