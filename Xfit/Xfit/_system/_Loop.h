#pragma once

#include "../system/System.h"

class Object;
class Object3D;

namespace _System::_Loop {
	inline chrono::nanoseconds deltaTime(0);
	inline chrono::system_clock::time_point time;
	inline bool resized=false, start,exited;


	void Loop();
	void Release();
}