#pragma once

#include "Decoder.h"

class SoundDecoder : public Decoder {
protected:
	unsigned samplesPerSec, nChannels, wBitsPerSample;
public:
	unsigned GetwBitsPerSample()const;
	unsigned GetNumChannels()const;
	unsigned GetSamplesPerSec()const;
};