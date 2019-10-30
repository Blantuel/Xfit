#include "OGGDecoder.h"


OGGDecoder::~OGGDecoder() {
	if(HasHeader()) ov_clear(&vorbisFile);
}
size_t OGGDecoder::oggRead(void* ptr, size_t size, size_t nmemb, void *datasource) {
	OGGC* oggc = (OGGC*)datasource;
	size_t read;
	int oggpos = oggc->oggpos;
	int oggsize = oggc->oggsize;
	if ((oggsize - oggpos) < (size*nmemb)) {
		read = oggsize - oggpos;
		memcpy(ptr,(unsigned char*)(oggc->data) + oggpos, read);
		oggc->oggpos = oggsize;
	} else {
		read = size*nmemb;
		memcpy(ptr, (unsigned char*)(oggc->data) + oggpos, size*nmemb);
		oggc->oggpos += (int)read;
	}
	return read;
}
int OGGDecoder::oggSeek(void* _ptr, long long offset, int whence) {
	OGGC* ptr = (OGGC*)_ptr;
	switch (whence) {
	case SEEK_SET:
		ptr->oggpos = (int)offset;
		break;
	case SEEK_CUR:
		ptr->oggpos += (int)offset;
		break;
	case SEEK_END:
		ptr->oggpos = ptr->oggsize + (int)offset;
		break;
	}
	if (ptr->oggpos > ptr->oggsize)ptr->oggpos = ptr->oggsize;
	return ptr->oggpos;
}
int OGGDecoder::oggClose(void *datasource) {
	return 0;
}
long OGGDecoder::oggTell(void *datasource) {
	return ((OGGC*)datasource)->oggpos;
}
void OGGDecoder::LoadHeader(const void* _data,unsigned _size) {
#ifdef _DEBUG
	if(!_data)throw DecoderError(DecoderError::Code::NullData);
	if(_size==0)throw DecoderError(DecoderError::Code::ZeroSize);
#endif
	if(HasHeader()){
		decoded=false;
		ov_clear(&vorbisFile);
	}
	ov_callbacks b;
	b.close_func = oggClose;
	b.read_func = oggRead;
	b.seek_func = oggSeek;
	b.tell_func = oggTell;

	oggc.oggpos = 0;
	oggc.oggsize = (int)_size;
	oggc.data = _data;

	if (ov_open_callbacks(&oggc, &vorbisFile, nullptr, -1, b)) {
		ov_clear(&vorbisFile);
		hasHeader=false;
		throw DecoderError(DecoderError::Code::HeaderLoadFail);
	}
	vorbis_info*info = ov_info(&vorbisFile, -1);
	nChannels = info->channels;
	samplesPerSec = info->rate;
	wBitsPerSample = 16;

	inputSize = _size;
	inputData=_data;

	outputSize=ov_pcm_total(&vorbisFile, -1) * nChannels * 2;

	hasHeader = true;
}
void OGGDecoder::Decode(void* _outputData) {
#ifdef _DEBUG
	if(!_outputData)throw DecoderError(DecoderError::Code::NullData);
	if(IsDecoded())throw DecoderError(DecoderError::Code::AlreadyDecoded);
	if(!HasHeader())throw DecoderError(DecoderError::Code::NoHeader);
#endif
	char* data = (char*)_outputData;
	int cnt = 0;
	int size = (int)outputSize;
	int sec, r = 1;
	while (true) {
		if ((size - cnt) < 4096) {
			r = ov_read(&vorbisFile, data, size - cnt, 0, 2, 1, &sec);
			break;
		} else {
			r = ov_read(&vorbisFile, data, 4096, 0, 2, 1, &sec);
		}
		if (r < 0) {
			throw DecoderError(DecoderError::Code::DecodeFail);
		} else {
			data += r;
			cnt += r;
		}
	}
	decoded=true;
}