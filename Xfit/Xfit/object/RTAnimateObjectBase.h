#pragma once

#include "MatrixObject.h"
#include "../resource/AnimateBase.h"

class Sampler;
class Frame;
class Vertex;

class RTAnimateObjectBaseError : public Error {
public:
	enum class Code {
		NullVertex,
		NullUV,
		NotVertexBuild,
		NotUVBuild,
		VertexUVMismatch,
		NotBuild,
		AlreadyBuild,
		ZeroWidth,
		ZeroHeight,
		InvalidFormat,
		NullData,
		ZeroSize
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code _code):code(_code){}
};
class RTAnimateObjectBase : public MatrixObject, public AnimateBase {
protected:
	GLuint texture;

	void DrawImage(const Vertex* _vertex,const Vertex* _uv);
public:
	Matrix colorMat;
	Sampler* sampler;

	void Delete();

	RTAnimateObjectBase(PointF _pos,PointF _scale,float _rotation,Blend* _blend,Sampler* _sampler);
	RTAnimateObjectBase();
	~RTAnimateObjectBase();
};