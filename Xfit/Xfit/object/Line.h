#pragma once

#include "SizeMatrixObject.h"
#include "../system/Error.h"
#include "../math/Point3Dw.h"

class Vertex;


class Line : public SizeMatrixObject {
	
public:
	float lineWidth;
	Vertex* vertex;
	Point3DwF lineColor;

	Line(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Vertex* _vertex,
		Point3DwF _lineColor = Point3DwF(0.f, 0.f, 0.f, 1.f), float _lineWidth = 1.f);

	virtual void Draw();
};

