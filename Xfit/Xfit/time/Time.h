#pragma once

#include "../stdafx.h"

class Time {
	static LARGE_INTEGER frequency;
	static LARGE_INTEGER time;
public:
	static void Init();
	static double GetTime();
};