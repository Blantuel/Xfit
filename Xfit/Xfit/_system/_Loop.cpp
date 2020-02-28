#include "_Loop.h"

#include "_Windows.h"
#include "_Android.h"

#include "_DirectX11.h"

#include "../data/Array.h"
#include "../time/Time.h"

namespace _System::_Loop {
	void Loop() {
		if (!start) {
			time = std::chrono::high_resolution_clock::now();
			start = true;
		} else {
			const auto time2 = time;
			time = std::chrono::high_resolution_clock::now();
			deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(time - time2).count();
			if (maxFrame > 0.0) {
				while ((1.f / maxFrame) > deltaTime) {
					const auto time2 = time;
					time = std::chrono::high_resolution_clock::now();
					deltaTime += std::chrono::duration_cast<std::chrono::duration<float>>(time - time2).count();
				}
			}
		}
		System::updateFuncs();
	}
}
