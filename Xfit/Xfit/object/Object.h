#pragma once

#include "../stdafx.h"

class Blend;
class Viewport;

class Object {
public:
	Blend * blend;

	Object(Blend* _blend);
	Object();
	bool visible;
	virtual ~Object() {}
	virtual void Draw();

};