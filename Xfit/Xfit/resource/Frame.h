#pragma once

#include "../system/Error.h"
#include "FrameFormat.h"
#include "FrameCompressFormat.h"

class FrameError : public Error {
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
	FrameError(Code _code) :code(_code) {}
};
class Frame {
	friend class ImageBase;
	friend class ImageInstance;

protected:
	unsigned width, height;
#ifdef _WIN32
	union {
		ID3D11ShaderResourceView* srv;
		ID3D11ShaderResourceView1* srv1;
	};
	union {
		ID3D11Texture2D* texture;
		ID3D11Texture2D1* texture1;
	};
#elif __ANDROID__
	GLuint texture;
	GLenum fmt;
#endif

public:
	unsigned GetWidth()const;
	unsigned GetHeight()const;
	bool IsBuild()const;

	Frame();
	~Frame();
	Frame(const void* _data, unsigned _width, unsigned _height, FrameFormat _format = FrameFormat::RGBA);
	void Build(const void* _data, unsigned _width, unsigned _height, FrameFormat _format = FrameFormat::RGBA);
	void Edit(const void* _data, unsigned _width, unsigned _height, unsigned _offsetX = 0, unsigned _offsetY = 0);
	void BuildCompress(const void* _data, unsigned _size, unsigned _width, unsigned _height, FrameCompressFormat _format);
	void EditCompress(const void* _data, unsigned _size, unsigned _width, unsigned _height, unsigned _offsetX = 0, unsigned _offsetY = 0);
	void Delete();
};