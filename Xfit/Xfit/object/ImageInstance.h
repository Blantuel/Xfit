#pragma once

#include "Object.h"
#include "../math/Rect.h"

class Sampler;

class ImageInstance :public Object{
#ifdef OPENGL
	struct {
		GLuint posUV;//..PosUV������ 0���� �̹����� Build�ƴ����� �Ǻ���.
		GLuint ins;//ins������ 0���� �̹����� BuildInstance�ƴ����� �Ǻ���.
		GLuint texture;
	}openGL;
#elif VULKAN
#endif
	unsigned insNum,maxInsNum;

	unsigned width, height;
public:
	Sampler * sampler;

	virtual void Draw();

	unsigned GetWidth() const;
	unsigned GetHeight() const;

	void Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs);
	void Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect);
	void BuildInstance(unsigned _maxInsNum, unsigned _insNum,Matrix* _insMatries);
	void UpdateInstance(unsigned _insNum, Matrix* _insMatries);

	ImageInstance();
	virtual ~ImageInstance();
};

