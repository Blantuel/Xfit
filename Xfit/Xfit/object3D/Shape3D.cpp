#pragma once


#include "shape3D.h"
#include "../resource/Vertex.h"



#ifdef _WIN32
bool Shape3D::Draw(unsigned index, unsigned viewIndex)
{
	return true;
}
#else
#endif