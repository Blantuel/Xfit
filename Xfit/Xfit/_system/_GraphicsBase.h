#pragma once

#include "../math/Point.h"
#include "../math/Point3D.h"
#include "../math/Point3Dw.h"

class Camera3D;
class Camera;

struct Light;
class Frame;
class FrameArray;
class Sampler;
class Blend;
class Vertices;
class Object3D;
class Viewport;


namespace _System {
#pragma pack(push,1)
	struct ObjectBaseStruct {
		PointF pos;
		PointF size;
		float rotation;
		PointF centerPoint;
		float unused;
	};
	struct MatrixStruct {
		Matrix worldMatrix;
		Matrix viewMatrix;
		Matrix projectionMatrix;
		unsigned int flag;
		Point3DF view;
	};
	struct ColorStruct {
		Matrix colorMatrix;
		Point3DF view;
		unsigned int flag;
		float bloomCutOff;
		float bloomMul;
		float bloomAdd;
		float bloomPow;
	};
	struct LightStruct
	{
		float diffuse;
		float specular;
		float specularReflect;
		float ambient;
		unsigned length;
		Point3DF reversed;
	};
	struct PosUV2D {
		PointF pos;
		PointF UV;
	};
	struct Vertex3D {
		Point3DF position;
	};
	struct Color {
		Point3DwF position;
	};
#pragma pack(pop)


	inline PointF defaultUV2D[4] = { PointF(0.f,0.f),PointF(1.f,0.f) ,PointF(0.f,1.f) ,PointF(1.f,1.f) };
}