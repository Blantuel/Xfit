#pragma once

#include "../object/ScaleHeader.h"
#include "Button.h"
#include "../object/CenterPointPos.h"

class SizeLabel;

class SizeButton : public Button {
public:
	bool isVertical;

	CenterPointPos centerPointPos;

	//생성할때 반드시 프레임 넣기
	SizeButton(const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame, 
		HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, CenterPointPos _centerPointPos = CenterPointPos::Center, bool _isVertical = false);

	PointF basePos;
	PointF baseScale;

	virtual void Size();

	virtual void SetPos(PointF _pos);
	virtual void SetX(float _x);
	virtual void SetY(float _y);
};



SizeButton* MakeSizeButton(Frame* _upFrame, Frame* _overFrame, Frame* _downFrame, Vertex* _vertex, PointF _pos, bool _noUseMipmap = false, PointF _scale = PointF(1.f, 1.f));
