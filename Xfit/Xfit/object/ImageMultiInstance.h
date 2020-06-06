#pragma once

#include "SizeMatrixObject.h"
#include "../resource/MultiInstanceNode.h"
#include "../system/Error.h"
#include "../data/Array.h"

class Sampler;
class FrameArray;
class Vertex;
class Index;


class ImageMultiInstanceError : public Error {
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
	Code GetCode()const { return code; }
	ImageMultiInstanceError(Code _code) :code(_code) {}
};


class ImageMultiInstance :public SizeMatrixObject {
#ifdef __ANDROID__
	GLuint ins;//ins변수는 0으로 이미지가 BuildInstance됐는지를 판별함.
#elif _WIN32
	ID3D11Buffer* instanceBuffer;
#endif

	unsigned num, maxNum;
public:
	Sampler * sampler;
	FrameArray* frame;
	Vertex* vertex;
	Vertex* uv;
	Index* index;
	Array<MultiInstanceNode> nodes;

	virtual void Draw();

	void BuildInstance();
	void UpdateInstance(unsigned _start = 0, unsigned _num = UINT_MAX);
	void Delete();

	ImageMultiInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, FrameArray* _frame, Vertex* _vertex, Vertex* _uv, Index* _index);
	virtual ~ImageMultiInstance();

	unsigned GetNum()const;
	unsigned GetMaxNum()const;
	bool IsBuild()const;
};

