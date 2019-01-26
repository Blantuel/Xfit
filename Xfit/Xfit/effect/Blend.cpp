#include "Blend.h"

Blend::Blend(Value _srcColor, Value _dstColor,  Value _srcAlpha, Value _dstAlpha,Equation _colorEquation ,Equation _alphaEquation, 
	float _constantR, float _constantG, float _constantB , float _constantA) :srcColor(_srcColor),dstColor(_dstColor), srcAlpha(_srcAlpha), 
	dstAlpha(_dstAlpha), colorEquation(_colorEquation), alphaEquation(_alphaEquation),constants{ _constantR,  _constantG ,_constantB, _constantA } {
}