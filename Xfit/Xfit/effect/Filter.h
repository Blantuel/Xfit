#pragma once

#include "../math/Point3Dw.h"


class Filter {
protected:
	static void Init();
	static void Release();
public:

	virtual ~Filter(){}
};

class BlurFilter : public Filter {
protected:

	unsigned radius;
public:

	void Apply();
	unsigned GetSamplesSize()const {return radius;}
	void SetSamplesBuffer(const float* ar, unsigned _radius);

	~BlurFilter() {

	}
};

class BlurFilterPreset {
public:
	static const float* Gaussian(int _radius) {
		int a = _radius * 2 + 1;
		float* f = new float[a];
		const float div = (float)a / 7.f;

		for (int i = -_radius;i <= _radius;++i) {
			const float fa = ((float)i / (float)a)*6.f;
			f[i+ _radius] = 0.3989422804014f*powf(2.718281828459f, -(fa*fa) /2.f)/div;
		}

		return f;
	}
	static const float* Box(unsigned _radius) {
		const unsigned a = _radius * 2 + 1;
		const float ff = (float)a;
		float* f = new float[a];
		for (unsigned i = 0;i < a;++i) f[i] = 1.f / ff;
		return f;
	}
};

class ColorMatrixPreset {
public:
	static const Matrix ColorMultiplyAndPlus(float mr = 1, float mg = 1, float mb = 1, float ma = 1, float ar = 0, float ag =0, float ab = 0) {
		return Matrix({
		mr,0.f,0.f,ar,
		0.f,mg,0.f,ag,
		0.f,0.f,mb,ab,
		0.f,0.f,0.f,ma });
	}
	static const Matrix Brightness(float amount) {
		return Matrix({
			1.f,0.f,0.f,amount,
			0.f,1.f,0.f,amount,
			0.f,0.f,1.f,amount,
			0.f,0.f,0.f,1.f });
	}
	static const Matrix GrayScale() {
		return Matrix({
			0.2989f,0.587f,0.114f,0.f,
			0.2989f,0.587f,0.114f,0.f,
			0.2989f,0.587f,0.114f,0.f,
			0.f,0.f,0.f,1.f });
	}
	static const Matrix Contrast(float amount) {
		const float t = (1.f - amount) / 2.f;
		return Matrix({
			amount,0.f,0.f,t,
			0.f,amount,0.f,t,
			0.f,0.f,amount,t,
			0.f,0.f,0.f,1.f });
	}
	static const Matrix Saturation(float amount) {
		float r = 0.3086f, g = 0.6094f, b = 0.0820f;
		r = (1 - amount)*r;
		g = (1 - amount)*g;
		b = (1 - amount)*b;
		return Matrix({
			r + amount,g,b,0.f,
			r,g + amount,b,0.f,
			r,g,b + amount,0.f,
			0.f,0.f,0.f,1 });
	}
	static const Matrix Invent() {
		return Matrix({
			-1.f,0.f,0.f,1.f,
			0.f,-1.f,0.f,1.f,
			0.f,0.f,-1.f,1.f,
			0.f,0.f,0.f,1.f });
	}
};

class CartoonFilter : public Filter {
public:
	CartoonFilter() {}
	void Apply(const Point3Dw& _minColor, const Point3Dw& _stepColor, const  Point3Dw& _maxColor);
};
