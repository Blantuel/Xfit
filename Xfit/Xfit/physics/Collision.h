#pragma once
#include "../math/Point.h"

class Collision abstract {
public:
	virtual bool HitTest(PointF _point) = 0;
	virtual ~Collision() {}
};