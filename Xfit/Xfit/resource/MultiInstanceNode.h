#pragma once

#include "../math/Matrix.h"

#pragma pack(1)
struct MultiInstanceNode {
	Matrix mat;
	unsigned frameIndex;
	unsigned reversed[3];
};