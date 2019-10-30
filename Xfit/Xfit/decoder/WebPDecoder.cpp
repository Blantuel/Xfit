#include "WebPDecoder.h"

void WebPDecoder::LoadHeader(const void* _data, unsigned _size, ImageType _outputType/* = ImageType::RGBA*/) {
	int widthT, heightT;
	WebPGetInfo((const uint8_t*)_data, _size, &widthT, &heightT);
	width = (unsigned)widthT;
	height = (unsigned)heightT;
	outputType = _outputType;
	inputData = _data;
	inputSize = _size;
	switch (outputType) {
	case ImageType::RGBA:
		outputSize = width * height * 4;
		break;
	case ImageType::RGB:
		outputSize = width * height * 3;
		break;
	case ImageType::ARGB:
		outputSize = width * height * 4;
		break;
	case ImageType::BGRA:
		outputSize = width * height * 4;
		break;
	case ImageType::BGR:
		outputSize = width * height * 3;
		break;
	case ImageType::ABGR:
	default:throw ImageDecoderError(ImageDecoderError::Code::InvalidOutputType);
	}
	hasHeader = true;
}
void WebPDecoder::Decode(void* _outputData) {
	switch (outputType) {
	case ImageType::RGBA:
		WebPDecodeRGBAInto((const uint8_t*)inputData, inputSize, (uint8_t*)_outputData, outputSize, width * 4);
		break;
	case ImageType::RGB:
		WebPDecodeRGBInto((const uint8_t*)inputData, inputSize, (uint8_t*)_outputData, outputSize, width * 3);
		break;
	case ImageType::ARGB:
		WebPDecodeARGBInto((const uint8_t*)inputData, inputSize, (uint8_t*)_outputData, outputSize, width * 4);
		break;
	case ImageType::BGRA:
		WebPDecodeBGRAInto((const uint8_t*)inputData, inputSize, (uint8_t*)_outputData, outputSize, width * 4);
		break;
	case ImageType::BGR:
		WebPDecodeBGRInto((const uint8_t*)inputData, inputSize, (uint8_t*)_outputData, outputSize, width * 3);
		break;
	}
	decoded = true;
}