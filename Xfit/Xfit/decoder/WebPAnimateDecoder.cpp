#include "WebPAnimateDecoder.h"
#include "../data/Memory.h"

unsigned WebPAnimateDecoder::GetFrameLen()const {
	return frameLen;
}
unsigned WebPAnimateDecoder::GetFrameSize()const {
	return frameSize;
}
void WebPAnimateDecoder::LoadHeader(const void* _data, unsigned _size, ImageType _outputType/* = ImageType::RGBA*/) {
	if (hasHeader) WebPAnimDecoderDelete(dec);
	
	WebPAnimDecoderOptions decOptions;
	WebPAnimDecoderOptionsInit(&decOptions);

	outputType = _outputType;
	inputData = _data;
	inputSize = _size;

	switch (outputType) {
	case ImageType::RGBA:
		decOptions.color_mode = MODE_RGBA;
		break;
	case ImageType::BGRA:
		decOptions.color_mode = MODE_BGRA;
		break;
	case ImageType::RGB:
	case ImageType::ARGB:
	case ImageType::BGR:
	case ImageType::ABGR:
	default:throw ImageDecoderError(ImageDecoderError::Code::InvalidOutputType);
	}

	WebPData webPData;
	webPData.bytes = (const uint8_t*)_data;
	webPData.size = _size;
	dec = WebPAnimDecoderNew(&webPData, &decOptions);
	WebPAnimInfo animInfo;
	WebPAnimDecoderGetInfo(dec, &animInfo);

	width = animInfo.canvas_width;
	height = animInfo.canvas_height;

	frameSize = width * height * 4;
	frameLen = animInfo.frame_count;

	outputSize = frameSize * frameLen;

	hasHeader = true;
}
void WebPAnimateDecoder::Decode(void* _outputData) {
	unsigned i = 0;
	while (WebPAnimDecoderHasMoreFrames(dec)) {
		uint8_t* buf;
		int timestamp;
		WebPAnimDecoderGetNext(dec, &buf, &timestamp);
		Memory::Copy((uint8_t*)_outputData+i*frameSize, frameSize, buf, frameSize);
		i++;
	}
	decoded = true;
}
WebPAnimateDecoder::~WebPAnimateDecoder() {
	if(hasHeader) WebPAnimDecoderDelete(dec);
}