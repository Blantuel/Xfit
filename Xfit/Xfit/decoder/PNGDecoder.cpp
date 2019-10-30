#include "PNGDecoder.h"

void PNGDecoder::LoadHeader(const void* _data,unsigned _size, ImageType _outputType/* = ImageType::RGBA*/) {
#ifdef _DEBUG
	if(!_data)throw DecoderError(DecoderError::Code::NullData);
	if(_size==0)throw DecoderError(DecoderError::Code::ZeroSize);
#endif
	if(HasHeader()){
		decoded=false;
		png_image_free(&image);
	}
	memset(&image,0,sizeof(image));
	image.version = PNG_IMAGE_VERSION;
	if (!png_image_begin_read_from_memory(&image, _data, _size)) {
		png_image_free(&image);
		hasHeader=false;
		throw DecoderError(DecoderError::Code::HeaderLoadFail);
	}
	
	inputData=_data;
	inputSize=_size;

	width = image.width;
	height = image.height;

	outputType=_outputType;
	switch (outputType) {
	case ImageType::RGBA:image.format = PNG_FORMAT_RGBA; break;
	case ImageType::RGB:image.format = PNG_FORMAT_RGB; break;
	case ImageType::ARGB:image.format = PNG_FORMAT_ARGB; break;
	case ImageType::ABGR:image.format = PNG_FORMAT_ABGR; break;
	case ImageType::BGRA:image.format = PNG_FORMAT_BGRA; break;
	case ImageType::BGR:image.format = PNG_FORMAT_BGR; break;
	default:throw ImageDecoderError(ImageDecoderError::Code::InvalidOutputType);
	}
	outputSize=PNG_IMAGE_SIZE(image);

	hasHeader=true;
}
PNGDecoder::~PNGDecoder() {
	if(HasHeader())png_image_free(&image);
}
void PNGDecoder::Decode(void* _outputData) {
#ifdef _DEBUG
	if(!_outputData)throw DecoderError(DecoderError::Code::NullData);
	if(IsDecoded())throw DecoderError(DecoderError::Code::AlreadyDecoded);
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	if (!png_image_finish_read(&image, nullptr, _outputData, 0, nullptr)) {
		png_image_free(&image);
		hasHeader=false;
		throw DecoderError(DecoderError::Code::DecodeFail);
	}
	decoded=true;
}