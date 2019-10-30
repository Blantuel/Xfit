#pragma once

#include "../system/System.h"

class Object;
class Object3D;

namespace _System::_Loop {
	inline bool  start=false,exited=false;

	inline double time;
	inline float deltaTime = 0.f;
	inline float maxFrame = 0.f;
	void Loop();
}