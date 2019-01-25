#pragma once

#include "../stdafx.h"

class Blend {
	friend class Object;
	friend class Object3D;

	const float _constants[4];


public:
	enum class Value {
		SRC_COLOR,
		SRC_ALPHA,
		DST_ALPHA,
		DST_COLOR,
		ONE,
		ZERO,
		CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_SRC_COLOR,
		ONE_MINUS_DST_COLOR,
		ONE_MINUS_SRC_ALPHA,
		ONE_MINUS_DST_ALPHA,
		ONE_MINUS_CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_ALPHA,
	};
	enum class Equation {
		ADD,
		SUBTRACT,
		REVERSE_SUBTRACT,
		MAX,
		MIN
	};

	Blend(bool _enable = false, Value _srcColor = Value::ONE, Value _dstColor = Value::ZERO,bool alphaToCoverage=false, Value _srcAlpha = Value::ONE, Value _dstAlpha = Value::ZERO,
		Equation _colorEquation = Equation::ADD, Equation _alphaEquation = Equation::ADD,float _constantR = 1.f, float _constantG = 1.f, float _constantB = 1.f, float _constantA = 1.f);
};