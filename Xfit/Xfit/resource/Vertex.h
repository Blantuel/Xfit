#pragma once

#include "../stdafx.h"
#include "../system/WriteFlag.h"

class PointF;
class Point3DF;
class Point3DwF;


class Vertices {
	union {
		struct {
			GLuint _posBuffer;
			GLuint _normalBuffer;
			GLuint _colorBuffer;
			GLuint _uvBuffer;
		}openGL;
	};
	void _WriteBufferOpenGL(GLuint* _buffer, const void * _data, unsigned nodeSize);
	unsigned vertexlen,vertexslen;
	unsigned indexlen, indexslen;
	
	WriteFlag flag;
	
public:
	unsigned vertexblen;
	unsigned indexblen;

	Vertices(WriteFlag _flag = WriteFlag::WriteGPU, unsigned _vertexblen=0, unsigned _indexblen=0);

	~Vertices();

	void WriteVertex(unsigned _vertexlen, const Point3DF * _pos, const PointF* _uv = nullptr, const Point3DF* _normal = nullptr, const Point3DwF* _color = nullptr);
	void WriteIndex(unsigned _indexlen, const unsigned* _index);
	unsigned GetVertexlength() const;
	unsigned GetIndexlength() const;
	WriteFlag GetWriteFlag() const;


	static Point3DF* BuildNormal(const Point3DF* _pos, const unsigned* _index,unsigned _poslen, unsigned _indexlen,bool _cw=true);
};