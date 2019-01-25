#pragma once

#include "Matrix.h"

class Point3DF {
public:
	float x, y, z;
	Point3DF() {}
	Point3DF(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
	float Distance(const Point3DF& _point) const {
		const float a = x - _point.x, b = y - _point.y, c = z - _point.z;
		return sqrtf((a*a + b * b + c * c));
	}
	float DistancePow(const Point3DF& _point) const {
		const float a = x - _point.x, b = y - _point.y, c = z - _point.z;
		return a * a + b * b + c * c;
	}

	Point3DF AnglePoint3D(float _distance, float _rotationX = 0, float _rotationY = 0, float _rotationZ = 0)const {
		const Point3DF point(cosf(_rotationY*Math::DIVPI_180)*cosf(_rotationZ*Math::DIVPI_180), sinf(_rotationX*Math::DIVPI_180)*sinf(_rotationZ*Math::DIVPI_180), cosf(_rotationX*Math::DIVPI_180)*sinf(_rotationY*Math::DIVPI_180));
		return *this + (point*_distance);
	}
	Point3DF& AnglePoint3DThis(float _distance, float _rotationX = 0, float _rotationY = 0, float _rotationZ = 0) {
		const Point3DF point(cosf(_rotationY*Math::DIVPI_180)*cosf(_rotationZ*Math::DIVPI_180), sinf(_rotationX*Math::DIVPI_180)*sinf(_rotationZ*Math::DIVPI_180), cosf(_rotationX*Math::DIVPI_180)*sinf(_rotationY*Math::DIVPI_180));
		return *this += (point*_distance);
	}
	float GetAngleX(const Point3DF& _point) const { return atan2(_point.y - y, _point.z - z)*Math::DIV180_PI + 180; }
	float GetAngleY(const Point3DF& _point) const { return atan2(_point.x - x, _point.z - z)*Math::DIV180_PI + 180; }
	float GetAngleZ(const Point3DF& _point) const { return atan2(_point.y - y, _point.x - x)*Math::DIV180_PI + 180; }

	Point3DF DirectionPoint3D(float _distance, const Point3DF& _direction)const { return *this + (_direction*_distance); }
	Point3DF& DirectionPoint3DThis(float _distance, const Point3DF& _direction) { return *this += (_direction*_distance); }
	Point3DF& SetTransform(const Matrix& _matrix) { return *this *= _matrix; }
	Point3DF& operator*=(const Matrix& _matrix) {
		x = x * _matrix._11 + y * _matrix._21 + z * _matrix._31 + _matrix._41;
		y = x * _matrix._12 + y * _matrix._22 + z * _matrix._32 + _matrix._42;
		z = x * _matrix._13 + y * _matrix._23 + z * _matrix._33 + _matrix._43;
		return *this;
	}
	Point3DF operator*(const Matrix& _matrix) const {
		return Point3DF(x*_matrix._11 + y * _matrix._21 + z * _matrix._31 + _matrix._41,
			x*_matrix._12 + y * _matrix._22 + z * _matrix._32 + _matrix._42,
			x*_matrix._13 + y * _matrix._23 + z * _matrix._33 + _matrix._43);
	}
	Point3DF operator/(const Matrix& matrix) const { return *this*matrix.Inverse(); }
	Point3DF& operator/=(const Matrix& matrix) { return *this *= matrix.Inverse(); }

	Point3DF& operator+=(const Point3DF& point) {
		x += point.x;
		y += point.y;
		z += point.z;
		return *this;
	}
	Point3DF& operator-=(const Point3DF& point) {
		x -= point.x;
		y -= point.y;
		z -= point.z;
		return *this;
	}
	Point3DF& operator*=(const Point3DF& point) {
		x *= point.x;
		y *= point.y;
		z *= point.z;
		return *this;
	}
	Point3DF& operator/=(const Point3DF& point) {
		x /= point.x;
		y /= point.y;
		z /= point.z;
		return *this;
	}
	Point3DF operator+(const Point3DF& point) const { return Point3DF(x + point.x, y + point.y, z + point.z); }
	Point3DF operator-(const Point3DF& point) const { return Point3DF(x - point.x, y - point.y, z - point.z); }
	Point3DF operator*(const Point3DF& point) const { return Point3DF(x * point.x, y * point.y, z * point.z); }
	Point3DF operator/(const Point3DF& point) const { return Point3DF(x / point.x, y / point.y, z / point.z); }

	Point3DF& NormalizeThis() { return *this /= Distance(Point3DF(0, 0, 0)); }
	Point3DF Normalize() const { return *this / Distance(Point3DF(0, 0, 0)); }

	Point3DF& operator+=(float value) {
		x += value;
		y += value;
		z += value;
		return *this;
	}
	Point3DF& operator-=(float value) {
		x -= value;
		y -= value;
		z -= value;
		return *this;
	}
	Point3DF& operator*=(float value) {
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}
	Point3DF& operator/=(float value) {
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}
	Point3DF operator+(float value) const { return Point3DF(x + value, y + value, z + value); }
	Point3DF operator-(float value) const { return Point3DF(x - value, y - value, z - value); }
	Point3DF operator*(float value) const { return Point3DF(x*value, y*value, z*value); }
	Point3DF operator/(float value) const { return Point3DF(x / value, y / value, z / value); }

	bool operator==(const Point3DF& point)const { return x == point.x&&y == point.y&&z == point.z; }
	bool operator!=(const Point3DF& point)const { return x != point.x || y != point.y || z != point.z; }
	Point3DF operator-()const { return Point3DF(-x, -y, -z); }
	float InnerProduct(const Point3DF& point) const { return x * point.x + y * point.y + z * point.z; }

	Point3DF& OuterProductThis(const Point3DF& point) {
		x = y * point.z - z * point.y;
		y = z * point.x - x * point.z;
		z = x * point.y - y * point.x;
		return *this;
	}
	Point3DF& LinePoint3DThis(const Point3DF& point, float t) {
		x += (point.x - x)*t;
		y += (point.y - y)*t;
		z += (point.z - z)*t;
		return *this;
	}
	const Point3DF OuterProduct(const Point3DF& point) const { return Point3DF(y*point.z - z * point.y, z*point.x - x * point.z, x*point.y - y * point.x); }
	const Point3DF LinePoint3D(const Point3DF& point, float t) const { return Point3DF(x + (point.x - x)*t, y + (point.y - y)*t, z + (point.z - z)*t); }
};

