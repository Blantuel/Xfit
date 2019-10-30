#pragma once
#include "../math/Point.h"

class HitTest {
public:
	virtual bool Test(Point _point) = 0;
};