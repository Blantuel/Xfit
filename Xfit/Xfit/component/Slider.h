#pragma once

#include "../resource/Vertex.h"
#include "../object/ScaleHeader.h"
#include "../object/ScaleImage.h"

class Slider {
	bool sliding;

	float value;

	PointF barBasePos;
	ScaleImage* bar;
	ScaleImage* stick;
public:
	void (*controlFinish)(Slider* _target);
	void (*controlling)(Slider* _target);

	Slider(ScaleImage* _bar, ScaleImage* _stick, PointF _pos, float _value);

	float GetValue()const;

	void SetValue(float _value);//0~1

	PointF GetPos()const;
	void SetPos(PointF _pos);

	float GetWidth()const;

	bool Update();
	void Draw();
	void Size();

	ScaleImage* GetBar()const;
	ScaleImage* GetStick()const;
};

