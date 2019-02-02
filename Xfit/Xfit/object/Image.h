#pragma once


#include "Object.h"
#include "../math/Rect.h"

class Sampler;

class Image :public Object{
#ifdef OPENGL
	GLuint posUV;//..PosUV������ 0���� �̹����� Build�ƴ����� �Ǻ���.
	GLuint texture;
#elif VULKAN
#endif
public:
	Sampler * sampler;

	virtual void Draw();

	void Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs);
	void Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect);
	void BuildEdit(const void* _data, unsigned _width, unsigned _height, unsigned _offsetX = 0, unsigned _offsetY = 0);

	Image();
	virtual ~Image();
};

