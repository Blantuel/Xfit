#pragma once

#include "../system/System.h"

class Object;
class Object3D;

namespace _System::_Loop {
	inline bool  start=false,exited=false;

	inline std::chrono::high_resolution_clock::time_point time;
	inline float deltaTime = 0.f;
	inline float maxFrame = 0.f;
	void Loop();
}