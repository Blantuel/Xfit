#include "WebPLosslessEncoder.h"

void* WebPLosslessEncoder::Encode(const void* _input, ImageType _inputType, unsigned _width, unsigned _height) {
	inputType = _inputType;
	unsigned stride;
	inputData = _input;
	width = _width;
	height = _height;
	switch (inputType) {
	case ImageType::RGB:
		stride = width * 3;
		inputSize = stride * height;
		outputSize = WebPEncodeLosslessRGB((const uint8_t*)inputData, width, height, stride, (uint8_t**)&outputData);
		encoded = true;
		return outputData;
	case ImageType::BGR:
		stride = width * 3;
		inputSize = stride * height;
		outputSize = WebPEncodeLosslessBGR((const uint8_t*)inputData, width, height, stride, (uint8_t**)&outputData);
		encoded = true;
		return outputData;
	case ImageType::RGBA:
		stride = width * 4;
		inputSize = stride * height;
		outputSize = WebPEncodeLosslessRGBA((const uint8_t*)inputData, width, height, stride, (uint8_t**)&outputData);
		encoded = true;
		return outputData;
	case ImageType::BGRA:
		stride = width * 4;
		inputSize = stride * height;
		outputSize = WebPEncodeLosslessBGRA((const uint8_t*)inputData, width, height, stride, (uint8_t**)&outputData);
		encoded = true;
		return outputData;
	}
	return nullptr;
}