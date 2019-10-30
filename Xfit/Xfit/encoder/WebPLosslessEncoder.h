#pragma once

#include "ImageEncoder.h"

class WebPLosslessEncoder : public ImageEncoder {
public:
	void* Encode(const void* _input,ImageType _inputType, unsigned _width,unsigned _height);
};