#include "_Loop.h"

#include "_Windows.h"
#include "_Android.h"

#include "_OpenGL.h"
#include "_Vulkan.h"

#include "../data/Array.h"
#include "../time/Time.h"

namespace _System::_Loop {
	void Loop() {
		if (!start) {
			time = Time::GetTime();
			start = true;
		} else {
			const auto time2 = time;
			time = Time::GetTime();
			deltaTime = time - time2;
		}
		System::updateFuncs();

	}
}
