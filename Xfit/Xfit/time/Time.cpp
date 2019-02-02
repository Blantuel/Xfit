#pragma once

#include "Time.h"

void Time::Init() {
#ifdef _WIN32
	QueryPerformanceFrequency(&frequency);
#endif
}
double Time::GetTime() {
#ifdef _WIN32
	QueryPerformanceCounter(&time);

	return (double)time.QuadPart / (double)frequency.QuadPart;
#endif
}
