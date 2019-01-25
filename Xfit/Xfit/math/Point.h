#pragma once

#include "Matrix.h"

class Point {
public:
	int x, y;

	Point() {}
	Point(int _x, int _y) :x(_x), y(_y) {}

	int DistancePow(const Point& _point) const {
		const int a = x - _point.x, b = y - _point.y;
		return (a*a + b * b);
	}

	Point DirectionPoint(int _distance, const Point& _direction) const { return *this + (_direction*_distance); }
	Point& DirectionPointThis(int _distance, const Point& _direction) { return *this += (_direction*_distance); }

	Point operator+(const Point& _point) const { return Point(x + _point.x, y + _point.y); }
	Point operator-(const Point& _point) const { return Point(x - _point.x, y - _point.y); }
	Point operator*(const Point& _point) const { return Point(x * _point.x, y * _point.y); }
	Point operator/(const Point& _point) const { return Point(x / _point.x, y / _point.y); }
	Point& operator+=(const Point& _point) { return *this = *this + _point; }
	Point& operator-=(const Point& _point) { return *this = *this - _point; }
	Point& operator*=(const Point& _point) { return *this = *this * _point; }
	Point& operator/=(const Point& _point) { return *this = *this / _point; }

	Point operator+(int _value) const { return Point(x + _value, y + _value); }
	Point operator-(int _value) const { return Point(x - _value, y - _value); }
	Point operator*(int _value) const { return Point(x * _value, y * _value); }
	Point operator/(int _value) const { return Point(x / _value, y / _value); }
	Point& operator+=(int _value) { return *this = *this + _value; }
	Point& operator-=(int _value) { return *this = *this - _value; }
	Point& operator*=(int _value) { return *this = *this * _value; }
	Point& operator/=(int _value) { return *this = *this / _value; }

	bool operator==(const Point& _point)const { return x == _point.x&&y == _point.y; }
	bool operator!=(const Point& _point)const { return x != _point.x || y != _point.y; }
	Point operator-()const { return Point(-x, -y); }
	int InnerProduct(const Point& _point) const { return x * _point.x + y * _point.y; }
	Point LinePoint(const Point& _point, int _t) const { return Point(x + (_point.x - x)*_t, y + (_point.y - y)*_t); }
	Point& LinePointThis(const Point& _point, int _t) {
		x += (_point.x - x)*_t;
		y += (_point.y - y)*_t;
		return *this;
	}
};
class PointF {
public:
	float x, y;

	PointF() {}
	PointF(float _x, float _y) :x(_x), y(_y) {}
	float Distance(const PointF& _point) const {
		const float a = x - _point.x, b = y - _point.y;
		return sqrtf((a*a + b * b));
	}
	float DistancePow(const PointF& _point) const {
		const float a = x - _point.x, b = y - _point.y;
		return (a*a + b * b);
	}

	PointF AnglePoint(float _distance, float _angle = 0) const { return PointF(x + cosf(_angle*Math::DIVPI_180)*_distance, y + sinf(_angle*Math::DIVPI_180)*_distance); }
	PointF DirectionPoint(float _distance, const PointF& _direction) const { return *this + (_direction*_distance); }

	PointF& AnglePointThis(float _distance, float _angle = 0) {
		x += cosf(_angle*Math::DIVPI_180)*_distance;
		y += sinf(_angle*Math::DIVPI_180)*_distance;
		return *this;
	}
	PointF& DirectionPointThis(float _distance, const PointF& _direction) { return *this += (_direction*_distance); }

	float GetAngle(const PointF& _point) const { return atan2f((_point.y - y), (_point.x - x))*Math::DIV180_PI + 180; }

	PointF& operator*=(const Matrix& _matrix) {
		const PointF xx(_matrix._11, _matrix._21), yy(_matrix._12, _matrix._22);
		x = InnerProduct(xx) + _matrix._41;
		y = InnerProduct(yy) + _matrix._42;
		return *this;
	}
	PointF operator*(const Matrix& _matrix) const {
		const PointF xx(_matrix._11, _matrix._21), yy(_matrix._12, _matrix._22);
		return PointF(InnerProduct(xx) + _matrix._41, InnerProduct(yy) + _matrix._42);
	}
	PointF& SetTransform(const Matrix& _matrix) { return *this *= _matrix; }

	PointF& operator/=(const Matrix& _matrix) { return *this *= _matrix.Inverse(); }
	PointF operator/(const Matrix& _matrix) const { return *this*_matrix.Inverse(); }

	PointF operator+(const PointF& _point) const { return PointF(x + _point.x, y + _point.y); }
	PointF operator-(const PointF& _point) const { return PointF(x - _point.x, y - _point.y); }
	PointF operator*(const PointF& _point) const { return PointF(x * _point.x, y * _point.y); }
	PointF operator/(const PointF& _point) const { return PointF(x / _point.x, y / _point.y); }
	PointF& operator+=(const PointF& _point) {return *this= *this+_point;}
	PointF& operator-=(const PointF& _point) { return *this = *this - _point; }
	PointF& operator*=(const PointF& _point) { return *this = *this * _point; }
	PointF& operator/=(const PointF& _point) { return *this = *this / _point; }

	PointF operator+(float _value) const { return PointF(x + _value, y + _value); }
	PointF operator-(float _value) const { return PointF(x - _value, y - _value); }
	PointF operator*(float _value) const { return PointF(x * _value, y * _value); }
	PointF operator/(float _value) const { return PointF(x / _value, y / _value); }
	PointF& operator+=(float _value) { return *this = *this + _value; }
	PointF& operator-=(float _value) { return *this = *this - _value; }
	PointF& operator*=(float _value) { return *this = *this * _value; }
	PointF& operator/=(float _value) { return *this = *this / _value; }

	bool operator==(const PointF& _point)const { return x == _point.x&&y == _point.y; }
	bool operator!=(const PointF& _point)const { return x != _point.x || y != _point.y; }
	PointF operator-()const { return PointF(-x, -y); }
	float InnerProduct(const PointF& _point) const { return x * _point.x + y * _point.y; }
	PointF LinePoint(const PointF& _point, float _t) const { return PointF(x + (_point.x - x)*_t, y + (_point.y - y)*_t); }
	PointF& LinePointThis(const PointF& _point, float _t) {
		x += (_point.x - x)*_t;
		y += (_point.y - y)*_t;
		return *this;
	}
};
class PointD {
public:
	double x, y;

	PointD() {}
	PointD(double _x, double _y) :x(_x), y(_y) {}
	double Distance(const PointD& _point) const {
		const double a = x - _point.x, b = y - _point.y;
		return sqrt((a*a + b * b));
	}
	constexpr double DistancePow(const PointD& _point) const {
		const double a = x - _point.x, b = y - _point.y;
		return (a*a + b * b);
	}

	PointD AnglePoint(double _distance, double _angle = 0) const { return PointD(x + cos(_angle*Math::DIVPI_180)*_distance, y + sin(_angle*Math::DIVPI_180)*_distance); }
	PointD DirectionPoint(double _distance, const PointD& _direction) const { return *this + (_direction* _distance); }

	PointD& AnglePointThis(double _distance, double _angle = 0) {
		x += cos(_angle*Math::DIVPI_180)*_distance;
		y += sin(_angle*Math::DIVPI_180)*_distance;
		return *this;
	}
	PointD& DirectionPointThis(double _distance, const PointD& _direction) { return *this += (_direction*_distance); }

	double GetAngle(const PointD& _point) const { return atan2((_point.y - y), (_point.x - x))*Math::DIV180_PI + 180; }

	 PointD& operator*=(const Matrix& _matrix) {
		const PointD xx(_matrix._11, _matrix._21), yy(_matrix._12, _matrix._22);
		x = InnerProduct(xx) + _matrix._41;
		y = InnerProduct(yy) + _matrix._42;
		return *this;
	}
	PointD operator*(const Matrix& _matrix) const {
		const PointD xx(_matrix._11, _matrix._21), yy(_matrix._12, _matrix._22);
		return PointD(InnerProduct(xx) + _matrix._41, InnerProduct(yy) + _matrix._42);
	}
	PointD& SetTransform(const Matrix& _matrix) { return *this *= _matrix; }

	PointD& operator/=(const Matrix& _matrix) { return *this *= _matrix.Inverse(); }
	PointD operator/(const Matrix& _matrix) const { return *this*_matrix.Inverse(); }

	PointD operator+(const PointD& _point) const { return PointD(x + _point.x, y + _point.y); }
	PointD operator-(const PointD& _point) const { return PointD(x - _point.x, y - _point.y); }
	PointD operator*(const PointD& _point) const { return PointD(x * _point.x, y * _point.y); }
	PointD operator/(const PointD& _point) const { return PointD(x / _point.x, y / _point.y); }
	PointD& operator+=(const PointD& _point) { return *this = *this + _point; }
	PointD& operator-=(const PointD& _point) { return *this = *this - _point; }
	PointD& operator*=(const PointD& _point) { return *this = *this * _point; }
	PointD& operator/=(const PointD& _point) { return *this = *this / _point; }

	PointD operator+(double _value) const { return PointD(x + _value, y + _value); }
	PointD operator-(double _value) const { return PointD(x - _value, y - _value); }
	PointD operator*(double _value) const { return PointD(x * _value, y * _value); }
	PointD operator/(double _value) const { return PointD(x / _value, y / _value); }
	PointD& operator+=(double _value) { return *this = *this + _value; }
	PointD& operator-=(double _value) { return *this = *this - _value; }
	PointD& operator*=(double _value) { return *this = *this * _value; }
	PointD& operator/=(double _value) { return *this = *this / _value; }

	bool operator==(const PointD& _point)const { return x == _point.x&&y == _point.y; }
	bool operator!=(const PointD& _point)const { return x != _point.x || y != _point.y; }
	PointD operator-()const { return PointD(-x, -y); }
	double InnerProduct(const PointD& _point) const { return x * _point.x + y * _point.y; }
	PointD LinePoint(const PointD& _point, double _t) const { return PointD(x + (_point.x - x)*_t, y + (_point.y - y)*_t); }
	PointD& LinePointThis(const PointD& _point, double _t) {
		x += (_point.x - x)*_t;
		y += (_point.y - y)*_t;
		return *this;
	}
};
class PointU {
public:
	union {
		struct { unsigned x, y; };
		struct { unsigned width,height; };
	};
	
	PointU() {}
	PointU(unsigned _x, unsigned _y) :x(_x), y(_y) {}

	unsigned DistancePow(const PointU& _point) const {
		const unsigned a = x - _point.x, b = y - _point.y;
		return (a*a + b * b);
	}

	PointU DirectionPoint(unsigned _distance, const PointU& _direction) const { return *this + (_direction*_distance); }
	PointU& DirectionPointThis(unsigned _distance, const PointU& _direction) { return *this += (_direction*_distance); }

	PointU operator+(const PointU& _point) const { return PointU(x + _point.x, y + _point.y); }
	PointU operator-(const PointU& _point) const { return PointU(x - _point.x, y - _point.y); }
	PointU operator*(const PointU& _point) const { return PointU(x * _point.x, y * _point.y); }
	PointU operator/(const PointU& _point) const { return PointU(x / _point.x, y / _point.y); }
	PointU& operator+=(const PointU& _point) {return *this=*this+_point;}
	PointU& operator-=(const PointU& _point) { return *this = *this - _point; }
	PointU& operator*=(const PointU& _point) { return *this = *this * _point; }
	PointU& operator/=(const PointU& _point) { return *this = *this / _point; }

	PointU operator+(unsigned _value) const { return PointU(x + _value, y + _value); }
	PointU operator-(unsigned _value) const { return PointU(x - _value, y - _value); }
	PointU operator*(unsigned _value) const { return PointU(x * _value, y * _value); }
	PointU operator/(unsigned _value) const { return PointU(x / _value, y / _value); }
	PointU& operator+=(unsigned _value) { return *this = *this + _value; }
	PointU& operator-=(unsigned _value) { return *this = *this - _value; }
	PointU& operator*=(unsigned _value) { return *this = *this * _value; }
	PointU& operator/=(unsigned _value) { return *this = *this / _value; }

	bool operator==(const PointU& _point)const { return x == _point.x&&y == _point.y; }
	bool operator!=(const PointU& _point)const { return x != _point.x || y != _point.y; }
	unsigned InnerProduct(const Point& _point) const { return x * _point.x + y * _point.y; }
	PointU LinePoint(const PointU& _point, unsigned _t) const { return PointU(x + (_point.x - x)*_t, y + (_point.y - y)*_t); }
	PointU& LinePointThis(const PointU& _point, unsigned _t) {
		x += (_point.x - x)*_t;
		y += (_point.y - y)*_t;
		return *this;
	}
};