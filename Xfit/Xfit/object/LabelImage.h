#pragma once

#include "Image.h"
#include "ScaleHeader.h"
#include "../text/SizeLabel.h"

class LabelImage :public Image {
public:
	bool isVertical;

	CenterPointPos centerPointPos;

	LabelImage(SizeLabel* _label, PointF _pos = PointF(0.f,0.f), CenterPointPos _centerPointPos=CenterPointPos::Center,  float _rotation = 0.f, bool _isVertical = false);

	PointF basePos;


	virtual void SetPos(PointF _pos);
	virtual void SetX(float _x);
	virtual void SetY(float _y);
	virtual void SetRotation(float _rotation);
	virtual void SetTransform(PointF _pos, float _rotation);

	SizeLabel* GetLabel()const;

	void Size();

	static void MakeLabelImageText(LabelImage* _labelImage, const wchar_t* _text, unsigned _color = 0, unsigned _fontSize = 0);

	static void LABELIMAGE_SIZE(LabelImage* _labelImage) { if (_labelImage->GetLabel()->IsChangeSize(WindowRatio())) { _labelImage->GetLabel()->SizePrepareDraw(WindowRatio()); _labelImage->Size(); } }
	static void LABELIMAGE_SIZE2(LabelImage* _labelImage, float _scale) { if (_labelImage->GetLabel()->IsChangeSize(WindowRatio() * _scale)) { _labelImage->GetLabel()->SizePrepareDraw(WindowRatio() * _scale); _labelImage->Size(); } }
};




