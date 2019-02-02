#include "_Loop.h"

#include "_Windows.h"
#include "_Android.h"

#include "_OpenGL.h"
#include "_Vulkan.h"

#include "../data/Array.h"

namespace _System::_Loop {
	void Loop() {
		if (!start) {
			//time = chrono::system_clock::now();
			start = true;
		} else {
			//const auto time2 = time;
			//time = chrono::system_clock::now();
			//deltaTime = time - time2;
		}
		System::updateFuncs();

	}
}
