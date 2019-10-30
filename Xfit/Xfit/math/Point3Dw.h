#pragma once

#include "Matrix.h"


class alignas(16) Point3Dw {
public:
	union {
		struct {
			int x, y, z, w;
		};
#ifdef SSE4
		__m128i m;
#else
#endif
	};
#ifdef SSE4
	Point3Dw(const __m128i& _m) :m(_m) {}
	Point3Dw& operator=(const __m128i& _m) { m = _m; }
	operator __m128i() { return m; }
#else
#endif
	Point3Dw(int _x=0, int _y=0, int _z=0, int _w = 1) {
#ifdef SSE4
		m = _mm_set_epi32(_x, _y, _z, _w);
#else
		x = _x; y = _y; z = _z; w = _w;
#endif
	}
	Point3Dw() {}

	unsigned Distance(const Point3Dw& _point)const {
		Point3Dw point = *this-_point;
		point *= point;
		return sqrt(point.x + point.y + point.z);
	}
	unsigned DistancePow(const Point3Dw& _point)const {
		Point3Dw point = *this-_point;
		point *= point;
		return point.x + point.y + point.z;
	}
	static Point3Dw Load(const void* _mem) {
#ifdef SSE4
		return Point3Dw(_mm_load_si128((const __m128i*)_mem));
#else
		return Point3Dw(((int*)_mem)[0], ((int*)_mem)[1], ((int*)_mem)[2], ((int*)_mem)[3]);
#endif
	}
	Point3Dw& Store(void* _mem) {
#ifdef SSE4
		_mm_store_si128((__m128i*)_mem, m);	
#else
		x = ((int*)_mem)[0]; y = ((int*)_mem)[1]; z = ((int*)_mem)[2]; w = ((int*)_mem)[3];
#endif
	}
	Point3Dw& Stream(void* _mem) {
#ifdef SSE4
		_mm_stream_si128((__m128i*)_mem, m);
#else
		x = ((int*)_mem)[0]; y = ((int*)_mem)[1]; z = ((int*)_mem)[2]; w = ((int*)_mem)[3];
#endif
	}

	Point3Dw DirectionPoint3Dw(unsigned _distance, const Point3Dw& _direction)const { return *this + (_direction*_distance); }
	Point3Dw& DirectionPoint3DwThis(unsigned _distance, const Point3Dw& _direction) { return *this += (_direction*_distance); }

	Point3Dw& operator+=(const Point3Dw& _point) {
#ifdef SSE4
		m = _mm_add_epi32(m, _point.m); 
#else
		x += _point.x; y += _point.y; z += _point.z; w += _point.w;
#endif
		return *this;
	}
	Point3Dw& operator-=(const Point3Dw& _point) { 
#ifdef SSE4
		m = _mm_sub_epi32(m, _point.m); 
#else
		x -= _point.x; y -= _point.y; z -= _point.z; w -= _point.w;
#endif
		return *this;
	}
	Point3Dw& operator*=(const Point3Dw& _point) { 
#ifdef SSE4
		m = _mm_mul_epi32(m, _point.m); 
#else
		x *= _point.x; y *= _point.y; z *= _point.z; w *= _point.w;
#endif
		return *this;
	}
	Point3Dw& operator/=(const Point3Dw& _point) { x /= _point.x; y /= _point.y; z /= _point.z; w /= _point.w; return *this; }

	Point3Dw operator+(const Point3Dw& _point) const {
#ifdef SSE4
		return _mm_add_epi32(m, _point.m);
#else
		return Point3Dw(x + _point.x, y + _point.y, z + _point.z, w + _point.w);
#endif
	}
	Point3Dw operator-(const Point3Dw& _point) const {
#ifdef SSE4
		return _mm_sub_epi32(m, _point.m);
#else
		return Point3Dw(x + _point.x, y + _point.y, z + _point.z, w + _point.w);
#endif
	}
	Point3Dw operator*(const Point3Dw& _point) const {
#ifdef SSE4
		return _mm_mul_epi32(m, _point.m);
#else
		return Point3Dw(x * _point.x, y * _point.y, z * _point.z, w * _point.w);
#endif
	}
	Point3Dw operator/(const Point3Dw& _point) const {return Point3Dw(*this) /= _point;}
	
	Point3Dw& NormalizeThis() {
		const Point3Dw p = *this*(*this);
		return *this /= sqrt(p.x + p.y + p.z); 
	}
	Point3Dw Normalize() const { return Point3Dw(*this).NormalizeThis(); }
	Point3Dw& DivideW() { return *this /= w; }
	
	Point3Dw& operator+=(int _value) {
#ifdef SSE4
		m = _mm_add_epi32(m, _mm_set1_epi32(_value));
#else
		x += _value; y += _value; z += _value; w += _value;
#endif
		return *this;
	}
	Point3Dw& operator-=(int _value) {
#ifdef SSE4
		m = _mm_sub_epi32(m, _mm_set1_epi32(_value));
#else
		x -= _value; y -= _value; z -= _value; w -= _value;
#endif
		return *this;
	}
	Point3Dw& operator*=(int _value) {
#ifdef SSE4
		m = _mm_mul_epi32(m, _mm_set1_epi32(_value));
#else
		x *= _value; y *= _value; z *= _value; w *= _value;
#endif
		return *this;
	}
	Point3Dw& operator/=(int _value) {x /= _value; y /= _value; z /= _value; w /= _value; return *this;}
	Point3Dw operator+(int _value) const {Point3Dw(*this) += _value;}
	Point3Dw operator-(int _value) const { Point3Dw(*this) -= _value; }
	Point3Dw operator*(int _value) const { Point3Dw(*this) *= _value; }
	Point3Dw operator/(int _value) const { return Point3Dw(*this) /= _value; }
	Point3Dw& operator=(unsigned _value) {
#ifdef SSE4
		m = _mm_set1_epi32(_value);
#else
		x = _value; y = _value; z = _value; w = _value;
#endif
		return *this;
	}
	bool operator==(const Point3Dw& _point)const {
#ifdef SSE4
		return _mm_movemask_epi8(_mm_cmpeq_epi32(m, _point.m)) == 65535;
#else
		return (x == _point.x) && (y == _point.y) && (z == _point.z) && (w == _point.w);
#endif
	}
	bool operator!=(const Point3Dw& _point)const {
#ifdef SSE4
		return _mm_movemask_epi8(_mm_cmpeq_epi32(m, _point.m)) != 65535;
#else
		return !((x == _point.x) && (y == _point.y) && (z == _point.z) && (w == _point.w));
#endif
	}
	Point3Dw operator-()const { return Point3Dw(-x, -y, -z, -w); }

	Point3Dw& OuterProductThis(const Point3Dw& _point) {
		x = y * _point.z - z * _point.y;
		y = z * _point.x - x * _point.z;
		z = x * _point.y - y * _point.x;
		return *this;
	}
	Point3Dw& LinePoint3DwThis(const Point3Dw& point, unsigned t) {return *this*=t;}

	int InnerProduct(const Point3Dw& _point) const {
#ifdef SSE4
		union {
			__m128i m2;
			int ar[4];
		};
		m2 = _mm_mul_epi32(m, _point.m);
		
		return ar[0]+ar[1]+ar[2]+ar[3];
#else
		return x * _point.x + y * _point.y + z * _point.z + w * _point.w;
#endif
	}
	Point3Dw OuterProduct(const Point3Dw& point) const { return Point3Dw(y*point.z - z * point.y, z*point.x - x * point.z, x*point.y - y * point.x); }
	Point3Dw LinePoint3Dw(const Point3Dw& point, unsigned t)const { return *this * t; }
};
class alignas(16) Point3DwF {
public:
	union {
		struct {
			float x, y, z, w;
		};
		struct {
			float r,g,b,a;
		};
#ifdef SSE4
		__m128 m;
#else
#endif
	};
#ifdef SSE4
	Point3DwF(const __m128& _m) :m(_m) {}
	Point3DwF& operator=(const __m128& _m) { m = _m; }
	operator __m128() { return m; }
#else
#endif
	Point3DwF() {}
	Point3DwF(float _x, float _y, float _z, float _w = 1.f) :x(_x), y(_y), z(_z), w(_w) {}
	

	float Distance(const Point3DwF& _point)const {
		const float a = x / w - _point.x / _point.w, b = y / w - _point.y / _point.w, c = z / w - _point.z / _point.w;
		return sqrtf(a * a + b * b + c * c);
	}
	float DistancePow(const Point3DwF& _point)const {
		const float a = x / w - _point.x / _point.w, b = y / w - _point.y / _point.w, c = z / w - _point.z / _point.w;
		return a * a + b * b + c * c;
	}
	Point3DwF AnglePoint3Dw(float _distance, float _rotationX = 0, float _rotationY = 0, float _rotationZ = 0.f)const {
		const Point3DwF point(cosf(_rotationY)*cosf(_rotationZ), sinf(_rotationX)*sinf(_rotationZ), cosf(_rotationX)*sinf(_rotationY), 1.f);
		return *this + (point*_distance);
	}
	Point3DwF& AnglePoint3DwThis(float _distance, float _rotationX = 0, float _rotationY = 0, float _rotationZ = 0.f) {
		const Point3DwF point(cosf(_rotationY)*cosf(_rotationZ), sinf(_rotationX)*sinf(_rotationZ), cosf(_rotationX)*sinf(_rotationY), 1.f);
		return *this += (point*_distance);
	}
	static Point3DwF ConvertUintColorARGBTo(unsigned _color) {
		return Point3DwF((float)((_color & 0xff0000) >> 16), (float)((_color & 0xff00) >> 8), (float)(_color & 0xff), (float)(_color >> 24)) / 255.f;
	}
	static Point3DwF ConvertUintColorRGBATo(unsigned _color) {
		return Point3DwF((float)(_color >> 24), (float)((_color & 0xff0000) >> 16), (float)((_color & 0xff00) >> 8), (float)(_color & 0xff)) / 255.f;
	}
	float GetAngleX(const Point3DwF& _point) const { return atan2(_point.y - y, _point.z - z)+Math::PiF; }
	float GetAngleY(const Point3DwF& _point) const { return atan2(_point.x - x, _point.z - z) + Math::PiF; }
	float GetAngleZ(const Point3DwF& _point) const { return atan2(_point.y - y, _point.x - x) + Math::PiF; }


	Point3DwF DirectionPoint3Dw(float _distance, const Point3DwF& _direction)const { return *this + (_direction*_distance); }
	Point3DwF& DirectionPoint3DwThis(float _distance, const Point3DwF& _direction) { return *this += (_direction*_distance); }

	Point3DwF& SetTransform(const Matrix& _matrix) { return *this *= _matrix; }
	/*
	 [1,2,3,    4]   [x]
	 [5,6,7,    8]   [y]
	 [9,10,11, 12] * [z]
	 [13,14,15,16]   [w]
	*/
	Point3DwF operator*(const Matrix& _matrix) const {
		return Point3DwF(x*_matrix._11 + y * _matrix._12 + z * _matrix._13 + w * _matrix._14,
			x*_matrix._21 + y * _matrix._22 + z * _matrix._23 + w * _matrix._24,
			x*_matrix._31 + y * _matrix._32 + z * _matrix._33 + w * _matrix._34,
			x*_matrix._41 + y * _matrix._42 + z * _matrix._43 + w * _matrix._44);
	}
	Point3DwF& operator*=(const Matrix& _matrix) {
		x = x * _matrix._11 + y * _matrix._12 + z * _matrix._13 + w * _matrix._14;
		y = x * _matrix._21 + y * _matrix._22 + z * _matrix._23 + w * _matrix._24;
		z = x * _matrix._31 + y * _matrix._32 + z * _matrix._33 + w * _matrix._34;
		w = x * _matrix._41 + y * _matrix._42 + z * _matrix._43 + w * _matrix._44;
		return *this;
	}
	Point3DwF operator/(const Matrix& _matrix) const { return *this * _matrix.Inverse(); }
	Point3DwF& operator/=(const Matrix& _matrix) { return *this *= _matrix.Inverse(); }
	Point3DwF operator+(const Point3DwF& _point) const {
#ifdef SSE4
		return _mm_add_ps(m, _point.m);
#else
		return Point3DwF(x + _point.x, y + _point.y, z + _point.z, w + _point.w);
#endif
	}
	Point3DwF operator-(const Point3DwF& _point) const {
#ifdef SSE4
		return _mm_sub_ps(m, _point.m);
#else
		return Point3DwF(x - _point.x, y - _point.y, z - _point.z, w - _point.w);
#endif
	}
	Point3DwF operator*(const Point3DwF& _point) const {
#ifdef SSE4
		return _mm_mul_ps(m, _point.m);
#else
		return Point3DwF(x * _point.x, y * _point.y, z * _point.z, w * _point.w);
#endif
	}
	Point3DwF operator/(const Point3DwF& _point) const {
#ifdef SSE4
		return _mm_div_ps(m, _point.m);
#else
		return Point3DwF(x / _point.x, y / _point.y, z / _point.z, w / _point.w);
#endif
	}
	Point3DwF& operator+=(const Point3DwF& _point) {
#ifdef SSE4
		m = _mm_add_ps(m, _point.m);
#else
		x += _point.x; y += _point.y; z += _point.z; w += _point.w;
#endif
		return *this;
	}
	Point3DwF& operator-=(const Point3DwF& _point) {
#ifdef SSE4
		m = _mm_sub_ps(m, _point.m);
#else
		x -= _point.x; y -= _point.y; z -= _point.z; w -= _point.w;
#endif
		return *this;
	}
	Point3DwF& operator*=(const Point3DwF& _point) {
#ifdef SSE4
		m = _mm_mul_ps(m, _point.m);
#else
		x *= _point.x; y *= _point.y; z *= _point.z; w *= _point.w;
#endif
		return *this;
	}
	Point3DwF& operator/=(const Point3DwF& _point) {
#ifdef SSE4
		m = _mm_div_ps(m, _point.m);
#else
		x *= _point.x; y *= _point.y; z *= _point.z; w *= _point.w;
#endif
		return *this;
	}
	Point3DwF& NormalizeThis() {
		const Point3DwF p = *this*(*this);
		return *this /= sqrt(p.x + p.y + p.z);
	}
	Point3DwF Normalize() const { return Point3DwF(*this).NormalizeThis(); }

	Point3DwF& DivideW() { return *this /= w; }
	Point3DwF operator+(float _value) const {
#ifdef SSE4
		return _mm_add_ps(m, _mm_set1_ps(_value));
#else
		return Point3DwF(x + _value,y + _value, z + _value, w + _value);
#endif
	}
	Point3DwF operator-(float _value) const {
#ifdef SSE4
		return _mm_sub_ps(m, _mm_set1_ps(_value));
#else
		return Point3DwF(x - _value, y - _value, z - _value, w - _value);
#endif
	}
	Point3DwF operator*(float _value) const {
#ifdef SSE4
		return _mm_mul_ps(m, _mm_set1_ps(_value));
#else
		return Point3DwF(x * _value, y * _value, z * _value, w * _value);
#endif
	}
	Point3DwF operator/(float _value) const {
#ifdef SSE4
		return _mm_div_ps(m, _mm_set1_ps(_value));
#else
		return Point3DwF(x / _value, y / _value, z / _value, w / _value);
#endif
	}
	Point3DwF& operator+=(float _value) {return *this= *this + _value;}
	Point3DwF& operator-=(float _value) { return *this = *this - _value; }
	Point3DwF& operator*=(float _value) { return *this = *this * _value; }
	Point3DwF& operator/=(float _value) { return *this = *this / _value; }
	Point3DwF& operator=(float _value) {
#ifdef SSE4
		m = _mm_set1_ps(_value);
#else
		x = _value; y = _value; z = _value; w = _value;
#endif
		return *this;
	}
	bool operator==(const Point3DwF& _point)const {
#ifdef SSE4
		return _mm_movemask_ps(_mm_cmpeq_ps(m, _point.m)) == 15.f;
#else
		return (x == _point.x) && (y == _point.y) && (z == _point.z) && (w == _point.w);
#endif
	}
	bool operator!=(const Point3DwF& _point)const {
#ifdef SSE4
		return _mm_movemask_ps(_mm_cmpeq_ps(m, _point.m)) != 15.f;
#else
		return !((x == _point.x) && (y == _point.y) && (z == _point.z) && (w == _point.w));
#endif
	}
	Point3DwF operator-()const { return Point3DwF(-x, -y, -z, -w); }

	Point3DwF& OuterProductThis(const Point3DwF& _point) {
		x = y * _point.z - z * _point.y;
		y = z * _point.x - x * _point.z;
		z = x * _point.y - y * _point.x;
		return *this;
	}
	Point3DwF& LinePoint3DwThis(const Point3DwF& _point, float t) {
		const Point3DwF& point_t = _point / _point.w*w;
		x += (point_t.x - x)*t;
		y += (point_t.y - y)*t;
		z += (point_t.z - z)*t;
		return *this;
	}
	float InnerProduct(const Point3DwF& _point) const {
#ifdef SSE4
		union {
			__m128 m2;
			float ar[4];
		};
		m2 = _mm_mul_ps(m, _point.m);
		return ar[0] + ar[1] + ar[2] + ar[3];
#else
		return  x * _point.x + y * _point.y + z * _point.z + w * _point.w;
#endif
	}
	Point3DwF OuterProduct(const Point3DwF& _point) const { return Point3DwF(y*_point.z - z * _point.y, z*_point.x - x * _point.z, x*_point.y - y * _point.x); }
	Point3DwF LinePoint3Dw(const Point3DwF& _point, float _t)const {
		const Point3DwF& point = _point / _point.w*w;
		return Point3DwF(x + (point.x - x)*_t, y + (point.y - y)*_t, z + (point.z - z)*_t);
	}
};

class ALIGN Point3DwD {
public:
	union {
		struct {
			double x, y, z, w;
		};
#ifdef AVX2
		__m256d m;
#else
#endif
	};
	Point3DwD() {}
#ifdef AVX2
	Point3DwD(const __m256d& _m) :m(_m) {}
	Point3DwD& operator=(const __m256d& _m) {
		m = _m;
		return *this;
	}
	operator __m256d() { return m; }
#else
#endif
	Point3DwD(double _x, double _y, double _z, double _w = 1.0) :x(_x), y(_y), z(_x), w(_w) {}

	double DistancePow(const Point3DwD& _point)const {
		const double a = x / w - _point.x / _point.w, b = y / w - _point.y / _point.w, c = z / w - _point.z / _point.w;
		return a * a + b * b + c * c;
	}
	double Distance(const Point3DwD& _point)const { return sqrt(DistancePow(_point)); }
	Point3DwD AnglePoint3Dw(double _distance, double _rotationX = 0.0, double _rotationY = 0.0, double _rotationZ = 0.0)const {
		const Point3DwD point(cos(_rotationY*Math::DIVPI_180)*cos(_rotationZ*Math::DIVPI_180), sin(_rotationX*Math::DIVPI_180)*sin(_rotationZ*Math::DIVPI_180), cos(_rotationX*Math::DIVPI_180)*sin(_rotationY*Math::DIVPI_180), 1.0);
		return *this + (point*_distance);
	}
	Point3DwD& AnglePoint3DwThis(double _distance, double _rotationX = 0.0, double _rotationY = 0.0, double _rotationZ = 0.0) {
		const Point3DwD point(cos(_rotationY*Math::DIVPI_180)*cos(_rotationZ*Math::DIVPI_180), sin(_rotationX*Math::DIVPI_180)*sin(_rotationZ*Math::DIVPI_180), cos(_rotationX*Math::DIVPI_180)*sin(_rotationY*Math::DIVPI_180), 1.0);
		return *this += (point*_distance);
	}

	double GetAngleX(const Point3DwD& _point) const { return atan2(_point.y - y, _point.z - z)*Math::DIV180_PI + 180.0; }
	double GetAngleY(const Point3DwD& _point) const { return atan2(_point.x - x, _point.z - z)*Math::DIV180_PI + 180.0; }
	double GetAngleZ(const Point3DwD& _point) const { return atan2(_point.y - y, _point.x - x)*Math::DIV180_PI + 180.0; }


	Point3DwD DirectionPoint3Dw(double _distance, const Point3DwD& _direction)const { return *this + (_direction*_distance); }
	Point3DwD& DirectionPoint3DwThis(double _distance, const Point3DwD& _direction) { return *this += (_direction*_distance); }

	Point3DwD& SetTransform(const Matrix& _matrix) { return *this *= _matrix; }
	Point3DwD operator*(const Matrix& _matrix) const {
		return Point3DwD(x*_matrix._11 + y * _matrix._21 + z * _matrix._31 + w * _matrix._41,
			x*_matrix._12 + y * _matrix._22 + z * _matrix._32 + w * _matrix._42,
			x*_matrix._13 + y * _matrix._23 + z * _matrix._33 + w * _matrix._43,
			x*_matrix._14 + y * _matrix._24 + z * _matrix._34 + w * _matrix._44);
	}
	Point3DwD& operator*=(const Matrix& _matrix) {
		x = x * _matrix._11 + y * _matrix._21 + z * _matrix._31 + w * _matrix._41;
		y = x * _matrix._12 + y * _matrix._22 + z * _matrix._32 + w * _matrix._42;
		z = x * _matrix._13 + y * _matrix._23 + z * _matrix._33 + w * _matrix._43;
		w = x * _matrix._14 + y * _matrix._24 + z * _matrix._34 + w * _matrix._44;
		return *this;
	}
	Point3DwD operator/(const Matrix& _matrix) const { return *this*_matrix.Inverse(); }
	Point3DwD& operator/=(const Matrix& _matrix) { return *this = *this/_matrix; }
	Point3DwD operator+(const Point3DwD& _point) const {
#ifdef AVX2
		return _mm256_add_pd(m, _point.m);
#else
		return Point3DwD(x + _point.x, y + _point.y, z + _point.z, w + _point.w);
#endif
	}
	Point3DwD operator-(const Point3DwD& _point) const {
#ifdef AVX2
		return _mm256_sub_pd(m, _point.m);
#else
		return Point3DwD(x - _point.x, y - _point.y, z - _point.z, w - _point.w);
#endif
	}
	Point3DwD operator*(const Point3DwD& _point) const {
#ifdef AVX2
		return _mm256_mul_pd(m, _point.m);
#else
		return Point3DwD(x * _point.x, y * _point.y, z * _point.z, w * _point.w);
#endif
	}
	Point3DwD operator/(const Point3DwD& _point) const {
#ifdef AVX2
		return _mm256_div_pd(m, _point.m);
#else
		return Point3DwD(x / _point.x, y / _point.y, z / _point.z, w / _point.w);
#endif
	}
	Point3DwD& NormalizeThis() {return *this=Normalize();}
	Point3DwD Normalize() const {
		const Point3DwD p = *this*(*this);
		return *this / sqrt(p.x + p.y + p.z);
	}
	Point3DwD& DivideW() { return *this /= w; }
	Point3DwD& operator+=(const Point3DwD& _point) {return *this = *this + _point;}
	Point3DwD& operator-=(const Point3DwD& _point) {return *this=*this-_point;}
	Point3DwD& operator*=(const Point3DwD& _point) {return *this = *this * _point;}
	Point3DwD& operator/=(const Point3DwD& _point) {return *this = *this / _point;}
	Point3DwD operator+(double _value) const {
#ifdef AVX2
		return _mm256_add_pd(m, _mm256_set1_pd(_value));
#else
		return Point3DwD(x + _value, y + _value, z + _value, w + _value);
#endif
	}
	Point3DwD operator-(double _value) const {
#ifdef AVX2
		return _mm256_sub_pd(m, _mm256_set1_pd(_value));
#else
		return Point3DwD(x - _value, y - _value, z - _value, w - _value);
#endif
	}
	Point3DwD operator*(double _value) const {
#ifdef AVX2
		return _mm256_mul_pd(m, _mm256_set1_pd(_value));
#else
		return Point3DwD(x * _value, y * _value, z * _value, w * _value);
#endif
	}
	Point3DwD operator/(double _value) const {
#ifdef AVX2
		return _mm256_div_pd(m, _mm256_set1_pd(_value));
#else
		return Point3DwD(x / _value, y / _value, z / _value, w / _value);
#endif
	}
	Point3DwD& operator+=(double _value) {
		return *this=*this+_value;
	}
	Point3DwD& operator-=(double _value) {
		return *this = *this - _value;
	}
	Point3DwD& operator*=(double _value) {
		return *this = *this * _value;
	}
	Point3DwD& operator/=(double _value) {
		return *this = *this / _value;
	}
	Point3DwD& operator=(double _value) {
#ifdef AVX2
		m = _mm256_set1_pd(_value);
#else
		x = _value; y = _value; z = _value; w = _value;
#endif
		return *this;
	}
	bool operator==(const Point3DwD& _point)const {
#ifdef AVX2
		return _mm256_movemask_pd(_mm256_cmp_pd(m, _point.m, _CMP_EQ_OQ)) == -1;
#else
		return (x == _point.x) && (y == _point.y) && (z == _point.z) && (w == _point.w);
#endif
	}
	bool operator!=(const Point3DwD& _point)const {
#ifdef AVX2
		return _mm256_movemask_pd(_mm256_cmp_pd(m, _point.m, _CMP_NEQ_OQ)) == -1;
#else
		return !((x == _point.x) && (y == _point.y) && (z == _point.z) && (w == _point.w));
#endif
	}
	Point3DwD operator-()const { return Point3DwD(-x, -y, -z, -w); }

	Point3DwD& OuterProductThis(const Point3DwD& _point) {
		x = y * _point.z - z * _point.y;
		y = z * _point.x - x * _point.z;
		z = x * _point.y - y * _point.x;
		return *this;
	}
	Point3DwD& LinePoint3DwThis(const Point3DwD& _point, double _t) {
		const Point3DwD& point = _point / _point.w*w;
		x += (point.x - x)*_t;
		y += (point.y - y)*_t;
		z += (point.z - z)*_t;
		return *this;
	}

	double InnerProduct(const Point3DwD& _point) const { 
#ifdef AVX2
		union {
			__m256d m2;
			double ar[4];
		};
		m2 = _mm256_mul_pd(m, _point.m);
		return ar[0] + ar[1] + ar[2] + ar[3];
#else
		return  x * _point.x + y * _point.y + z * _point.z + w * _point.w; 
#endif
	}
	Point3DwD OuterProduct(const Point3DwD& _point) const { return Point3DwD(y*_point.z - z * _point.y, z*_point.x - x * _point.z, x*_point.y - y * _point.x); }
	Point3DwD LinePoint3Dw(const Point3DwD& _point, double _t)const {
		const Point3DwD& point = _point / _point.w*w;
		return Point3DwD(x + (point.x - x)*_t, y + (point.y - y)*_t, z + (point.z - z)*_t);
	}
};


