#pragma once

#include "Point.h"
#include "../data/Array.h"

class HexagonF {
public:
	Array<PointF> points;

	const PointF GetCenterPoint()const;
	float GetArea()const;
};