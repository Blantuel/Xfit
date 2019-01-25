#include "Blend.h"

Blend::Blend(bool _enable, Value _srcColor, Value _dstColor, bool alphaToCoverage, Value _srcAlpha, Value _dstAlpha,Equation _colorEquation ,Equation _alphaEquation, 
	float _constantR, float _constantG, float _constantB , float _constantA) :_constants{ _constantR,  _constantG ,_constantB, _constantA } {

}