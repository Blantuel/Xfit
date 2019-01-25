#pragma once

#include "Point.h"

template <typename T> class Array;

class HexagonF {
public:
	Array<PointF>* points;

	const PointF GetCenterPoint()const;
	float GetArea()const;
};