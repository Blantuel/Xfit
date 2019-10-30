#include "Compress.h"



void Compress::CompressZip(const void* _input, unsigned _inputSize, void* _output, unsigned* _outOutputSize) {
#ifdef _DEBUG
	if(!_input)throw CompressError(CompressError::Code::NullInput);
	if(!_output)throw CompressError(CompressError::Code::NullOutput);
	if(_inputSize==0)throw CompressError(CompressError::Code::ZeroInputSize);
	if(!_outOutputSize)throw CompressError(CompressError::Code::NullOutOutputSize);
#endif
	*_outOutputSize = _inputSize + _inputSize / 100 + 12;

	int err;
	z_stream stream;
	stream.total_in = 0;
	stream.total_out = 0;

	stream.msg = nullptr;
	stream.state = nullptr;

	stream.zalloc = nullptr;
	stream.zfree = nullptr;
	stream.opaque = nullptr;

	stream.adler = 0;

	stream.next_in = (Bytef*)_input;
	stream.avail_in = _inputSize;
	
	stream.next_out = (Bytef*)_output;
	stream.avail_out = *_outOutputSize;
	stream.data_type = Z_BINARY;
	
	err = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
	if (err != Z_OK) {
		deflateEnd(&stream);
		throw CompressError(CompressError::Code::Compress);
	}

	err = deflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		deflateEnd(&stream);
		throw CompressError(CompressError::Code::Compress);
	}
	*_outOutputSize = stream.total_out;

	err = deflateEnd(&stream);
	if (err == Z_STREAM_ERROR) {
		throw CompressError(CompressError::Code::Compress);
	}
}
void Compress::UncompressZip(const void* _input, unsigned _inputSize, void* _output, unsigned _outputSize) {
#ifdef _DEBUG
	if(!_input)throw CompressError(CompressError::Code::NullInput);
	if(!_output)throw CompressError(CompressError::Code::NullOutput);
	if(_inputSize==0)throw CompressError(CompressError::Code::ZeroInputSize);
	if(_outputSize==0)throw CompressError(CompressError::Code::ZeroOutputSize);
#endif
	int err;
	z_stream stream;
	stream.total_in=0; 
	stream.total_out=0;

	stream.msg=nullptr;
	stream.state = nullptr;

	stream.zalloc = nullptr;
	stream.zfree = nullptr;
	stream.opaque = nullptr;

	stream.adler=0;

	stream.next_in = (Bytef*)_input;
	stream.avail_in = _inputSize;

	stream.next_out = (Bytef*)_output;
	stream.avail_out = _outputSize;
	stream.data_type = Z_BINARY;

	err = inflateInit(&stream);
	if (err != Z_OK) {
		inflateEnd(&stream);
		throw CompressError(CompressError::Code::Uncompress);
	}

	err = inflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		inflateEnd(&stream);
		throw CompressError(CompressError::Code::Uncompress);
	}
	err = inflateEnd(&stream);
	if (err == Z_STREAM_ERROR) {
		throw CompressError(CompressError::Code::Uncompress);
	}
}