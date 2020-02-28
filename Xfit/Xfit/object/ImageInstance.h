#pragma once

#include "Object.h"
#include "../math/Matrix.h"
#include "../system/Error.h"

class Sampler;
class Frame;
class Vertex;

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
#ifdef __ANDROID__
	GLuint ins;//ins변수는 0으로 이미지가 BuildInstance됐는지를 판별함.
#endif

	unsigned insLen, insMaxLen;
public:
	Sampler * sampler;
	Frame* frame;
	Vertex* vertex;
	Vertex* uv;
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

