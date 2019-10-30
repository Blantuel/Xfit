#pragma once

#include "Vertex.h"

class ShapeVertex : public Vertex{
	PointF centerPos;
public:
	PointF GetCenterPos()const;
	virtual void Build(bool _editable = false);
	virtual void Edit();

	virtual unsigned GetNum()const;
};