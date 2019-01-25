#pragma once

#include "../stdafx.h"

class Error : public exception {
public:
	int code;
	Error(int _code) :code(_code) {}
};
