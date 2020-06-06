#pragma once

#include "../stdafx.h"
#include "Math.h"


class Matrix {
public:
	union {
#ifdef SSE4
		struct {
			__m128 m1, m2, m3, m4;
		};
#else
#endif
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float ee[4][4];
		float e[16];
	};
	Matrix();
	Matrix(std::initializer_list<float> _initializer_list);
	Matrix& Transpose();
	void Identity();

	static Matrix GetIdentity();
	static Matrix GetMatrix2D(float _dx, float _dy, float _sx, float _sy, float _r);
	static Matrix GetMatrix2DTranspose(float _dx, float _dy, float _sx, float _sy, float _r);
	static Matrix GetMatrix3D(float _dx, float _dy, float _dz, float _sx, float _sy, float _sz, float _rx, float _ry, float _rz);
	static Matrix GetMatrix3DTranspose(float _dx, float _dy, float _dz, float _sx, float _sy, float _sz, float _rx, float _ry, float _rz);
	static Matrix GetMove(float _dx, float _dy, float _dz = 0.f);
	static Matrix GetMoveTranspose(float _dx, float _dy, float _dz = 0.f);
	static Matrix GetScale(float _sx, float _sy, float _sz = 1.f);
	static Matrix GetRotationXYZ(float _rx, float _ry, float _rz);
	static Matrix GetRotationXYZTranspose(float _rx, float _ry, float _rz);
	static Matrix GetRotationX(float _rx);
	static Matrix GetRotationY(float _ry);
	static Matrix GetRotationZ(float _rz);
	static Matrix GetRotationXTranspose(float _rx);
	static Matrix GetRotationYTranspose(float _ry);
	static Matrix GetRotationZTranspose(float _rz);
	static Matrix GetDirection2D(float _x, float _y);

	Matrix Multiply(const Matrix& _mat)const;
	Matrix operator*(const Matrix& m)const;
	Matrix& operator*=(const Matrix& m);

	Matrix& operator*=(float f);
	Matrix operator*(float f)const;


	Matrix& operator-=(const Matrix& m);
	Matrix& operator+=(const Matrix& m);
	Matrix operator-(const Matrix& m)const;
	Matrix operator+(const Matrix& m)const;

	Matrix& operator/=(const Matrix& m);
	Matrix operator/(const Matrix& m)const;

	Matrix Inverse()const;
	float GetDeterminant()const;
};
