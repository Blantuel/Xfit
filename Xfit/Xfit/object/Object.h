#pragma once

#include "../math/Matrix.h"
#include "../physics/Collision.h"

class Blend;
class Viewport;

class Object abstract {
public:
	Matrix mat;
	Blend * blend;

	virtual ~Object() {}
	virtual void Draw();
};