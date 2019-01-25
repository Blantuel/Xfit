#include "Sound.h"
#include "../_system/_Windows.h"

unsigned long long Sound::GetMasterPlaySpeed() {
#ifdef _WIN32
	UINT64 speedFr;
	HRESULT hr = _System::_Sound_Windows::soundClock->GetFrequency(&speedFr);
	return (unsigned long long)speedFr;
#elif __ANDROID__
#endif
}
bool Sound::IsInited() { 
#ifdef _WIN32
	return (bool)_System::_Sound_Windows::soundClient; 
#elif __ANDROID__
#endif
}
void Sound::ThreadFunc() {
#ifdef _WIN32
	_System::_Sound_Windows::soundClient->Start();
	while (!soundExit) {
		unsigned soundDataSize;
		unsigned soundDataSizeMulBit;
		unsigned soundRemain;

		unsigned padding;
		_System::_Sound_Windows::soundClient->GetCurrentPadding(&padding);
		soundDataSize = _System::_Sound_Windows::soundBufferSize - padding;
		soundDataSizeMulBit = soundDataSize * 2;
		short* bufferData;
		_System::_Sound_Windows::soundRenderClient->GetBuffer(soundDataSize, (BYTE**)&bufferData);
		Memory::Set(bufferData, (short)0, (size_t)soundDataSizeMulBit);
		
		for (auto sound : sounds) {
			sound->soundMutex.lock();
			if (!sound->playing)continue;
			const short*const sData = (short*)sound->data->rawData + sound->pos * 2;
			if (sound->data->size  < ((sound->pos + soundDataSize) * 2 * 2)) {
				soundRemain = (sound->data->size - sound->pos * 2 * 2) / 2;
				if (sound->volume < 1.f) {
					for (size_t j = 0; j < soundRemain; j++) bufferData[j] += (short)((float)sData[j] * sound->volume);
				} else {
					for (size_t j = 0; j < soundRemain; j++) bufferData[j] += sData[j];
				}
				sound->loopCount++;
				sound->pos = 0;
				if (sound->loopCount == sound->loop) {
					sound->playing = false;
					sound->loopCount = 0;
				}
			} else {
				if (sound->volume < 1.f) {
					for (size_t j = 0; j < soundDataSizeMulBit; j++) bufferData[j] += (short)((float)sData[j] * sound->volume);
				} else {
					for (size_t j = 0; j < soundDataSizeMulBit; j++) bufferData[j] += sData[j];
				}
				sound->pos += soundDataSize;
			}
			sound->soundMutex.unlock();
		}
		_System::_Sound_Windows::soundRenderClient->ReleaseBuffer(soundDataSize, 0);

		WaitForSingleObject(_System::_Sound_Windows::hEvent, INFINITE);
	}
	CloseHandle(_System::_Sound_Windows::hEvent);
	_System::_Sound_Windows::soundClient->Stop();
	_System::_Sound_Windows::soundClient->Release();
	_System::_Sound_Windows::soundRenderClient->Release();
	_System::_Sound_Windows::soundVolume->Release();
	_System::_Sound_Windows::soundClock->Release();
	_System::_Sound_Windows::soundDevice->Release();
	_System::_Sound_Windows::soundDeviceEnumerator->Release();
	CoUninitialize();
	_System::_Sound_Windows::soundClient = nullptr;
#elif __ANDROID__
#endif
	sounds.Free();
}
void Sound::Init(size_t _maxSoundLen) {
#ifdef _WIN32
	CoInitializeEx(0, COINIT_MULTITHREADED);
	sounds.Alloc(_maxSoundLen);

	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&_System::_Sound_Windows::soundDeviceEnumerator);
	hr = _System::_Sound_Windows::soundDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_System::_Sound_Windows::soundDevice);
	hr = _System::_Sound_Windows::soundDevice->Activate(__uuidof(IAudioClient), CLSCTX_INPROC_SERVER, nullptr, (void**)&_System::_Sound_Windows::soundClient);

	WAVEFORMATEXTENSIBLE soundFormat;
	soundFormat.Format.cbSize = 22;
	soundFormat.Format.nChannels = 2;
	soundFormat.Format.wBitsPerSample = 16;
	soundFormat.Format.nBlockAlign = (soundFormat.Format.nChannels*soundFormat.Format.wBitsPerSample) / 8;
	soundFormat.Format.nSamplesPerSec = 44100;
	soundFormat.Format.nAvgBytesPerSec = soundFormat.Format.nBlockAlign*soundFormat.Format.nSamplesPerSec;
	soundFormat.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	soundFormat.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
	soundFormat.Samples.wValidBitsPerSample = 16;
	soundFormat.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;

	//100000 == 0.01 second;
	hr = _System::_Sound_Windows::soundClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_RATEADJUST | AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 100000, 0,
		(WAVEFORMATEX*)&soundFormat, nullptr);
	hr = _System::_Sound_Windows::soundClient->GetBufferSize(&_System::_Sound_Windows::soundBufferSize);
	hr = _System::_Sound_Windows::soundClient->GetService(__uuidof(IAudioRenderClient), (void**)&_System::_Sound_Windows::soundRenderClient);
	hr = _System::_Sound_Windows::soundClient->GetService(__uuidof(IChannelAudioVolume), (void**)&_System::_Sound_Windows::soundVolume);
	hr = _System::_Sound_Windows::soundClient->GetService(__uuidof(IAudioClock), (void**)&_System::_Sound_Windows::soundClock);
	hr = _System::_Sound_Windows::soundVolume->GetChannelCount(&_System::_Sound_Windows::channelCount);

	_System::_Sound_Windows::hEvent = CreateEvent(nullptr, false, false, nullptr);

	hr = _System::_Sound_Windows::soundClient->SetEventHandle(_System::_Sound_Windows::hEvent);
#elif __ANDROID__
#endif

	soundThread = thread(Sound::ThreadFunc);
}
void Sound::Release() {
	soundExit = true;
	soundThread.join();

}