#include "Time.h"

LARGE_INTEGER Time::frequency;
LARGE_INTEGER Time::time;

void Time::Init() {
	QueryPerformanceFrequency(&frequency);
}
double Time::GetTime() {
	QueryPerformanceCounter(&time);

	return (double)time.QuadPart / (double)frequency.QuadPart;
}
