#pragma once

#include "ImageDecoder.h"

class WebPAnimateDecoder : public ImageDecoder {
protected:
	unsigned frameLen;
	unsigned frameSize;
	WebPAnimDecoder* dec;
public:
	unsigned GetFrameLen()const;
	unsigned GetFrameSize()const;

	void LoadHeader(const void* _data, unsigned _size, ImageType _outputType = ImageType::RGBA);
	void Decode(void* _outputData);
	~WebPAnimateDecoder();
};