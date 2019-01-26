#pragma once

#include "../stdafx.h"

class Blend {
	friend class Object;
	friend class Object3D;
public:
#ifdef OPENGL
	enum class Value {
		SRC_COLOR = GL_SRC_COLOR,
		SRC_ALPHA = GL_SRC_ALPHA,
		DST_ALPHA = GL_DST_ALPHA,
		DST_COLOR = GL_DST_COLOR,
		ONE = GL_ONE,
		ZERO = GL_ZERO,
		CONSTANT_COLOR = GL_CONSTANT_COLOR,
		CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
		ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
		ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
		ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
		ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
		ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
	};
	enum class Equation {
		ADD=GL_FUNC_ADD,
		SUBTRACT=GL_FUNC_SUBTRACT,
		REVERSE_SUBTRACT=GL_FUNC_REVERSE_SUBTRACT,
		MAX=GL_MAX,
		MIN=GL_MIN
	};
#elif VULKAN
#endif

	Blend(Value _srcColor = Value::ONE, Value _dstColor = Value::ZERO, Value _srcAlpha = Value::ONE, Value _dstAlpha = Value::ZERO,
		Equation _colorEquation = Equation::ADD, Equation _alphaEquation = Equation::ADD,float _constantR = 1.f, float _constantG = 1.f, float _constantB = 1.f, float _constantA = 1.f);
private:
	Value srcColor;
	Value dstColor;
	Value srcAlpha;
	Value dstAlpha;
	Equation colorEquation;
	Equation alphaEquation;
	const float constants[4];
};