#include "Time.h"


void Time::Init() {
}
double Time::GetTime() {
	std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::duration<double>>(time.time_since_epoch()).count();
}
