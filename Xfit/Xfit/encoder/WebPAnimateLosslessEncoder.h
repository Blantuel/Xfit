#pragma once

#include "ImageEncoder.h"

class WebPAnimateLosslessEncoder : public ImageEncoder {
public:
	void* Encode(const void** _input, ImageType _inputType, unsigned _len, unsigned _width, unsigned _height);//모든 프레임의 너비, 높이가 같아야 합니다.
};