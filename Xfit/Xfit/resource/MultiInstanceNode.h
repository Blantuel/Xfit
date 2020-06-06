#pragma once

#include "../math/Matrix.h"

#pragma pack(push,1)
struct MultiInstanceNode {
	Matrix mat;
	Matrix colorMat;
	int imgIndex;
};
#pragma pack(pop)