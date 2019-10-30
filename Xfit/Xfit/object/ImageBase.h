#pragma once

#include "MatrixObject.h"
#include "../system/Error.h"

class Sampler;
class Frame;
class Vertex;
class Index;

class ImageBaseError : public Error {
public:
	enum class Code {
		NullVertex,
		NullUV,
		NullFrame,
		NotVertexBuild,
		NotUVBuild,
		NotFrameBuild,
		VertexUVMismatch
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	ImageBaseError(Code _code):code(_code) {}
};

class ImageBase : public MatrixObject {
protected:
	void DrawImage(const Vertex* _vertex,const Vertex* _uv,const Index* _index,const Frame* _frame);
public:
	Sampler * sampler;
	Matrix colorMat;

	ImageBase();
	ImageBase(PointF _pos,PointF _scale,float _rotation,Blend* _blend,Sampler* _sampler);
};