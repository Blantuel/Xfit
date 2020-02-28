#pragma once

#include "Image.h"
#include "ScaleHeader.h"

class SizeTextBox;

class TextBoxImage :public Image {
public:
	bool isVertical;

	CenterPointPos centerPointPos;

	TextBoxImage(PosType _posType, SizeTextBox* _sizeTextBox, PointF _pos = PointF(0.f, 0.f), CenterPointPos _centerPointPos = CenterPointPos::Center, float _rotation = 0.f, bool _isVertical = false, float _scale = 1.f);

	PointF basePos;
	float baseScale;

	PosType posType;

	SizeTextBox* GetTextBox()const;

	virtual void SetPos(PointF _pos);
	virtual void SetX(float _x);
	virtual void SetY(float _y);
	virtual void SetScale(float _scale);
	//virtual void SetRotation(float _rotation);
	virtual void SetTransform(PointF _pos, float _scale, float _rotation);

	void Size(float _scaleRatio = 1.f);
	virtual void PrepareDraw(float _scaleRatio = 1.f);
};

