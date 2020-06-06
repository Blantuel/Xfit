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
struct SLBufferQueueItf_;
typedef const struct SLBufferQueueItf_* const* SLBufferQueueItf;

class Sound {
	static void ThreadFunc();
#ifdef __ANDROID__
	static void BufferQueueCallback(SLBufferQueueItf caller, void* pContext);
#endif
	size_t pos;
	
	float volume;

	bool playing,paused;
	unsigned loop, loopCount;
	
	SoundSource* data;

	static inline Array<Sound*> sounds;

	std::mutex soundMutex;

	static inline bool soundExit = false;
public:
	static bool Init(size_t _maxSoundLen, unsigned _samplingRate = 44100);
	static void Release();

	void Decode(SoundSource* _data);
	SoundSource* GetData()const;
	Sound();
	static void SetMute(bool _mute);
	static void SetMasterVolume(float _volume);

	void SetVolume(float _volume);
	float GetVolume()const;

	unsigned GetLoop()const;
	unsigned GetLoopCount()const;

	unsigned GetPos() const;
	~Sound();
	void Play(unsigned _loop);

	void SetPos(unsigned _pos);
	void Stop();
	void Pause();
	void Resume();
};
