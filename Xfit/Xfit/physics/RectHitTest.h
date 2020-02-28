#pragma once
#include "HitTest.h"
#include "../math/Rect.h"

class RectHitTest : public HitTest {
public:
	RectF rect;

	bool Test(PointF _point);
};