#include "ImageDecoder.h"

unsigned ImageDecoder::GetWidth()const { 
#ifdef _DEBUG
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	return width; 
}
unsigned ImageDecoder::GetHeight()const {
#ifdef _DEBUG
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	return height;
}
ImageType ImageDecoder::GetOutputType()const {
#ifdef _DEBUG
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	return outputType;
}