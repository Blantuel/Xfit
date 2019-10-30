#include "FrameArray.h"
#include "../_system/_OpenGL.h"

#ifdef __ANDROID__

using namespace _System::_OpenGL;


unsigned FrameArray::GetWidth()const {
#ifdef _DEBUG
	if (!texture);
#endif
	return width;
}
unsigned FrameArray::GetHeight()const {
#ifdef _DEBUG
	if (!texture);
#endif
	return height;
}
unsigned FrameArray::GetFrameLen()const {
#ifdef _DEBUG
	if (!texture);
#endif
	return len;
}
FrameArray::FrameArray() {
#ifdef OPENGL
	texture = 0;
#endif
}
bool FrameArray::IsCompressed()const {
#ifdef _DEBUG
	if (!IsBuild());
#endif
	return isCompressed;
}
bool FrameArray::IsBuild()const { return (bool)texture; }
void FrameArray::Delete() {
#ifdef _DEBUG
	if (!IsBuild());
#endif
	glDeleteTextures(1, &texture);
	texture = 0;
}
FrameArray::~FrameArray() { if(texture)glDeleteTextures(1, &texture); }

FrameArray::FrameArray(const void* _data,unsigned _len, unsigned _width, unsigned _height, FrameFormat _format/* = FrameFormat::RGBA*/) {Build(_data, _len, _width, _height,_format);}
void FrameArray::Build(const void* _data,unsigned _len, unsigned _width, unsigned _height, FrameFormat _format/* = FrameFormat::RGBA*/) {
#ifdef _DEBUG
	if (texture);
#endif
	width = _width;
	height = _height;
	len=_len;
	glGenTextures(1, &texture);

	switch (_format) {
	case FrameFormat::RGBA:
		fmt = GL_RGBA;
		break;
	case FrameFormat::RGB:
		fmt = GL_RGB;
		break;
	case FrameFormat::BGRA:
		fmt = GL_BGRA;
		_format = FrameFormat::RGBA;
		break;
	case FrameFormat::BGR:
		fmt = GL_BGR;
		_format = FrameFormat::RGB;
		break;
	default:
		break;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	if (glTexStorage3D) {
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, (GLenum)_format, width, height, len);
		if (_data)glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, len, fmt, GL_UNSIGNED_BYTE, _data);
	} else {
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, (GLenum)_format, width, height, len ,0, fmt, GL_UNSIGNED_BYTE, _data);
	}
	isCompressed = false;
}
void FrameArray::Edit(const void* _data, unsigned _start, unsigned _len, unsigned _width, unsigned _height, unsigned _offsetX/* = 0*/, unsigned _offsetY/* = 0*/) {
#ifdef _DEBUG
	if (!texture);
#endif
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,_offsetX,_offsetY,_start,_width,_height,_len,fmt,GL_UNSIGNED_BYTE,_data);
}
void FrameArray::BuildCompress(const void* _data, unsigned _size, unsigned _len, unsigned _width, unsigned _height, FrameCompressFormat _format) {
#ifdef _DEBUG
	if (IsBuild());
#endif
	width = _width;
	height = _height;
	len = _len;
	glGenTextures(1, &texture);

	fmt = (GLenum)_format;

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	if (glTexStorage3D) {
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, fmt, width, height, len);
		if (_data)glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, len,fmt, _size, _data);
	} else {
		glCompressedTexImage3D(GL_TEXTURE_2D, 0, fmt, width, height, len, 0, _size, _data);
	}
	isCompressed = true;
}
void FrameArray::EditCompress(const void* _data, unsigned _size, unsigned _start, unsigned _len, unsigned _width, unsigned _height, unsigned _offsetX/* = 0*/, unsigned _offsetY /*= 0*/) {
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, _offsetX, _offsetY, _start,_width, _height, _len, fmt, _size, _data);
}

#endif