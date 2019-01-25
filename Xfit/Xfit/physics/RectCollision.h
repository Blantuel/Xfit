#pragma once
#include "Collision.h"
#include "../math/Rect.h"

class RectCollision : public Collision {
public:
	RectF rect;
	bool HitTest(PointF _point);
};