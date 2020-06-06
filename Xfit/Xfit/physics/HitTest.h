#pragma once
#include "../math/Point.h"

class HitTest {
public:
	virtual bool Test(PointF _point) = 0;
	virtual void Size() {}
	virtual ~HitTest() {}
};