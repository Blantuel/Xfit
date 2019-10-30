#pragma once

#include "ImageBase.h"
#include "../resource/AnimateBase.h"

class AnimateFrame;


template <typename T> class Array;

class AnimateObject : public ImageBase, public AnimateBase {
public:
	Array<AnimateFrame>* frames;

	AnimateObject();
	AnimateObject(PointF _pos,PointF _scale,float _rotation,Blend* _blend,Sampler* _sampler,Array<AnimateFrame>* _frames);

	virtual void Draw();

	virtual unsigned GetTotalFrame();
};
