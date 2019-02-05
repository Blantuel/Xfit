#pragma once

#include "../stdafx.h"

namespace System {
#ifdef _WIN32
	void Create(HINSTANCE _hInstance);
#endif
}

class Time {
#ifdef _WIN32
	friend void System::Create(HINSTANCE _hInstance);
	static inline LARGE_INTEGER frequency;
	static inline thread_local LARGE_INTEGER time;
#elif __ANDROID__
	static inline thread_local timespec tsp;
#endif
	static void Init();
public:
	static double GetTime();
};