#pragma once


#include "../resource/Vertex.h"

#include "../math/Point3D.h"


class Converter {
protected:
	const void *source;
	void *dest;
	unsigned sSize, dSize;
public:
	unsigned GetInputSize() const { return sSize; }
	unsigned GetOutputSize() const { return dSize; }
	const void* GetInputData()const { return source; }
	void* GetOutputData()const { return dest; }
	Converter() :source(nullptr), dest(nullptr), sSize(0), dSize(0) {}
	void Free() {
		source = nullptr;
		dest = nullptr;
		sSize = 0;
		dSize = 0;
	}
};

class ImageConverter : public Converter {
public:
	enum class Type {RGB, BGR, RGBA, BGRA, ARGB, ABGR, UNKNOWN};
	ImageConverter() : Converter(),inputType(Type::UNKNOWN), outputType(Type::UNKNOWN), width(0), height(0) {}
	Type GetInputType() const { return inputType; }
	Type GetOutputType() const { return outputType; }
	unsigned GetInputBitsPerPixel()const {
		if ((int)inputType < 2)return 24;
		if ((int)inputType < 10)return 32;
		return 0;
	}
	unsigned GetOutputBitsPerPixel()const {
		if ((int)outputType < 2)return 24;
		if ((int)outputType < 10)return 32;
		return 0;
	}
	void Free() {
		Converter::Free();
		width = 0;
		height = 0;
		inputType = Type::UNKNOWN;
		outputType = Type::UNKNOWN;
	}
	unsigned GetWidth()const { return width; }
	unsigned GetHeight()const { return height; }
	
protected:
	unsigned width, height;
	Type inputType, outputType;
};

class JPEGConverter :public ImageConverter {
public:
	JPEGConverter() :ImageConverter() {}
	void Decode(const void* data, unsigned size, Type _outputType = Type::RGBA) {DecodeToOut(data, &dest, size, _outputType);}
	void DecodeToOut(const void* data, void** outData, unsigned size, Type _outputType = Type::RGBA);
};
class PNGConverter :public ImageConverter {
public:
	PNGConverter() :ImageConverter() {}
	void Decode(const void* data, unsigned size, Type _outputType = Type::RGBA) {DecodeToOut(data, &dest, size, _outputType);}
	void DecodeToOut(const void* data, void** outData, unsigned size, Type _outputType = Type::RGBA);
};
class SoundConverter : public Converter {
protected:
	unsigned samplesPerSec, nChannels, wBitsPerSample;
	bool decoded;
public:
	SoundConverter() :Converter(), decoded(false), samplesPerSec(0), nChannels(0), wBitsPerSample(0) {}
	unsigned GetwBitsPerSample() const { return wBitsPerSample; }
	unsigned GetNumChannels()const { return nChannels; }
	unsigned GetSamplesPerSec()const { return samplesPerSec; }
	bool IsDecoded()const { return decoded; }
	void Free() {
		Converter::Free();
		decoded = false;
		samplesPerSec = 0;
		nChannels = 0;
		wBitsPerSample = 0;
	}
};

class WAVConverter : public SoundConverter {
	unsigned dataPos;
public:
	WAVConverter() :SoundConverter(), dataPos(0){}

	void Decode(const void* data, unsigned size, bool copyNewData=false) {DecodeToOut(data, &dest, size, copyNewData);}
	void DecodeToOut(const void* data, void** outData, unsigned size, bool copyNewData = false);
	unsigned GetDataPos()const { return dataPos; }
};
 
class OGGConverter : public SoundConverter {
	struct OGGC {
		const void* data;
		int oggpos;
		int oggsize;
	};
	OGGC g;

	static size_t oggRead(void* ptr, size_t size, size_t nmemb, void *datasource);
	static int oggSeek(void* ptr, long long offset, int whence);
	static int oggClose(void *datasource);
	static long oggTell(void *datasource);

public:
	OGGConverter() :SoundConverter() {}

	void Decode(const void* data, unsigned size) {DecodeToOut(data, &dest, size);}
	void DecodeToOut(const void* data, void** outData, unsigned size);
};

struct ObjData {
	Point3DF* posAr;
	Point3DF* normalAr;
	Point3DwF* colorAr;
	PointF* uvAr;
	unsigned* indexAr;
	unsigned verticesLen;
	unsigned indicesLen;

	float specularReflect;
	Point3DF color;

	char* objectName;
	char* materialName;
	char* mtlPath;
	char* texturePath;
	char* normalTexturePath;
};

class ObjConverter : public Converter{
protected:
public:
	ObjData* Getvertices()const { return (ObjData*)GetOutputData(); }

	bool Decode(const void* data, unsigned size, const void* mtldata,unsigned mtlsize);
};