#pragma once

#include "ImageEncoder.h"

class WebPAnimateLosslessEncoder : public ImageEncoder {
public:
	void* Encode(const void** _input, ImageType _inputType, unsigned _len, unsigned _width, unsigned _height);//��� �������� �ʺ�, ���̰� ���ƾ� �մϴ�.
};