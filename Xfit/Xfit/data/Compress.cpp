#include "Compress.h"

#ifdef __ANDROID__
#include "zlib.h"
#elif _WIN32
#include "../../Zlib/zlib.h"
#endif

void* Compress::CompressZip(const void* input, unsigned len, unsigned* destLen) {
	*destLen = len + len / 100 + 12;
	unsigned char* output = new unsigned char[*destLen];

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

	stream.next_in = (Bytef*)input;
	stream.avail_in = len;
	
	stream.next_out = output;
	stream.avail_out = *destLen;
	stream.data_type = Z_BINARY;

	err = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
	if (err != Z_OK) return nullptr;

	err = deflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		deflateEnd(&stream);
		return nullptr;
	}
	*destLen = stream.total_out;

	deflateEnd(&stream);

	return output;
}
void* Compress::UncompressZip(const void* input, unsigned len, unsigned destLen) {
	unsigned char* output = new unsigned char[destLen];

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

	stream.next_in = (Bytef*)input;
	stream.avail_in = len;

	stream.next_out = output;
	stream.avail_out = destLen;
	stream.data_type = Z_BINARY;

	err = inflateInit(&stream);
	if (err != Z_OK) return nullptr;

	err = inflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		inflateEnd(&stream);
		return nullptr;
	}

	err = inflateEnd(&stream);

	return output;
}