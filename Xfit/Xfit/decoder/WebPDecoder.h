#pragma once

#include "ImageDecoder.h"

class WebPDecoder : public ImageDecoder {
protected:
public:
	void LoadHeader(const void* _data, unsigned _size, ImageType _outputType = ImageType::RGBA);
	void Decode(void* _outputData);
};