#pragma once

#include "Decoder.h"
#include "../data/ImageType.h"


class ImageDecoderError : public Error {
public:
	enum class Code {
		InvalidOutputType
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	ImageDecoderError(Code _code):code(_code) {}
};

class ImageDecoder : public Decoder {
public:

	unsigned GetWidth()const;
	unsigned GetHeight()const;
	ImageType GetOutputType()const;
protected:
	ImageType outputType;
	unsigned width,height;
};