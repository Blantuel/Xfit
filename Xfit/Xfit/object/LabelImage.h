#pragma once

#include "Image.h"
#include "ScaleHeader.h"

class SizeLabel;

class LabelImage :public Image {
public:
	bool isVertical;

	CenterPointPos centerPointPos;

	PosType posType;

	LabelImage(PosType _posType, SizeLabel* _label, PointF _pos = PointF(0.f,0.f), CenterPointPos _centerPointPos=CenterPointPos::Center,  float _rotation = 0.f, bool _isVertical = false, float _scale = 1.f);

	PointF basePos;
	float baseScale;


	virtual void SetPos(PointF _pos);
	virtual void SetX(float _x);
	virtual void SetY(float _y);
	virtual void SetScale(float _scale);
	//virtual void SetRotation(float _rotation);
	virtual void SetTransform(PointF _pos, float _scale, float _rotation);

	SizeLabel* GetLabel()const;

	void Size();
};

