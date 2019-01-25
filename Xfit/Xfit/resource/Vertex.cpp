#pragma once

#include "Vertex.h"

#include "../math/Point.h"
#include "../math/Point3D.h"
#include "../math/Point3Dw.h"


#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

void Vertices::_WriteBufferOpenGL(GLuint* _buffer, const void * _data, unsigned nodeSize) {
	if (!_data) {
		if (*_buffer) {
			_System::_OpenGL::glDeleteBuffers(1, _buffer);
			*_buffer = NULL;
		}
		return;
	}
}

unsigned Vertices::GetVertexlength() const { return vertexlen; }
unsigned Vertices::GetIndexlength() const { return indexlen; }
WriteFlag Vertices::GetWriteFlag() const { return flag; }

void Vertices::WriteVertex(unsigned _vertexlen, const Point3DF * _pos, const PointF* _uv/* = nullptr*/, const Point3DF* _normal /* = nullptr*/, const Point3DwF* _color/* = nullptr*/){
	vertexlen = _vertexlen;
	_WriteBufferOpenGL(&openGL._posBuffer, _pos, sizeof(Point3DF));
	_WriteBufferOpenGL(&openGL._normalBuffer, _normal, sizeof(Point3DF));
	_WriteBufferOpenGL(&openGL._colorBuffer, _color, sizeof(Point3DwF));
	_WriteBufferOpenGL(&openGL._uvBuffer, _uv, sizeof(PointF));

	if (vertexlen > vertexslen) vertexslen = vertexlen + vertexblen;
}

void Vertices::WriteIndex(unsigned _indexlen, const unsigned * _index) {
}

Vertices::Vertices(WriteFlag _flag, unsigned _vertexblen, unsigned _indexblen){
	vertexlen = 0;
	vertexslen = 0;
	indexlen = 0;
	indexslen = 0;
	flag = _flag;
	vertexblen = _vertexblen;
	indexblen = _indexblen;
}

Vertices::~Vertices(){
}

Point3DF * Vertices::BuildNormal(const Point3DF* _pos, const unsigned* _index, unsigned _poslen, unsigned _indexlen, bool _cw) {
	if (_pos&&_index) {
		unsigned i;
		Point3DF *const normals = new Point3DF[_poslen];
		unsigned * const num = new unsigned[_poslen];
		memset(num, 0, sizeof(unsigned)*_poslen);
		memset(normals, 0, sizeof(Point3DF)*_poslen);
		if (_cw) {
			for (i = 0; i < _indexlen; i += 3) {
				unsigned j1 = _index[i], j2 = _index[i + 1], j3 = _index[i + 2];
				Point3DF pp = (_pos[j2] - _pos[j1]).OuterProduct(_pos[j3] - _pos[j1]).Normalize();
				normals[j1] += pp;
				normals[j2] += pp;
				normals[j3] += pp;
				num[j1]++;
				num[j2]++;
				num[j3]++;
			}
		} else {
			for (i = 0; i < _indexlen; i += 3) {
				unsigned j1 = _index[i], j2 = _index[i + 1], j3 = _index[i + 2];
				Point3DF pp = (_pos[j3] - _pos[j1]).OuterProduct(_pos[j2] - _pos[j1]).Normalize();
				normals[j1] += pp;
				normals[j2] += pp;
				normals[j3] += pp;
				num[j1]++;
				num[j2]++;
				num[j3]++;
			}
		}
		for (i = 0; i < _poslen; ++i) if (num[i]>0)normals[i] /= (float)num[i];
		delete[]num;
		return normals;
	}
	return nullptr;
}