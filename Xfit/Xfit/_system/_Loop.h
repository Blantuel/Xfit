#pragma once

#include "../system/System.h"

class Object;
class Object3D;

namespace _System::_Loop {
	inline bool  start=false,exited=false;

	inline double time,deltaTime=-1;
	void Loop();
}