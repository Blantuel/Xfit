#pragma once

#include "../math/Point3D.h"

class Blend;
class Object3D;

class Object3D {
public:
	enum Type {
		Type_OBJECT,
		Type_GROUPOBJECT,
		Type_IMAGE,
		Type_IMAGEINSTANCE,
		Type_SHAPE
	};
protected:
	Type type;

	virtual bool Draw(unsigned index, unsigned viewIndex)=0;
public:
	typedef unsigned int Flag;
	enum class Culling {None,Back, Front};
	
	const static Flag Flag_NO_TRANSFORM_CAMERA = 1;
	const static Flag Flag_NO_INSTANCE_RENDER = 2;

	const static Flag Flag_USE_COLOR = 4;
	const static Flag Flag_VERTEX_LIGHT = 8;
	const static Flag Flag_PIXEL_LIGHT = 16;
	const static Flag Flag_USE_NORMAL_TEXTURE = 32;
	const static Flag Flag_NO_COLOR_MATRIX = 64;//Not->COLOR_MATRIX3D

	Blend* blend;
	Culling culling;
	unsigned textureIndex;
	Matrix matrix, colorMatrix;
	unsigned threadId;

	float diffuse, specular, specularReflect,ambient;

	bool visible;
	bool focus;

	Flag flag;

	Object3D(float _x = 0.f, float _y = 0.f, float _z = 0.f, float _rotationX = 0.f, float _rotationY = 0.f, float _rotationZ = 0.f,
		float _scaleX = 1.f, float _scaleY = 1.f, float _scaleZ = 1.f);
};