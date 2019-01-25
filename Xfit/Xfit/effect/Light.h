#pragma once

#include "../math/Point3D.h"
#include "../math/Point3Dw.h"


struct Light {
	enum class Type {
		Direction,
		Point,
		Spot
	};
	Point3DF point_spot_pos;
	Type type;
	Point3DF direction_spot_direction;
	float spot_range;
	Point3DwF color;
};
