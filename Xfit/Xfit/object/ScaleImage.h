#pragma once

#include "Image.h"
#include "ScaleHeader.h"

class Frame;

class ScaleImage :public Image {
public:
	bool isVertical;

	CenterPointPos centerPointPos;

	ScaleImage(PosType _posType, PointF _pos, PointF _scale, float _rotation, CenterPointPos _centerPointPos, bool _isVertical,
		Blend* _blend, Sampler* _sampler, Frame* _frame, Vertex* _uv, Index* _index);

	PointF basePos;
	PointF baseScale;

	PosType posType;

	void Size(float _scaleRatio = 1.f);

	virtual void SetPos(PointF _pos);
	virtual void SetX(float _x);
	virtual void SetY(float _y);
	virtual void SetScale(PointF _scale);
	virtual void SetScaleX(float _scaleX);
	virtual void SetScaleY(float _scaleY);
	//virtual void SetRotation(float _rotation);
	virtual void SetTransform(PointF _pos, PointF _scale, float _rotation);
};

