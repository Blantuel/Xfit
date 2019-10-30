#pragma once

#include "../stdafx.h"

class Blend {
	friend class Object;
	friend class Object3D;
#ifdef __ANDROID__
	static constexpr int OpenGLValue[] = {
		GL_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE,
		GL_ZERO,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_ONE_MINUS_CONSTANT_COLOR,
	};
	static constexpr int OpenGLEquation[] = {
		GL_FUNC_ADD,
		GL_FUNC_SUBTRACT,
		GL_FUNC_REVERSE_SUBTRACT,
		GL_MAX,
		GL_MIN
	}
#elif _WIN32
	static constexpr D3D11_BLEND DirectX11Value[] = {
		D3D11_BLEND_SRC_COLOR,
		D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND_DEST_ALPHA,
		D3D11_BLEND_DEST_COLOR,
		D3D11_BLEND_ONE,
		D3D11_BLEND_ZERO,
		D3D11_BLEND_BLEND_FACTOR,
		D3D11_BLEND_INV_SRC_COLOR,
		D3D11_BLEND_INV_DEST_COLOR,
		D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_INV_DEST_ALPHA,
		D3D11_BLEND_INV_BLEND_FACTOR
	};
	static constexpr D3D11_BLEND_OP DirectX11Equation[] = {
		D3D11_BLEND_OP_ADD,
		D3D11_BLEND_OP_SUBTRACT,
		D3D11_BLEND_OP_REV_SUBTRACT,
		D3D11_BLEND_OP_MAX,
		D3D11_BLEND_OP_MIN
	};
#endif
public:
	enum class Value {
		SrcColor,
		SrcAlpha,
		DestAlpha,
		DestColor,
		One,
		Zero,
		ConstColor,
		OneMinusSrcColor,
		OneMinusDestColor,
		OneMinusSrcAlpha,
		OneMinusDestAlpha,
		OneMinusConstColor,
	};
	enum class Equation {
		Add,
		Sub,
		ReverseSub,
		Max,
		Min
	};

	Blend(Value _srcColor = Value::One, Value _dstColor = Value::Zero, Value _srcAlpha = Value::One, Value _destAlpha = Value::Zero,
		Equation _colorEquation = Equation::Add, Equation _alphaEquation = Equation::Add, float _constR = 1.f, float _constG = 1.f, float _constB = 1.f, float _constA = 1.f);

	~Blend();
private:
#ifdef __ANDROID__
	const Value srcColor;
	const Value destColor;
	const Value srcAlpha;
	const Value destAlpha;
	const Equation colorEquation;
	const Equation alphaEquation;
#endif
	const float consts[4];

#ifdef _WIN32
	union {
		ID3D11BlendState* blendState;
	};
	union {
		ID3D11BlendState1* blendState1;
	};
#elif __ANDROID__
#endif

};