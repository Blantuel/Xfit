#pragma once

#include "../data/Array.h"

struct SoundSource {
	void* rawData;
	size_t size;
	unsigned samplesPerSec;
	unsigned short nChannels;
	unsigned short bitsPerSample;
	
	unsigned GetBytesPerSec()const {return samplesPerSec*nChannels*GetBytesPerSample();}
	unsigned GetBytesPerSample() const{return bitsPerSample/8;}
};
class Sound {
	static void ThreadFunc();
	size_t pos;
	
	float volume;

	bool playing,paused;
	unsigned loop, loopCount;
	
	SoundSource* data;

	static inline Array<Sound*> sounds;

	static inline thread soundThread;
	mutex soundMutex;

	static inline bool soundExit = false;
	static inline float masterVolume = 1.f;
public:
	static void Init(size_t _maxSoundLen);
	static void Release();
	static bool IsInited();

	bool Decode(SoundSource* _data) {
		Stop();
		data = _data;
		return true;
	}
	SoundSource* GetData()const { return data; }
	Sound() :data(nullptr), pos(0), volume(1.f), playing(false), paused(false), loop(1), loopCount(0) {sounds.InsertLast(this);}

	static bool SetMasterVolume(float _volume);
	static float GetMasterVolume() { return masterVolume; }

	bool SetVolume(float _volume);
	float GetVolume()const { return volume; }
	bool SetChannelVolumes(const float* _volumes);
	void GetChannelVolumes(float* _volumes)const;

	unsigned GetLoop()const { return loop; }
	unsigned GetLoopCount()const { return loopCount; }

	unsigned GetSamplePlayPos() const { return pos; }
	static unsigned long long GetMasterPlaySpeed();
	~Sound() {
		soundMutex.lock();
		sounds.EraseIndex(sounds.Search(this));
		soundMutex.unlock();
	}
	bool Play(unsigned _loop) {
		if (playing || paused)return false;
		soundMutex.lock();
		loop = _loop;
		playing = true;

		soundMutex.unlock();
		return true;
	}

	bool SetSamplePlayPos(unsigned _pos) {
		soundMutex.lock();
		pos = _pos;
		soundMutex.unlock();
		return true;
	}
	bool Stop() {
		if ((!playing) && (!paused))return false;
		soundMutex.lock();

		playing = false;
		paused = false;
		loopCount = 0;
		pos = 0;
		soundMutex.unlock();
		return true;
	}
	bool Pause() {
		if (playing && (!paused)) {
			soundMutex.lock();
			playing = false;
			paused = true;
			soundMutex.unlock();
			return true;
		}
		return false;
	}
	bool Resume() {
		if ((!playing) && paused) {
			soundMutex.lock();
			playing = true;
			paused = false;
			soundMutex.unlock();
			return true;
		}
		return false;
	}
};
#ifdef _WIN32
namespace _System::_Sound_Windows {
	inline HANDLE hEvent;

	inline IMMDevice* soundDevice;
	inline IMMDeviceEnumerator* soundDeviceEnumerator;
	inline UINT32 soundBufferSize;

	inline IAudioClient* soundClient = nullptr;//check sound inited too
	inline IAudioRenderClient* soundRenderClient;
	inline IChannelAudioVolume* soundVolume;
	inline IAudioClock* soundClock;

	inline UINT32 channelCount;
}
#endif