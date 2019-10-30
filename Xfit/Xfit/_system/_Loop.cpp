#include "_Loop.h"

#include "_Windows.h"
#include "_Android.h"

#include "_DirectX11.h"

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
			deltaTime = (float)(time - time2);
			if (maxFrame > 0.0) {
				while ((1.f / maxFrame) > deltaTime) {
					const auto time2 = time;
					time = Time::GetTime();
					deltaTime += (float)(time - time2);
				}
			}
		}
		System::updateFuncs(System::updateData);
	}
}
