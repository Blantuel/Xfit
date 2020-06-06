#pragma once

#include "SizeMatrixObject.h"
#include "../system/Error.h"
#include "../data/Array.h"

class Sampler;
class Frame;
class Vertex;
class Index;


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

#pragma pack(push,1)
struct ImageInstanceNode {
	Matrix mat;
	Matrix colorMat;
};
#pragma pack(pop)

class ImageInstance :public SizeMatrixObject {
#ifdef _WIN32
	ID3D11Buffer* instanceBuffer;
#elif __ANDROID__
	GLuint ins;//ins변수는 0으로 이미지가 BuildInstance됐는지를 판별함.
#endif
	unsigned num;
	unsigned maxNum;
public:
	PointF basePos;
	PointF baseScale;

	Sampler * sampler;
	Frame* frame;
	Vertex* vertex;
	Vertex* uv;
	Index* index;
	Array<ImageInstanceNode> nodes;

	virtual void Draw();

	void BuildInstance();
	void UpdateInstance(unsigned _start = 0, unsigned _num = UINT_MAX);
	void Delete();

	ImageInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, Frame* _frame, Vertex* _vertex, Vertex* _uv, Index* _index);
	virtual ~ImageInstance();

	unsigned GetNum()const;
	unsigned GetMaxNum()const;
	bool IsBuild()const;
};

