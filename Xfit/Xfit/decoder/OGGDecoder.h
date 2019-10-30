#include "SoundDecoder.h"
#include "../stdafx.h"

class OGGDecoder : public SoundDecoder {
protected:
	struct OGGC {
		const void* data;
		int oggpos;
		int oggsize;
	};
	OGGC oggc;
	OggVorbis_File vorbisFile;

	static size_t oggRead(void* ptr, size_t size, size_t nmemb, void *datasource);
	static int oggSeek(void* ptr, long long offset, int whence);
	static int oggClose(void *datasource);
	static long oggTell(void *datasource);
public:
	void LoadHeader(const void* _data,unsigned _size);
	void Decode(void* _outputData);
	~OGGDecoder();
};