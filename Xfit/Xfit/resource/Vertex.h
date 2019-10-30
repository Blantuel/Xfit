#pragma once

#include "../math/Point.h"
#include "../data/Array.h"
#include "CenterPointPos.h"

class VertexError : public Error {
public:
	enum class Code {
		Null,
		NoData,
		LessSize,
		InvalidImageSize,
		NotEmptyData,
		AlreadyBuild,
		NotBuild
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	VertexError(Code _code):code(_code) {}
};
class Vertex {
	friend class ImageBase;
	friend class ImageInstance;
	friend class ImageMultiInstance;
	friend class RTAnimateObjectBase;
	friend class Shape;
	friend class Line;
	friend class ShapeInstance;
	friend class LineInstance;

#ifdef _WIN32
	union {
		ID3D11Buffer* vertex;
	};
#elif __ANDROID__
	GLuint vertex;
#endif

protected:
	unsigned num;

	void _Build(PointF* _vertices, unsigned _num, bool _editable);
	void _Edit(PointF* _vertices, unsigned _num);
public:
	Array<PointF> vertices;

	void MakeImageVertex2D(PointF _size, PointF _pos, CenterPointPos _centerPointPos = CenterPointPos::Center);
	void MakeImageVertex2D(PointF _size, CenterPointPos _centerPointPos = CenterPointPos::Center);
	void MakePtImageVertex2D(PointU _size, CenterPointPos _centerPointPos = CenterPointPos::Center);

	Vertex();
	~Vertex();
	virtual void Build(bool _editable = false);
	virtual void Edit();
	bool IsBuild()const;
	void Delete();

	virtual unsigned GetNum()const;
};