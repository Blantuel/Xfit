#pragma once

#include "Object.h"
#include "../resource/MultiInstanceNode.h"

class Sampler;
class FrameArray;
class Vertex2D;

template <typename T> class Array;

class ImageMultiInstance :public Object {
	//GLuint ins;//ins������ 0���� �̹����� BuildInstance�ƴ����� �Ǻ���.


	unsigned insLen, insMaxLen;
public:
	Sampler * sampler;
	FrameArray* frame;
	Vertex2D* vertex;
	Vertex2D* uv;
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

