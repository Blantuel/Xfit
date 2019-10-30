#pragma once
#include "Point.h"
#include "../data/Array.h"

class LinesF {
public:
	Array<PointF> points;

	float Distance() const;
};
