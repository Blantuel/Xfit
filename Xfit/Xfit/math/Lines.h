#pragma once
#include "Point.h"

template <typename T> class Array;

class LinesF {
public:
	Array<PointF>* points;

	float Distance() const;
};
