#pragma once

#include "../resource/Vertex.h"
#include "../object/ScaleHeader.h"
#include "../object/ScaleImage.h"

class Slider {
	bool sliding;

	float value;

	PointF barBasePos;
	ScaleImage bar;
	ScaleImage stick;
public:
	PosType posType;
	void (*controlFinish)(Slider* _target);
	void (*controlling)(Slider* _target);

	Slider(PosType _posType, Frame* _barFrame, Frame* _stickFrame, PointF _pos, float _value);

	float GetValue()const;

	void SetValue(float _value);//0~1

	PointF GetPos()const;
	void SetPos(PointF _pos);

	float GetWidth()const;

	bool Update();
	void Draw();
	void Size();
};

