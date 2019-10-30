#pragma once

#include "Object.h"
#include "../math/Matrix.h"
#include "../system/Error.h"

class Sampler;
class Frame;
class Vertex2D;

template <typename T> class Array;

class ImageInstanceError : public Error {
public:
	enum class Code {
		NullVertex,
		NullUV,
		NullFrame,
		NullSampler,
		NullNodes,
		NotVertexBuild,
		NotUVBuild,
		NotFrameBuild,
		VertexUVMismatch,
		NotBuild,
		AlreadyBuild,
		TooBigNode
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	ImageInstanceError(Code _code):code(_code){}
};

class ImageInstance :public Object {
	GLuint ins;//ins������ 0���� �̹����� BuildInstance�ƴ����� �Ǻ���.

	unsigned insLen, insMaxLen;
public:
	Sampler * sampler;
	Frame* frame;
	Vertex2D* vertex;
	Vertex2D* uv;
	Matrix colorMat;
	Array<Matrix>* nodes;

	virtual void Draw();

	void BuildInstance();
	void UpdateInstance();
	void Delete();

	ImageInstance();
	virtual ~ImageInstance();

	unsigned GetNum()const;
	unsigned GetMaxNum()const;
	bool IsBuild()const;
};

