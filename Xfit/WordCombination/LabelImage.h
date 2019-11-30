#pragma once

#include <object/Image.h>
#include <resource/CenterPointPos.h>
#include "main.h"

class SizeLabel;

class LabelImage :public Image {
public:
	bool isVertical;

	CenterPointPos centerPointPos;

	LabelImage(PosType _posType, SizeLabel* _label, PointF _pos = PointF(0.f,0.f), CenterPointPos _centerPointPos=CenterPointPos::Center,  float _rotation = 0.f, bool _isVertical = false);

	PointF basePos;

	PosType posType;

	SizeLabel* GetLabel()const;

	void Size();
	virtual void PrepareDraw();

	void SetPos(PointF _pos);
};

