#pragma once

#include "../stdafx.h"

class Vertex;

class RTAnimateFrame {
public:
	void* data;
	unsigned size;
	Vertex* vertex;
	Vertex* uv;

	RTAnimateFrame();
};