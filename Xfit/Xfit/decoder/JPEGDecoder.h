#pragma once

#include "ImageDecoder.h"
#include "../stdafx.h"

class JPEGDecoder : public ImageDecoder {
	jpeg_decompress_struct jpegInfo;
	jpeg_error_mgr jerr;
public:
	JPEGDecoder();
	~JPEGDecoder();
	void LoadHeader(const void* _data,unsigned _size, ImageType _outputType = ImageType::RGBA);
	void Decode(void* _outputData);
};