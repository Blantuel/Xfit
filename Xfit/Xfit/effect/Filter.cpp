
#include "Filter.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

void Filter::Init()
{	

}

void Filter::Release()
{
	
}

void BlurFilter::Apply()
{
	
}

void BlurFilter::SetSamplesBuffer(const float * ar, unsigned _radius)
{

}

void CartoonFilter::Apply(const Point3Dw & _minColor, const Point3Dw & _stepColor, const Point3Dw & _maxColor)
{

}

