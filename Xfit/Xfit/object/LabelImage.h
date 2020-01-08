#pragma once

#include "Image.h"
#include "ScaleHeader.h"

class SizeLabel;

class LabelImage :public Image {
public:
	bool isVertical;

	CenterPointPos centerPointPos;

	LabelImage(PosType _posType, SizeLabel* _label, PointF _pos = PointF(0.f,0.f), CenterPointPos _centerPointPos=CenterPointPos::Center,  float _rotation = 0.f, bool _isVertical = false);

	PointF basePos;

	PosType posType;

	SizeLabel* GetLabel()const;

	void Size(float _scaleRatio = 1.f);
	virtual void PrepareDraw(float _scaleRatio = 1.f);
};

