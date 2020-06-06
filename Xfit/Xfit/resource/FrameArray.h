#pragma once

#include "../system/Error.h"
#include "FrameFormat.h"
#include "FrameCompressFormat.h"

class FrameArrayError : public Error {
public:
	enum class Code {
		NotBuild,
		AlreadyBuild,
		TooBigWidth,
		TooBigHeight,
		IsCompressTexture,
		NotCompressTexture
	};
protected:
	Code code;
public:
	Code GetCode()const { return code; }
	FrameArrayError(Code _code) :code(_code) {}
};

class FrameArray {
	friend class ImageMultiInstance;

#ifdef _WIN32
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView1* srv1;
	ID3D11Texture2D* texture;
	ID3D11Texture2D1* texture1;
#elif __ANDROID__
	GLuint texture;
	GLenum fmt;
#endif
	unsigned width, height, len;
	bool isCompressed;
public:
	unsigned GetWidth()const;
	unsigned GetHeight()const;
	unsigned GetFrameLen()const;
	bool IsCompressed()const;
	bool IsBuild()const;

	FrameArray();
	virtual ~FrameArray();
	FrameArray(const void* _data,unsigned _len, unsigned _width, unsigned _height, FrameFormat _format = FrameFormat::RGBA, unsigned _mipLevels = 1);
	void Build(const void* _data,unsigned _len, unsigned _width, unsigned _height, FrameFormat _format = FrameFormat::RGBA, unsigned _mipLevels = 1);
	void Edit(const void* _data,unsigned _start,unsigned _len,unsigned _width,unsigned _height,unsigned _offsetX=0,unsigned _offsetY=0);
	void BuildCompress(const void* _data, unsigned _size, unsigned _len, unsigned _width, unsigned _height, FrameCompressFormat _format);
	void EditCompress(const void* _data, unsigned _size, unsigned _start, unsigned _len, unsigned _width, unsigned _height, unsigned _offsetX=0, unsigned _offsetY=0);

	void Delete();
};