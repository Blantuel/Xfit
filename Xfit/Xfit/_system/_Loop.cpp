#include "_Loop.h"

#ifdef _WIN32
#include "_Windows.h"
#elif __ANDROID__
#include "_Android.h"
#endif

#ifdef OPENGL
#include "_OpenGL.h"
#elif VULKAN
#include "_Vulkan.h"
#endif

#include "../data/Array.h"

namespace _System::_Loop {
	void Loop() {
		start = false;
		while (true) {
			if (!start) {
				time = chrono::system_clock::now();
				start = true;
			} else {
				const auto time2 = time;
				time = chrono::system_clock::now();
				deltaTime = time - time2;
			}

			if (!_Windows::Loop())break;
			System::updateFuncs();
		}
		System::destroyFunc();
		exited = true;
	}
}
