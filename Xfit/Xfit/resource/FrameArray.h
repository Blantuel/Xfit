#pragma once

#include "../system/Error.h"
#include "FrameFormat.h"
#include "FrameCompressFormat.h"

class FrameArray {
	friend class ImageMultiInstance;

#ifdef _WIN32
#elif __ANDROID__
	GLuint texture;
	GLenum fmt;
#endif


	bool isCompressed;
	unsigned width, height, len;
public:
	unsigned GetWidth()const;
	unsigned GetHeight()const;
	unsigned GetFrameLen()const;
	bool IsCompressed()const;
	bool IsBuild()const;

	FrameArray();
	~FrameArray();
	FrameArray(const void* _data,unsigned _len, unsigned _width, unsigned _height, FrameFormat _format = FrameFormat::RGBA);
	void Build(const void* _data,unsigned _len, unsigned _width, unsigned _height, FrameFormat _format = FrameFormat::RGBA);
	void Edit(const void* _data,unsigned _start,unsigned _len,unsigned _width,unsigned _height,unsigned _offsetX=0,unsigned _offsetY=0);
	void BuildCompress(const void* _data, unsigned _size, unsigned _len, unsigned _width, unsigned _height, FrameCompressFormat _format);
	void EditCompress(const void* _data, unsigned _size, unsigned _start, unsigned _len, unsigned _width, unsigned _height, unsigned _offsetX=0, unsigned _offsetY=0);

	void Delete();
};