#include "Decoder.h"

Decoder::Decoder():hasHeader(false),decoded(false) {}

bool Decoder::HasHeader()const{return hasHeader;}

const void* Decoder::GetInputData()const {
#ifdef _DEBUG
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	return inputData;
}
unsigned Decoder::GetInputSize()const {
#ifdef _DEBUG
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	return inputSize;
}
unsigned Decoder::GetOutputSize()const {
#ifdef _DEBUG
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	return outputSize;
}
bool Decoder::IsDecoded()const {
	return decoded;
}