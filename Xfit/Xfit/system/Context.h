#pragma once

#include "../stdafx.h"



class Context {
	//HGLRC context;
public:
	Context();
	~Context();
	void Link();
	void Unlink();
};