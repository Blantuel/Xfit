#pragma once

#include "../math/Rect.h"

namespace Screen {
	PointF Pos(const PointF& _pos);
	float X(float _x);
	float Y(float _y);
	PointF Scale(const PointF& _scale);
	void VertexPos(PointF* _outVertexPos,const RectF& _rect);
}