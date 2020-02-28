#pragma once

#include "Object.h"
#include "../resource/MultiInstanceNode.h"

class Sampler;
class FrameArray;
class Vertex;

template <typename T> class Array;

class ImageMultiInstance :public Object {
#ifdef __ANDROID__
	GLuint ins;//ins변수는 0으로 이미지가 BuildInstance됐는지를 판별함.
#endif

	unsigned insLen, insMaxLen;
public:
	Sampler * sampler;
	FrameArray* frame;
	Vertex* vertex;
	Vertex* uv;
	Matrix colorMat;
	Array<MultiInstanceNode>* nodes;

	virtual void Draw();

	void BuildInstance();
	void UpdateInstance();
	void Delete();

	ImageMultiInstance();
	virtual ~ImageMultiInstance();

	unsigned GetNum()const;
	unsigned GetMaxNum()const;
	bool IsBuild()const;
};

