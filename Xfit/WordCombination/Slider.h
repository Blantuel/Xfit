#pragma once

#include <object/Shape.h>
#include <object/Line.h>
#include <resource/ShapeVertex.h>
#include "main.h"

class Slider {
	bool sliding;

	float width,value;
	PointF barVertexData[2];
	PointF stickVertexData[4];

	PointF barBasePos;
	Line bar;
	Shape stick;
	ShapeVertex barVertex;
	ShapeVertex stickVertex;
public:
	PosType posType;
	void (*controlFinish)(Slider* _target);
	void (*controlling)(Slider* _target);

	Slider(PosType _posType, PointF _pos, float _value, float _width = 200.f);

	float GetValue()const;

	void SetValue(float _value);//0~1

	PointF GetPos()const;
	void SetPos(PointF _pos);

	float GetWidth()const;

	bool Update();
	void Draw();
	void Size();
};

