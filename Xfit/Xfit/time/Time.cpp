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
#elif __ANDROID__
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsp);
	return tsp.tv_nsec/1000000000.0+(double)tsp.tv_sec;
#endif
}
