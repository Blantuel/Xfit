#pragma once

#include "ImageDecoder.h"
#include "../stdafx.h"

class PNGDecoder : public ImageDecoder {
	png_image image;
public:
	void LoadHeader(const void* _data,unsigned _size, ImageType _outputType = ImageType::RGBA);
	void Decode(void* _outputData);
	~PNGDecoder();
};