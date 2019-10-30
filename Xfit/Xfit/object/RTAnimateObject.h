#pragma once

#include "RTAnimateObjectBase.h"
#include "../resource/FrameFormat.h"
#include "../resource/FrameCompressFormat.h"

class Sampler;
class RTAnimateFrame;

template <typename T> class Array;

class RTAnimateObject : public RTAnimateObjectBase {
protected:
	unsigned width,height;

	GLenum fmt;
	bool isCompressed;
public:
	Array<RTAnimateFrame>* frames;

	void Build(unsigned _width,unsigned _height,FrameFormat _format = FrameFormat::RGBA);//�����Ӹ��� �̹��� ũ�Ⱑ ���� ���
	void BuildCompress(unsigned _width,unsigned _height,FrameCompressFormat _format);//�����Ӹ��� �̹��� ũ�Ⱑ ���� ���

	unsigned GetWidth()const;
	unsigned GetHeight()const;

	bool IsCompressed()const;

	virtual void Draw();

	RTAnimateObject(PointF _pos,PointF _scale,float _rotation,Blend* _blend,Sampler* _sampler,Array<RTAnimateFrame>* _frames);
	RTAnimateObject();

	virtual unsigned GetTotalFrame();
};