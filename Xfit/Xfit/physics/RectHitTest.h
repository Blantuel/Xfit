#pragma once
#include "HitTest.h"
#include "../math/Rect.h"

class RectHitTest : public HitTest {
public:
	Rect rect;
	bool Test(Point _point);
};