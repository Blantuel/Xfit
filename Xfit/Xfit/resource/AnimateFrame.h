#pragma once

#include "../stdafx.h"

class Frame;
class Vertex;
class Index;

class AnimateFrame {
public:
	Frame* frame;
	Vertex* vertex;
	Vertex* uv;
	Index* index;
	AnimateFrame();
	AnimateFrame(Frame* _frame, Vertex* _vertex, Vertex* _uv);
};