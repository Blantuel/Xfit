#include "JPEGDecoder.h"

static void error_exit(j_common_ptr cinfo) {
	throw 0;
}
JPEGDecoder::JPEGDecoder() {
	jpegInfo.err = jpeg_std_error(&jerr);

	jpegInfo.err->error_exit=error_exit;
	jpeg_create_decompress(&jpegInfo);
}
JPEGDecoder::~JPEGDecoder() {
	jpeg_destroy_decompress(&jpegInfo);
}

void JPEGDecoder::LoadHeader(const void* _data,unsigned _size, ImageType _outputType/* = ImageType::RGBA*/) {
#ifdef _DEBUG
	if(!_data)throw DecoderError(DecoderError::Code::NullData);
	if(_size==0)throw DecoderError(DecoderError::Code::ZeroSize);
#endif
	if(HasHeader()) {
		decoded=false;
		jpeg_destroy_decompress(&jpegInfo);
		jpegInfo.err = jpeg_std_error(&jerr);

		jpegInfo.err->error_exit=error_exit;
		jpeg_create_decompress(&jpegInfo);
	}
	jpeg_mem_src(&jpegInfo, (unsigned char*)_data, _size);

	try {
		jpeg_read_header(&jpegInfo, true);
	} catch(...) {
		jpeg_destroy_decompress(&jpegInfo);
		jpegInfo.err = jpeg_std_error(&jerr);

		jpegInfo.err->error_exit=error_exit;
		jpeg_create_decompress(&jpegInfo);

		hasHeader=false;
		throw DecoderError(DecoderError::Code::HeaderLoadFail);
	}

	width=(unsigned)jpegInfo.image_width;
	height=(unsigned)jpegInfo.image_height;
	jpegInfo.dither_mode = JDITHER_NONE;

	inputData=_data;
	inputSize=_size;

	outputType=_outputType;
	switch (outputType) {
	case ImageType::RGBA:
		outputSize = width*height*4;
		jpegInfo.out_color_space = JCS_RGBA;
		break;
	case ImageType::RGB:
		outputSize = width*height*3;
		jpegInfo.out_color_space = JCS_RGB; 
		break;
	case ImageType::ARGB:
		outputSize = width*height*4;
		jpegInfo.out_color_space = JCS_ARGB; 
		break;
	case ImageType::ABGR:
		outputSize = width*height*4;
		jpegInfo.out_color_space = JCS_ABGR; 
		break;
	case ImageType::BGRA:
		outputSize = width*height*4;
		jpegInfo.out_color_space = JCS_BGRA;
		break;
	case ImageType::BGR:
		outputSize = width*height*3;
		jpegInfo.out_color_space = JCS_BGR;
		break;
	default:throw ImageDecoderError(ImageDecoderError::Code::InvalidOutputType);
	}

	hasHeader=true;
}
void JPEGDecoder::Decode(void* _outputData) {
#ifdef _DEBUG
	if(!_outputData)throw DecoderError(DecoderError::Code::NullData);
	if(IsDecoded())throw DecoderError(DecoderError::Code::AlreadyDecoded);
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	try {
		jpeg_start_decompress(&jpegInfo);
	} catch(...) {
		jpeg_destroy_decompress(&jpegInfo);
		jpegInfo.err = jpeg_std_error(&jerr);

		jpegInfo.err->error_exit=error_exit;
		jpeg_create_decompress(&jpegInfo);

		hasHeader=false;
		throw DecoderError(DecoderError::Code::DecodeFail);
	}

	const unsigned calcWidth = width * jpegInfo.output_components;
	unsigned char* outputData2 = (unsigned char*)_outputData;
	
	while (jpegInfo.output_scanline < jpegInfo.output_height) {
		jpeg_read_scanlines(&jpegInfo, &outputData2, 1);
		outputData2 += calcWidth;
	}
	
	jpeg_finish_decompress(&jpegInfo);
	decoded=true;
}