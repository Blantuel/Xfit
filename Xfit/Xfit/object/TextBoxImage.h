#pragma once

#include "Image.h"
#include "ScaleHeader.h"
#include "../text/SizeTextBox.h"

class TextBoxImage :public Image {
public:
	bool isVertical;

	CenterPointPos centerPointPos;

	TextBoxImage(SizeTextBox* _sizeTextBox, PointF _pos = PointF(0.f, 0.f), CenterPointPos _centerPointPos = CenterPointPos::Center, float _rotation = 0.f, bool _isVertical = false);

	PointF basePos;

	SizeTextBox* GetTextBox()const;

	virtual void SetPos(PointF _pos);
	virtual void SetX(float _x);
	virtual void SetY(float _y);
	virtual void SetRotation(float _rotation);
	virtual void SetTransform(PointF _pos, float _rotation);

	void Size();


	static void TEXTBOXIMAGE_SIZE(TextBoxImage* _textBoxImage) { if (_textBoxImage->GetTextBox()->IsChangeSize(WindowRatio())) { _textBoxImage->GetTextBox()->SizePrepareDraw(WindowRatio()); _textBoxImage->Size(); } }
	static void TEXTBOXIMAGE_SIZE2(TextBoxImage* _textBoxImage, float _scale) { if (_textBoxImage->GetTextBox()->IsChangeSize(WindowRatio() * _scale)) { _textBoxImage->GetTextBox()->SizePrepareDraw(WindowRatio() * _scale); _textBoxImage->Size(); } }
};
