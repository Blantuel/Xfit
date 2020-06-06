#pragma once

#include "ImageBase.h"


class Vertex;
class Index;

template <typename T> class Array;

class ImageError : public Error {
public:
	enum class Code {
		InvalidFile
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	ImageError(Code _code):code(_code) {}
};
class Image :public ImageBase {
public:
	Frame* frame;
	Vertex* vertex;
	Vertex* uv;
	Index* index;

	virtual void Draw();

	Image();
	Image(PointF _pos,PointF _scale,float _rotation, Blend* _blend,Sampler* _sampler,Frame* _frame,Vertex* _vertex,Vertex* _uv,Index* _index);

	virtual RectF GetRect()const;
	virtual RectF GetRect90()const;
};

