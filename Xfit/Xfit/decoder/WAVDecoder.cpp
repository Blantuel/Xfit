#include "WAVDecoder.h"

const void* WAVDecoder::LoadHeaderAndDecode(const void* _data,unsigned _size) {
#ifdef _DEBUG
	if(!_data)throw DecoderError(DecoderError::Code::NullData);
	if(_size==0)throw DecoderError(DecoderError::Code::ZeroSize);
#endif
	if(*(unsigned*)_data!=1179011410){//RIFF
		hasHeader=false;
		decoded=false;
		throw DecoderError(DecoderError::Code::HeaderLoadFail);
	}
	if(*(unsigned*)((char*)_data+8)!=1163280727){//WAVE
		hasHeader=false;
		decoded=false;
		throw DecoderError(DecoderError::Code::HeaderLoadFail);
	}
	unsigned fmt = 0, datapos = 0;
	for (unsigned i = 12; i<_size;) {//RIFF 헤더 크기 12를 건너뛰고 시작
		if (*(unsigned*)((char*)_data + i) == 544501094) {//fmt 
			fmt = i;
			if (fmt&&datapos)break;
		} else if (*(unsigned*)((char*)_data + i) == 1635017060) {//data
			datapos = i;
			if (fmt&&datapos)break;
		}
		i += *(unsigned*)((char*)_data + i + 4) + 8;
	}
	if (!(fmt&&datapos)) {
		hasHeader=false;
		decoded=false;
		throw DecoderError(DecoderError::Code::HeaderLoadFail);
	}

	nChannels = *(unsigned short*)((char*)_data + fmt + 10);
	samplesPerSec = *(unsigned*)((char*)_data + fmt + 12);
	wBitsPerSample = *(unsigned short*)((char*)_data + fmt + 22);

	inputData=_data;
	inputSize=_size;

	outputSize=*(unsigned*)((char*)_data + datapos + 4);

	hasHeader=true;
	decoded=true;
	return (char*)_data + datapos + 8;
}
void* WAVDecoder::LoadHeaderAndDecode(void* _data,unsigned _size) {
	return (void*)LoadHeaderAndDecode((const void*)_data,_size);
}