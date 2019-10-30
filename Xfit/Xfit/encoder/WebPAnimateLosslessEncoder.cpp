#include "WebPAnimateLosslessEncoder.h"

void* WebPAnimateLosslessEncoder::Encode(const void** _input, ImageType _inputType, unsigned _len, unsigned _width, unsigned _height) {
	WebPAnimEncoderOptions encOptions;
	WebPAnimEncoderOptionsInit(&encOptions);
	WebPAnimEncoder *enc = WebPAnimEncoderNew(_width, _height, &encOptions);
	inputType = _inputType;
	inputData = _input;
	width = _width;
	height = _height;
	unsigned stride;

	WebPData webPdata;
	WebPDataInit(&webPdata);
	for(unsigned i=0;i<_len;i++) {
		WebPConfig config;
		WebPConfigInit(&config);
		config.lossless = 1;
		config.quality = 100;
		config.method = 6;

		WebPPicture pic;
		WebPPictureInit(&pic);
		pic.width = _width;
		pic.height = _height;
		pic.use_argb = 1;

		WebPPictureAlloc(&pic);

		
		switch (inputType) {
		case ImageType::RGB:
			WebPPictureImportRGB(&pic, (const uint8_t*)_input[i], width * 3);
			break;
		case ImageType::BGR:
			WebPPictureImportBGR(&pic, (const uint8_t*)_input[i], width * 3);
			break;
		case ImageType::RGBA:
			WebPPictureImportRGBA(&pic, (const uint8_t*)_input[i], width * 4);
			break;
		case ImageType::BGRA:
			WebPPictureImportBGRA(&pic, (const uint8_t*)_input[i], width * 4);
			break;
		}
		WebPAnimEncoderAdd(enc, &pic, 1, &config);
		WebPPictureFree(&pic);
	}
	WebPAnimEncoderAdd(enc, nullptr, 1, nullptr);
	WebPAnimEncoderAssemble(enc, &webPdata);
	WebPAnimEncoderDelete(enc);

	outputSize = webPdata.size;
	encoded = true;
	return (void*)webPdata.bytes;
}