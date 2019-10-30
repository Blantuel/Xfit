#pragma once 

#include "SoundDecoder.h"

class WAVDecoder : public SoundDecoder {
public:
	const void* LoadHeaderAndDecode(const void* _data,unsigned _size);
	void* LoadHeaderAndDecode(void* _data,unsigned _size);
};