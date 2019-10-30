#include "SoundDecoder.h"

unsigned SoundDecoder::GetwBitsPerSample()const {
#ifdef _DEBUG
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	return wBitsPerSample; 
}
unsigned SoundDecoder::GetNumChannels()const {
#ifdef _DEBUG
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	return nChannels; 
}
unsigned SoundDecoder::GetSamplesPerSec()const {
#ifdef _DEBUG
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	return samplesPerSec; 
}
