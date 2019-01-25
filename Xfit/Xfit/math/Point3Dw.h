#pragma once

#include "Matrix.h"


class alignas(16) Point3Dw {
public:
	union {
		struct {
			int x, y, z, w;
		};
		__m128i m;
	};
	Point3Dw(const __m128i& _m) :m(_m) {}
	Point3Dw& operator=(const __m128i& _m) { m = _m; }
	Point3Dw(int _x=0, int _y=0, int _z=0, int _w = 1) {m = _mm_set_epi32(_x, _y, _z, _w); }
	operator __m128i() { return m; }


	unsigned Distance(const Point3Dw& point)const {
		Point3Dw p = *this-point;
		p *= p;
		return sqrt(p.x+p.y+p.z);
	}
	unsigned DistancePow(const Point3Dw& point)const {
		Point3Dw p = *this-point;
		p *= p;
		return p.x + p.y + p.z;
	}
	static Point3Dw Load(const void* _mem) {
		return Point3Dw(_mm_load_si128((const __m128i*)_mem));
	}
	Point3Dw& Store(void* _mem) {_mm_store_si128((__m128i*)_mem, m);	}
	Point3Dw& Stream(void* _mem) {_mm_stream_si128((__m128i*)_mem, m);}

	Point3Dw DirectionPoint3Dw(unsigned distance, const Point3Dw& direction)const { return *this + (direction*distance); }
	Point3Dw& DirectionPoint3DwThis(unsigned distance, const Point3Dw& direction) { return *this += (direction*distance); }

	Point3Dw& operator+=(const Point3Dw& point) { m = _mm_add_epi32(m, point.m); return *this; }
	Point3Dw& operator-=(const Point3Dw& point) { m = _mm_sub_epi32(m, point.m); return *this; }
	Point3Dw& operator*=(const Point3Dw& point) { m = _mm_mul_epi32(m, point.m); return *this; }
	Point3Dw& operator/=(const Point3Dw& point) { x /= point.x; y /= point.y; z /= point.z; w /= point.w; return *this; }

	Point3Dw operator+(const Point3Dw& point) const {return _mm_add_epi32(m, point.m);}
	Point3Dw operator-(const Point3Dw& point) const {return _mm_sub_epi32(m, point.m);}
	Point3Dw operator*(const Point3Dw& point) const {return _mm_mul_epi32(m, point.m);}
	Point3Dw operator/(const Point3Dw& point) const {return Point3Dw(*this) /= point;}
	
	Point3Dw& NormalizeThis() {
		const Point3Dw p = *this*(*this);
		return *this /= sqrt(p.x + p.y + p.z); 
	}
	Point3Dw Normalize() const { return Point3Dw(*this).NormalizeThis(); }
	Point3Dw& DivideW() { return *this /= w; }
	
	Point3Dw& operator+=(int value) {
		m = _mm_add_epi32(m, _mm_set1_epi32(value));
		return *this;
	}
	Point3Dw& operator-=(int value) {
		m = _mm_sub_epi32(m, _mm_set1_epi32(value));
		return *this;
	}
	Point3Dw& operator*=(int value) {
		m = _mm_mul_epi32(m, _mm_set1_epi32(value));
		return *this;
	}
	Point3Dw& operator/=(int value) {x /= value; y /= value; z /= value; w /= value; return *this;}
	Point3Dw operator+(int value) const {Point3Dw(*this) += value;}
	Point3Dw operator-(int value) const { Point3Dw(*this) -= value; }
	Point3Dw operator*(int value) const { Point3Dw(*this) *= value; }
	Point3Dw operator/(int value) const { return Point3Dw(*this) /= value; }
	Point3Dw& operator=(unsigned value) {
		m = _mm_set1_epi32(value);
		return *this;
	}
	bool operator==(const Point3Dw& point)const {
		return (bool)_mm_cmpeq_epi32(m, point.m).m128i_i32[0];
	}
	bool operator!=(const Point3Dw& point)const {
		return !((bool)_mm_cmpeq_epi32(m, point.m).m128i_i32[0]);
	}
	Point3Dw operator-()const { return Point3Dw(-x, -y, -z, -w); }

	Point3Dw& OuterProductThis(const Point3Dw& point) {
		x = y * point.z - z * point.y;
		y = z * point.x - x * point.z;
		z = x * point.y - y * point.x;
		return *this;
	}
	Point3Dw& LinePoint3DwThis(const Point3Dw& point, unsigned t) {return *this*=t;}

	int InnerProduct(const Point3Dw& _point) const {
		__m128i m2 = _mm_mul_epi32(m, _point.m);
		return m2.m128i_i32[0]+ m2.m128i_i32[1]+m2.m128i_i32[2]+m2.m128i_i32[3];
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
		__m128 m;
	};
	Point3DwF(const __m128& _m) :m(_m) {}
	Point3DwF& operator=(const __m128& _m) { m = _m; }
	Point3DwF(float nX, float nY, float nZ, float nW = 1) :x(nX), y(nY), z(nZ), w(nW) {}
	operator __m128() { return m; }

	float Distance(const Point3DwF& point)const {
		const float a = x / w - point.x / point.w, b = y / w - point.y / point.w, c = z / w - point.z / point.w;
		return sqrtf(a*a + b * b + c * c);
	}
	float DistancePow(const Point3DwF& point)const {
		const float a = x / w - point.x / point.w, b = y / w - point.y / point.w, c = z / w - point.z / point.w;
		return a * a + b * b + c * c;
	}
	Point3DwF AnglePoint3Dw(float distance, float rotationX = 0, float rotationY = 0, float rotationZ = 0)const {
		const Point3DwF p(cosf(rotationY)*cosf(rotationZ), sinf(rotationX)*sinf(rotationZ), cosf(rotationX)*sinf(rotationY), 1.f);
		return *this + (p*distance);
	}
	Point3DwF& AnglePoint3DwThis(float distance, float rotationX = 0, float rotationY = 0, float rotationZ = 0) {
		const Point3DwF p(cosf(rotationY)*cosf(rotationZ), sinf(rotationX)*sinf(rotationZ), cosf(rotationX)*sinf(rotationY), 1.f);
		return *this += (p*distance);
	}
	static Point3DwF ConvertUintColorARGBTo(unsigned _color) {
		return Point3DwF((float)((_color & 0xff0000) >> 16), (float)((_color & 0xff00) >> 8), (float)(_color & 0xff), (float)(_color >> 24))/255.f;
	}
	static Point3DwF ConvertUintColorRGBATo(unsigned _color) {
		return Point3DwF((float)(_color >> 24), (float)((_color & 0xff0000) >> 16), (float)((_color & 0xff00) >> 8), (float)(_color & 0xff)) / 255.f;
	}
	float GetAngleX(const Point3DwF& point) const { return atan2(point.y - y, point.z - z)+Math::PiF; }
	float GetAngleY(const Point3DwF& point) const { return atan2(point.x - x, point.z - z) + Math::PiF; }
	float GetAngleZ(const Point3DwF& point) const { return atan2(point.y - y, point.x - x) + Math::PiF; }


	Point3DwF DirectionPoint3Dw(float distance, const Point3DwF& direction)const { return *this + (direction*distance); }
	Point3DwF& DirectionPoint3DwThis(float distance, const Point3DwF& direction) { return *this += (direction*distance); }

	Point3DwF& SetTransform(const Matrix& matrix) { return *this *= matrix; }
	Point3DwF operator*(const Matrix& matrix) const {
		return Point3DwF(x*matrix._11 + y * matrix._21 + z * matrix._31 + w * matrix._41,
			x*matrix._12 + y * matrix._22 + z * matrix._32 + w * matrix._42,
			x*matrix._13 + y * matrix._23 + z * matrix._33 + w * matrix._43,
			x*matrix._14 + y * matrix._24 + z * matrix._34 + w * matrix._44);
	}
	Point3DwF& operator*=(const Matrix& matrix) {
		x = x * matrix._11 + y * matrix._21 + z * matrix._31 + w * matrix._41;
		y = x * matrix._12 + y * matrix._22 + z * matrix._32 + w * matrix._42;
		z = x * matrix._13 + y * matrix._23 + z * matrix._33 + w * matrix._43;
		w = x * matrix._14 + y * matrix._24 + z * matrix._34 + w * matrix._44;
		return *this;
	}
	Point3DwF operator/(const Matrix& matrix) const { return *this*matrix.Inverse(); }
	Point3DwF& operator/=(const Matrix& matrix) { return *this *= matrix.Inverse(); }
	Point3DwF operator+(const Point3DwF& point) const {
		return _mm_add_ps(m, point.m);
	}
	Point3DwF operator-(const Point3DwF& point) const {
		return _mm_sub_ps(m, point.m);
	}
	Point3DwF operator*(const Point3DwF& point) const {
		return _mm_mul_ps(m, point.m);
	}
	Point3DwF operator/(const Point3DwF& point) const {
		return _mm_div_ps(m, point.m);
	}
	Point3DwF& operator+=(const Point3DwF& point) {
		m = _mm_add_ps(m, point.m);
		return *this;
	}
	Point3DwF& operator-=(const Point3DwF& point) {
		m = _mm_sub_ps(m, point.m);
		return *this;
	}
	Point3DwF& operator*=(const Point3DwF& point) {
		m = _mm_mul_ps(m, point.m);
		return *this;
	}
	Point3DwF& operator/=(const Point3DwF& point) {
		m = _mm_div_ps(m, point.m);
		return *this;
	}
	Point3DwF& NormalizeThis() {
		const Point3DwF p = *this*(*this);
		return *this /= sqrt(p.x + p.y + p.z);
	}
	Point3DwF Normalize() const { return Point3DwF(*this).NormalizeThis(); }

	Point3DwF& DivideW() { return *this /= w; }
	Point3DwF operator+(float value) const {
		const Point3DwF p = _mm_add_ps(m, _mm_set1_ps(value));
		return p;
	}
	Point3DwF operator-(float value) const {
		const Point3DwF p = _mm_sub_ps(m, _mm_set1_ps(value));
		return p;
	}
	Point3DwF operator*(float value) const {
		const Point3DwF p = _mm_mul_ps(m, _mm_set1_ps(value));
		return p;
	}
	Point3DwF operator/(float value) const {
		const Point3DwF p = _mm_div_ps(m, _mm_set1_ps(value));
		return p;
	}
	Point3DwF& operator+=(float value) {return *this=*this+value;}
	Point3DwF& operator-=(float value) { return *this = *this - value; }
	Point3DwF& operator*=(float value) { return *this = *this * value; }
	Point3DwF& operator/=(float value) { return *this = *this / value; }
	Point3DwF& operator=(float value) {
		m = _mm_set1_ps(value);
		return *this;
	}
	bool operator==(const Point3DwF& point)const {
		return (bool)_mm_cmpeq_ps(m, point.m).m128_f32[0];
	}
	bool operator!=(const Point3DwF& point)const {
		return !((bool)_mm_cmpeq_ps(m, point.m).m128_f32[0]);
	}
	Point3DwF operator-()const { return Point3DwF(-x, -y, -z, -w); }

	Point3DwF& OuterProductThis(const Point3DwF& point) {
		x = y * point.z - z * point.y;
		y = z * point.x - x * point.z;
		z = x * point.y - y * point.x;
		return *this;
	}
	Point3DwF& LinePoint3DwThis(const Point3DwF& point, float t) {
		const Point3DwF& point_t = point / point.w*w;
		x += (point_t.x - x)*t;
		y += (point_t.y - y)*t;
		z += (point_t.z - z)*t;
		return *this;
	}
	float InnerProduct(const Point3DwF& point) const { return  x * point.x + y * point.y + z * point.z + w * point.w; }
	Point3DwF OuterProduct(const Point3DwF& point) const { return Point3DwF(y*point.z - z * point.y, z*point.x - x * point.z, x*point.y - y * point.x); }
	Point3DwF LinePoint3Dw(const Point3DwF& point, float t)const {
		const Point3DwF& point_t = point / point.w*w;
		return Point3DwF(x + (point_t.x - x)*t, y + (point_t.y - y)*t, z + (point_t.z - z)*t);
	}
};

class ALIGN Point3DwD {
public:
	union {
		struct {
			double x, y, z, w;
		};
		__m256d m;
	};
	Point3DwD() {}
	Point3DwD(const __m256d& _m) :m(_m) {}
	Point3DwD& operator=(const __m256d& _m) {
		m = _m;
		return *this;
	}
	Point3DwD(double nX, double nY, double nZ, double nW = 1) :x(nX), y(nY), z(nZ), w(nW) {}
	operator __m256d() { return m; }

	double DistancePow(const Point3DwD& point)const {
		const double a = x / w - point.x / point.w, b = y / w - point.y / point.w, c = z / w - point.z / point.w;
		return a * a + b * b + c * c;
	}
	double Distance(const Point3DwD& point)const { return sqrtf(DistancePow(point)); }
	Point3DwD AnglePoint3Dw(double distance, double rotationX = 0, double rotationY = 0, double rotationZ = 0)const {
		const Point3DwD p(cos(rotationY*Math::DIVPI_180)*cos(rotationZ*Math::DIVPI_180), sin(rotationX*Math::DIVPI_180)*sin(rotationZ*Math::DIVPI_180), cos(rotationX*Math::DIVPI_180)*sin(rotationY*Math::DIVPI_180), 1.f);
		return *this + (p*distance);
	}
	Point3DwD& AnglePoint3DwThis(double distance, double rotationX = 0, double rotationY = 0, double rotationZ = 0) {
		const Point3DwD p(cos(rotationY*Math::DIVPI_180)*cos(rotationZ*Math::DIVPI_180), sin(rotationX*Math::DIVPI_180)*sin(rotationZ*Math::DIVPI_180), cos(rotationX*Math::DIVPI_180)*sin(rotationY*Math::DIVPI_180), 1.f);
		return *this += (p*distance);
	}

	double GetAngleX(const Point3DwD& point) const { return atan2(point.y - y, point.z - z)*Math::DIV180_PI + 180.0; }
	double GetAngleY(const Point3DwD& point) const { return atan2(point.x - x, point.z - z)*Math::DIV180_PI + 180.0; }
	double GetAngleZ(const Point3DwD& point) const { return atan2(point.y - y, point.x - x)*Math::DIV180_PI + 180.0; }


	Point3DwD DirectionPoint3Dw(double distance, const Point3DwD& direction)const { return *this + (direction*distance); }
	Point3DwD& DirectionPoint3DwThis(double distance, const Point3DwD& direction) { return *this += (direction*distance); }

	Point3DwD& SetTransform(const Matrix& matrix) { return *this *= matrix; }
	Point3DwD operator*(const Matrix& matrix) const {
		return Point3DwD(x*matrix._11 + y * matrix._21 + z * matrix._31 + w * matrix._41,
			x*matrix._12 + y * matrix._22 + z * matrix._32 + w * matrix._42,
			x*matrix._13 + y * matrix._23 + z * matrix._33 + w * matrix._43,
			x*matrix._14 + y * matrix._24 + z * matrix._34 + w * matrix._44);
	}
	Point3DwD& operator*=(const Matrix& matrix) {
		x = x * matrix._11 + y * matrix._21 + z * matrix._31 + w * matrix._41;
		y = x * matrix._12 + y * matrix._22 + z * matrix._32 + w * matrix._42;
		z = x * matrix._13 + y * matrix._23 + z * matrix._33 + w * matrix._43;
		w = x * matrix._14 + y * matrix._24 + z * matrix._34 + w * matrix._44;
		return *this;
	}
	Point3DwD operator/(const Matrix& matrix) const { return *this*matrix.Inverse(); }
	Point3DwD& operator/=(const Matrix& matrix) { return *this = *this/matrix; }
	Point3DwD operator+(const Point3DwD& point) const {
		return _mm256_add_pd(m, point.m);
	}
	Point3DwD operator-(const Point3DwD& point) const {
		return _mm256_sub_pd(m, point.m);
	}
	Point3DwD operator*(const Point3DwD& point) const {
		return _mm256_mul_pd(m, point.m);
	}
	Point3DwD operator/(const Point3DwD& point) const {
		return _mm256_div_pd(m, point.m);
	}
	Point3DwD& NormalizeThis() {return *this=Normalize();}
	Point3DwD Normalize() const {
		const Point3DwD p = *this*(*this);
		return *this / sqrt(p.x + p.y + p.z);
	}
	Point3DwD& DivideW() { return *this /= w; }
	Point3DwD& operator+=(const Point3DwD& point) {return *this = *this + point;}
	Point3DwD& operator-=(const Point3DwD& point) {return *this=*this-point;}
	Point3DwD& operator*=(const Point3DwD& point) {return *this = *this * point;}
	Point3DwD& operator/=(const Point3DwD& point) {return *this = *this / point;}
	Point3DwD operator+(double value) const {
		return _mm256_add_pd(m, _mm256_set1_pd(value));
	}
	Point3DwD operator-(double value) const {
		return _mm256_sub_pd(m, _mm256_set1_pd(value));
	}
	Point3DwD operator*(double value) const {
		return _mm256_mul_pd(m, _mm256_set1_pd(value));
	}
	Point3DwD operator/(double value) const {
		return _mm256_div_pd(m, _mm256_set1_pd(value));
	}
	Point3DwD& operator+=(double value) {
		return *this=*this+value;
	}
	Point3DwD& operator-=(double value) {
		return *this = *this - value;
	}
	Point3DwD& operator*=(double value) {
		return *this = *this * value;
	}
	Point3DwD& operator/=(double value) {
		return *this = *this / value;
	}
	Point3DwD& operator=(double value) {
		m = _mm256_set1_pd(value);
		return *this;
	}
	bool operator==(const Point3DwD& point)const {
		return _mm256_movemask_pd(_mm256_cmp_pd(m, point.m, _CMP_EQ_OQ)) == -1;
	}
	bool operator!=(const Point3DwD& point)const {
		return _mm256_movemask_pd(_mm256_cmp_pd(m, point.m, _CMP_NEQ_OQ)) == -1;
	}
	Point3DwD operator-()const { return Point3DwD(-x, -y, -z, -w); }

	Point3DwD& OuterProductThis(const Point3DwD& point) {
		x = y * point.z - z * point.y;
		y = z * point.x - x * point.z;
		z = x * point.y - y * point.x;
		return *this;
	}
	Point3DwD& LinePoint3DwThis(const Point3DwD& point, double t) {
		const Point3DwD& point_t = point / point.w*w;
		x += (point_t.x - x)*t;
		y += (point_t.y - y)*t;
		z += (point_t.z - z)*t;
		return *this;
	}

	double InnerProduct(const Point3DwD& point) const { return  x * point.x + y * point.y + z * point.z + w * point.w; }
	Point3DwD OuterProduct(const Point3DwD& point) const { return Point3DwD(y*point.z - z * point.y, z*point.x - x * point.z, x*point.y - y * point.x); }
	Point3DwD LinePoint3Dw(const Point3DwD& point, double t)const {
		const Point3DwD& point_t = point / point.w*w;
		return Point3DwD(x + (point_t.x - x)*t, y + (point_t.y - y)*t, z + (point_t.z - z)*t);
	}
};


