#pragma once

#include "SizeMatrixObject.h"
#include "../system/Error.h"
#include "../math/Point3Dw.h"

class ShapeVertex;

class ShapeError : public Error {
public:
	enum Code {
		NullVertex,
		NotVertexBuild
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	ShapeError(Code _code):code(_code) {}
};

class Shape : public SizeMatrixObject {
public:
	Point3DwF fillColor;
	Point3DwF lineColor;
    ShapeVertex* vertex;
    float lineWidth;

	Shape(PointF _pos,PointF _scale,float _rotation,Blend* _blend, ShapeVertex* _vertex, Point3DwF _fillColor = Point3DwF(1.f,1.f,1.f,1.f),
		Point3DwF _lineColor = Point3DwF(0.f, 0.f, 0.f, 1.f),float _lineWidth = 1.f);

	virtual void Draw();
};