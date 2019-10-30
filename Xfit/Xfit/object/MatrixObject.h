#pragma once

#include "Object.h"
#include "../math/Point.h"

class MatrixObject : public Object {
public:
	PointF pos, scale;
	float rotation;
	Matrix mat;
	MatrixObject(PointF _pos,PointF _scale,float _rotation,Blend* _blend);
	MatrixObject();
	virtual void UpdateMatrix();

	virtual void SetPos(PointF _pos);
	virtual void SetX(float _x);
	virtual void SetY(float _y);
	virtual void SetScale(PointF _scale);
	virtual void SetScaleX(float _scaleX);
	virtual void SetScaleY(float _scaleY);
	virtual void SetRotation(float _rotation);
	virtual void SetTransform(PointF _pos, PointF _scale, float _rotation);
};