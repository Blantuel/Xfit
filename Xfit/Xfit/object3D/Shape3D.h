#pragma once


#include "Object3D.h"

class Vertices;

class Shape3D :public Object3D {
protected:
	virtual bool Draw(unsigned index, unsigned viewIndex);
public:
	Vertices* vertices;

	Shape3D(float _x = 0.f, float _y = 0.f, float _z = 0.f, float _rotationX = 0.f, float _rotationY = 0.f, float _rotationZ = 0.f, float _scaleX = 1.f, float _scaleY = 1.f, float _scaleZ = 1.f)
		:Object3D(_x, _y, _z, _rotationX, _rotationY, _rotationZ, _scaleX, _scaleY, _scaleZ), vertices(nullptr) {
		type = Type_SHAPE;
	}
	Shape3D(Vertices* _vertices, float _x = 0.f, float _y = 0.f, float _z = 0.f, float _rotationX = 0.f, float _rotationY = 0.f, float _rotationZ = 0.f, float _scaleX = 1.f, float _scaleY = 1.f, float _scaleZ = 1.f)
		:Object3D(_x, _y, _z, _rotationX, _rotationY, _rotationZ, _scaleX, _scaleY, _scaleZ), vertices(_vertices) {
		type = Type_SHAPE;
	}
};
