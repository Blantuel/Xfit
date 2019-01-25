#pragma once


#include "Object.h"
#include "../math/Rect.h"

class Sampler;

class Image :public Object{
#ifdef OPENGL
	struct {
		GLuint posUV;//..PosUV변수는 0으로 이미지가 Build됐는지를 판별함.
		GLuint texture;
	}openGL;
#elif VULKAN
#endif
public:
	Sampler * sampler;

	virtual void Draw();

	void Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs);
	void Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect);

	Image();
	virtual ~Image();
};

