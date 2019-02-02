#include "Matrix.h"
#include "../data/Memory.h"

Matrix::Matrix() {}
Matrix::Matrix(initializer_list<float> _initializer_list) {
	Memory::Copy(e, 16, _initializer_list.begin(), 16);
}
Matrix& Matrix::Transpose() {
	swap(_14, _41);
	swap(_13, _31);
	swap(_12, _21);
	swap(_24, _42);
	swap(_34, _43);
	swap(_23, _32);
	return *this;
}
Matrix Matrix::GetMatrix2D(float _dx, float _dy, float _sx, float _sy, float _r) {
	const float c = cosf(_r), s = sinf(_r);

	const Matrix mat = { c*_sx, -s*_sy, 0.f, 0.f,
		s * _sx, c*_sy, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		_dx,_dy, 0.f, 1.f };
	return mat;
}
Matrix Matrix::GetMatrix2DTranspose(float _dx, float _dy, float _sx, float _sy, float _r) {
	const float c = cosf(_r), s = sinf(_r);

	const Matrix mat={ c*_sx, s * _sx, 0.f, _dx,
		-s*_sy, c*_sy, 0.f, _dy,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };
	return mat;
}
Matrix Matrix::GetMatrix3D(float _dx, float _dy, float _dz, float _sx, float _sy, float _sz, float _rx, float _ry, float _rz) {
	const float cx = cosf(_rx), ssx = sinf(_rx);
	const float cy = cosf(_ry), ssy = sinf(_ry);
	const float cz = cosf(_rz), ssz = sinf(_rz);
	const float f0 = _sx * cy*cz,
		f1 = _sy * (ssx*ssy*cz - cx * ssz),
		f2 = _sz * (cx*ssy*cz + ssx * ssz),
		f3 = _sx * cy*ssz,
		f4 = _sy * (ssx*ssy*ssz + cx * cz),
		f5 = _sz * (cx*ssy*ssz - ssx * cz),
		f6 = -_sx * ssy,
		f7 = _sy * ssx*cy,
		f8 = _sz * cx*cy;
	
	const Matrix mat = { f0,f3,f6,0.f,
		f1,f4,f7,0.f,
		f2,f5,f8,0.f,
		_dx,_dy,_dz,1.f };
	return mat;
}
Matrix Matrix::GetMatrix3DTranspose(float _dx, float _dy, float _dz, float _sx, float _sy, float _sz, float _rx, float _ry, float _rz) {
	const float cx = cosf(_rx),ssx = sinf(_rx);
	const float cy = cosf(_ry), ssy = sinf(_ry);
	const float cz = cosf(_rz),ssz = sinf(_rz);
	const float f0 = _sx * cy*cz,
		f1 = _sy * (ssx*ssy*cz - cx * ssz),
		f2 = _sz * (cx*ssy*cz + ssx * ssz),
		f3 = _sx * cy*ssz,
		f4 = _sy * (ssx*ssy*ssz + cx * cz),
		f5 = _sz * (cx*ssy*ssz - ssx * cz),
		f6 = -_sx * ssy,
		f7 = _sy * ssx*cy,
		f8 = _sz * cx*cy;

	const Matrix mat = { f0,f1,f2, _dx,
		f3,f4,f5, _dy,
		f6,f7,f8,_dz,
		0.f,0.f,0.f,1.f };
	return mat;
}
Matrix Matrix::GetMove(float _dx, float _dy, float _dz/* = 0.f*/) {
	const Matrix mat = {
	1.f,0.f,0.f,0.f,
	0.f,1.f,0.f,0.f,
	0.f,0.f,1.f,0.f,
	_dx,_dy,_dz,1.f };
	return mat;
}
Matrix Matrix::GetMoveTranspose(float _dx, float _dy, float _dz /*= 0.f*/) {
	const Matrix mat = {
	1.f,0.f,0.f,_dx,
	0.f,1.f,0.f,_dy,
	0.f,0.f,1.f,_dz,
	0.f,0.f,0.f,1.f };
	return mat;
}
Matrix Matrix::GetScale(float _sx, float _sy, float _sz/* = 1.f*/) {
	const Matrix mat = {
	_sx,0.f,0.f,0.f,
	0.f,_sy,0.f,0.f,
	0.f,0.f,_sz,0.f,
	0.f,0.f,0.f,1.f };
	return mat;
}
Matrix Matrix::GetRotationXYZ(float _rx, float _ry, float _rz) {
	const float cx = cosf(_rx),sx = sinf(_rx);
	const float cy = cosf(_ry),sy = sinf(_ry);
	const float cz = cosf(_rz),sz = sinf(_rz);
	const float f0 = cy * cz,
		f1 = (sx*sy*cz - cx * sz),
		f2 = (cx*sy*cz + sx * sz),
		f3 = cy * sz,
		f4 = (sx*sy*sz + cx * cz),
		f5 = (cx*sy*sz - sx * cz),
		f6 = -sy,
		f7 = sx * cy,
		f8 = cx * cy;

	const Matrix mat = { f0,f3,f6,0.f,
		f1,f4,f7,0.f,
		f2,f5,f8,0.f,
		0.f,0.f,0.f,1.f };
	return mat;
}
Matrix Matrix::GetRotationXYZTranspose(float _rx, float _ry, float _rz) {
	const float cx = cosf(_rx), sx = sinf(_rx);
	const float cy = cosf(_ry), sy = sinf(_ry);
	const float cz = cosf(_rz), sz = sinf(_rz);
	const float f0 = cy * cz,
		f1 = (sx*sy*cz - cx * sz),
		f2 = (cx*sy*cz + sx * sz),
		f3 = cy * sz,
		f4 = (sx*sy*sz + cx * cz),
		f5 = (cx*sy*sz - sx * cz),
		f6 = -sy,
		f7 = sx * cy,
		f8 = cx * cy;

	const Matrix mat = { f0,f1,f2, 0.f,
		f3,f4,f5, 0.f,
		f6,f7,f8, 0.f,
		0.f,0.f,0.f,1.f };
	return mat;
}
Matrix Matrix::GetRotationX(float _rx) {
	const float c = cosf(_rx), s = sinf(_rx);

	const Matrix mat = { 1.f, 0.f, 0.f, 0.f,
		0.f, c, -s, 0.f,
		0.f, s, c, 0.f,
		0.f, 0.f, 0.f, 1.f };
	return mat;
}
Matrix Matrix::GetRotationY(float _ry) {
	const float c = cosf(_ry), s = sinf(_ry);

	const Matrix mat = { c, 0.f, s, 0.f,
		0.f, 1.f, 0.f, 0.f,
		-s, 0.f, c, 0.f,
		0.f, 0.f, 0.f, 1.f };
	return mat;
}
Matrix Matrix::GetRotationZ(float _rz) {
	const float c = cosf(_rz), s = sinf(_rz);

	const Matrix mat = { c, -s, 0.f,  0.f,
		s, c, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };
	return mat;
}
Matrix Matrix::GetRotationXTranspose(float _rx) {
	const float c = cosf(_rx), s = sinf(_rx);

	const Matrix mat = { 1.f,0.f,0.f,0.f,
		0.f, c, s, 0.f,
		0.f, -s, c, 0.f,
		0.f,0.f,0.f,1.f };
	return mat;
}
Matrix Matrix::GetRotationYTranspose(float _ry) {
	const float c = cosf(_ry), s = sinf(_ry);

	const Matrix mat = { c, 0.f, -s, 0.f,
		0.f, 1.f, 0.f, 0.f,
		s, 0.f, c, 0.f,
		0.f,0.f,0.f,1.f };
	return mat;
}
Matrix Matrix::GetRotationZTranspose(float _rz) {
	const float c = cosf(_rz), s = sinf(_rz);

	const Matrix mat = { c,s,0.f,0.f,
		-s,c,0.f,0.f,
		0.f,0.f,1.f,0.f,
		0.f,0.f,0.f,1.f };
	return mat;
}
Matrix Matrix::GetDirection2D(float _x, float _y) {
	_x *= -1;
	_y *= -1;
	const float d = sqrtf(_x*_x + _y * _y);
	const float c = _x / d, s = -_y / d;
	const Matrix mat = { c, -s, 0.f, 0.f,
		s, c, 0.f, 0.f,
		0.f, 0.f, 1, 0.f,
		0.f, 0.f, 0.f, 1 };
	return mat;
}
Matrix Matrix::Multiply(const Matrix& _mat)const {
	Matrix matrix;
#ifdef SSE4
	matrix.m1 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(0, 0, 0, 0)), _mat.m1),
		_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(1, 1, 1, 1)), _mat.m2),
			_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(2, 2, 2, 2)), _mat.m3),
				_mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(3, 3, 3, 3)), _mat.m4))));

	matrix.m2 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(0, 0, 0, 0)), _mat.m1),
		_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(1, 1, 1, 1)), _mat.m2),
			_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(2, 2, 2, 2)), _mat.m3),
				_mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(3, 3, 3, 3)), _mat.m4))));

	matrix.m3 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(0, 0, 0, 0)), _mat.m1),
		_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(1, 1, 1, 1)), _mat.m2),
			_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(2, 2, 2, 2)), _mat.m3),
				_mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(3, 3, 3, 3)), _mat.m4))));

	matrix.m4 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(0, 0, 0, 0)), _mat.m1),
		_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(1, 1, 1, 1)), _mat.m2),
			_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(2, 2, 2, 2)), _mat.m3),
				_mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(3, 3, 3, 3)), _mat.m4))));
#else
	matrix.e[0] = e[0] * _mat.e[0] + e[1] * _mat.e[4] + e[2] * _mat.e[8] + e[3] * _mat.e[12];
	matrix.e[1] = e[0] * _mat.e[1] + e[1] * _mat.e[5] + e[2] * _mat.e[9] + e[3] * _mat.e[13];
	matrix.e[2] = e[0] * _mat.e[2] + e[1] * _mat.e[6] + e[2] * _mat.e[10] + e[3] * _mat.e[14];
	matrix.e[3] = e[0] * _mat.e[3] + e[1] * _mat.e[7] + e[2] * _mat.e[11] + e[3] * _mat.e[15];

	matrix.e[4] = e[4] * _mat.e[0] + e[5] * _mat.e[4] + e[6] * _mat.e[8] + e[7] * _mat.e[12];
	matrix.e[5] = e[4] * _mat.e[1] + e[5] * _mat.e[5] + e[6] * _mat.e[9] + e[7] * _mat.e[13];
	matrix.e[6] = e[4] * _mat.e[2] + e[5] * _mat.e[6] + e[6] * _mat.e[10] + e[7] * _mat.e[14];
	matrix.e[7] = e[4] * _mat.e[3] + e[5] * _mat.e[7] + e[6] * _mat.e[11] + e[7] * _mat.e[15];

	matrix.e[8] = e[8] * _mat.e[0] + e[9] * _mat.e[4] + e[10] * _mat.e[8] + e[11] * _mat.e[12];
	matrix.e[9] = e[8] * _mat.e[1] + e[9] * _mat.e[5] + e[10] * _mat.e[9] + e[11] * _mat.e[13];
	matrix.e[10] = e[8] * _mat.e[2] + e[9] * _mat.e[6] + e[10] * _mat.e[10] + e[11] * _mat.e[14];
	matrix.e[11] = e[8] * _mat.e[3] + e[9] * _mat.e[7] + e[10] * _mat.e[11] + e[11] * _mat.e[15];

	matrix.e[12] = e[12] * _mat.e[0] + e[13] * _mat.e[4] + e[14] * _mat.e[8] + e[15] * _mat.e[12];
	matrix.e[13] = e[12] * _mat.e[1] + e[13] * _mat.e[5] + e[14] * _mat.e[9] + e[15] * _mat.e[13];
	matrix.e[14] = e[12] * _mat.e[2] + e[13] * _mat.e[6] + e[14] * _mat.e[10] + e[15] * _mat.e[14];
	matrix.e[15] = e[12] * _mat.e[3] + e[13] * _mat.e[7] + e[14] * _mat.e[11] + e[15] * _mat.e[15];
#endif
	return matrix;
}
Matrix Matrix::operator*(const Matrix& m)const { return Multiply(m); }
Matrix& Matrix::operator*=(const Matrix& m) { return *this = *this*m; }

Matrix& Matrix::operator*=(float f) {
#ifdef SSE4
	__m128 me = _mm_set1_ps(f);
	m1 = _mm_mul_ps(m1, me);
	m2 = _mm_mul_ps(m2, me);
	m3 = _mm_mul_ps(m3, me);
	m4 = _mm_mul_ps(m4, me);
#else
	e[0] *= f; e[1] *= f; e[2] *= f; e[3] *= f;
	e[4] *= f; e[5] *= f; e[6] *= f; e[7] *= f;
	e[8] *= f; e[9] *= f; e[10] *= f; e[11] *= f;
	e[12] *= f; e[13] *= f; e[14] *= f; e[15] *= f;
#endif
	return *this;
}
Matrix Matrix::operator*(float f)const {
	Matrix matrix;
#ifdef SSE4
	__m128 me = _mm_set1_ps(f);
	matrix.m1 = _mm_mul_ps(m1, me);
	matrix.m2 = _mm_mul_ps(m2, me);
	matrix.m3 = _mm_mul_ps(m3, me);
	matrix.m4 = _mm_mul_ps(m4, me);
#else
	matrix.e[0] *= f; matrix.e[1] *= f; matrix.e[2] *= f; matrix.e[3] *= f;
	matrix.e[4] *= f; matrix.e[5] *= f; matrix.e[6] *= f; matrix.e[7] *= f;
	matrix.e[8] *= f; matrix.e[9] *= f; matrix.e[10] *= f; matrix.e[11] *= f;
	matrix.e[12] *= f; matrix.e[13] *= f; matrix.e[14] *= f; matrix.e[15] *= f;
#endif
	return matrix;
}

Matrix& Matrix::operator-=(const Matrix& m) {
#ifdef SSE4
	m1 = _mm_sub_ps(m1, m.m1);
	m2 = _mm_sub_ps(m2, m.m2);
	m3 = _mm_sub_ps(m3, m.m3);
	m4 = _mm_sub_ps(m4, m.m4);
#else
	e[0] -= m.e[0]; e[1] -= m.e[1]; e[2] -= m.e[2]; e[3] -= m.e[3];
	e[4] -= m.e[4]; e[5] -= m.e[5]; e[6] -= m.e[6]; e[7] -= m.e[7];
	e[8] -= m.e[8]; e[9] -= m.e[9]; e[10] -= m.e[10]; e[11] -= m.e[11];
	e[12] -= m.e[12]; e[13] -= m.e[13]; e[14] -= m.e[14]; e[15] -= m.e[15];
#endif
	return *this;
}
Matrix& Matrix::operator+=(const Matrix& m) {
#ifdef SSE4
	m1 = _mm_add_ps(m1, m.m1);
	m2 = _mm_add_ps(m2, m.m2);
	m3 = _mm_add_ps(m3, m.m3);
	m4 = _mm_add_ps(m4, m.m4);
#else
	e[0] += m.e[0]; e[1] += m.e[1]; e[2] += m.e[2]; e[3] += m.e[3];
	e[4] += m.e[4]; e[5] += m.e[5]; e[6] += m.e[6]; e[7] += m.e[7];
	e[8] += m.e[8]; e[9] += m.e[9]; e[10] += m.e[10]; e[11] += m.e[11];
	e[12] += m.e[12]; e[13] += m.e[13]; e[14] += m.e[14]; e[15] += m.e[15];
#endif
	return *this;
}
Matrix Matrix::operator-(const Matrix& m)const {
	Matrix matrix;
#ifdef SSE4
	matrix.m1 = _mm_sub_ps(m1, m.m1);
	matrix.m2 = _mm_sub_ps(m2, m.m2);
	matrix.m3 = _mm_sub_ps(m3, m.m3);
	matrix.m4 = _mm_sub_ps(m4, m.m4);
#else
	matrix.e[0] -= m.e[0]; matrix.e[1] -= m.e[1]; matrix.e[2] -= m.e[2]; matrix.e[3] -= m.e[3];
	matrix.e[4] -= m.e[4]; matrix.e[5] -= m.e[5]; matrix.e[6] -= m.e[6]; matrix.e[7] -= m.e[7];
	matrix.e[8] -= m.e[8]; matrix.e[9] -= m.e[9]; matrix.e[10] -= m.e[10]; matrix.e[11] -= m.e[11];
	matrix.e[12] -= m.e[12]; matrix.e[13] -= m.e[13]; matrix.e[14] -= m.e[14]; matrix.e[15] -= m.e[15];
#endif
	return matrix;
}
Matrix Matrix::operator+(const Matrix& m)const {
	Matrix matrix;
#ifdef SSE4
	matrix.m1 = _mm_add_ps(m1, m.m1);
	matrix.m2 = _mm_add_ps(m2, m.m2);
	matrix.m3 = _mm_add_ps(m3, m.m3);
	matrix.m4 = _mm_add_ps(m4, m.m4);
#else
	matrix.e[0] += m.e[0]; matrix.e[1] += m.e[1]; matrix.e[2] += m.e[2]; matrix.e[3] += m.e[3];
	matrix.e[4] += m.e[4]; matrix.e[5] += m.e[5]; matrix.e[6] += m.e[6]; matrix.e[7] += m.e[7];
	matrix.e[8] += m.e[8]; matrix.e[9] += m.e[9]; matrix.e[10] += m.e[10]; matrix.e[11] += m.e[11];
	matrix.e[12] += m.e[12]; matrix.e[13] += m.e[13]; matrix.e[14] += m.e[14]; matrix.e[15] += m.e[15];
#endif
	return matrix;
}
Matrix& Matrix::operator/=(const Matrix& m) { return *this *= m.Inverse(); }
Matrix Matrix::operator/(const Matrix& m)const { return *this*m.Inverse(); }

Matrix Matrix::Inverse()const {
	Matrix matrix;
#ifdef SSE4
	__m128 minor0, minor1, minor2, minor3;
	__m128 row0, row1 = _mm_setzero_ps(), row2, row3 = _mm_setzero_ps();
	__m128 det, tmp1 = _mm_setzero_ps();
	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(e)), (__m64*)(e + 4));
	row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(e + 8)), (__m64*)(e + 12));

	row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
	row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);

	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(e + 2)), (__m64*)(e + 6));
	row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(e + 10)), (__m64*)(e + 14));

	row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
	row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
	tmp1 = _mm_mul_ps(row2, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor0 = _mm_mul_ps(row1, tmp1);
	minor1 = _mm_mul_ps(row0, tmp1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
	tmp1 = _mm_mul_ps(row1, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
	minor3 = _mm_mul_ps(row0, tmp1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
	tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	row2 = _mm_shuffle_ps(row2, row2, 0x4E);

	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
	minor2 = _mm_mul_ps(row0, tmp1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
	tmp1 = _mm_mul_ps(row0, row1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
	tmp1 = _mm_mul_ps(row0, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
	tmp1 = _mm_mul_ps(row0, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
	det = _mm_mul_ps(row0, minor0);
	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
	tmp1 = _mm_rcp_ss(det);

	det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
	det = _mm_shuffle_ps(det, det, 0x00);

	minor0 = _mm_mul_ps(det, minor0);
	_mm_storel_pi((__m64*)(matrix.e), minor0);
	_mm_storeh_pi((__m64*)(matrix.e + 2), minor0);

	minor1 = _mm_mul_ps(det, minor1);
	_mm_storel_pi((__m64*)(matrix.e + 4), minor1);
	_mm_storeh_pi((__m64*)(matrix.e + 6), minor1);

	minor2 = _mm_mul_ps(det, minor2);
	_mm_storel_pi((__m64*)(matrix.e + 8), minor2);
	_mm_storeh_pi((__m64*)(matrix.e + 10), minor2);

	minor3 = _mm_mul_ps(det, minor3);
	_mm_storel_pi((__m64*)(matrix.e + 12), minor3);
	_mm_storeh_pi((__m64*)(matrix.e + 14), minor3);
#else
#endif
	return matrix;
}
float Matrix::GetDeterminant()const { return 0; }