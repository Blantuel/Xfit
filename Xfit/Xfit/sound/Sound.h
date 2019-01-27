#pragma once

#include "../data/Array.h"

struct SoundSource {
	void* rawData;
	size_t size;
	/*unsigned samplesPerSec;
	unsigned short nChannels;
	unsigned short bitsPerSample;
	
	unsigned GetBytesPerSec()const {return samplesPerSec*nChannels*GetBytesPerSample();}
	unsigned GetBytesPerSample() const{return bitsPerSample/8;}*/
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

	bool Decode(SoundSource* _data);
	SoundSource* GetData()const;
	Sound();
	static bool SetMasterVolume(float _volume);
	static float GetMasterVolume() { return masterVolume; }

	void SetVolume(float _volume);
	float GetVolume()const;

	unsigned GetLoop()const;
	unsigned GetLoopCount()const;

	unsigned GetSamplePlayPos() const;
	static unsigned long long GetMasterPlaySpeed();
	~Sound();
	bool Play(unsigned _loop);

	bool SetSamplePlayPos(unsigned _pos);
	bool Stop();
	bool Pause();
	bool Resume();
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