#include "AnimateObject.h"
#include "../resource/AnimateFrame.h"
#include "../data/Array.h"
#include "../file/File.h"
#include "../resource/Frame.h"
#include "../resource/Vertex.h"
#include "../data/Compress.h"
#include "../system/System.h"

AnimateObject::AnimateObject(PointF _pos,PointF _scale,float _rotation,Blend* _blend,Sampler* _sampler,Array<AnimateFrame>* _frames):ImageBase(_pos,_scale,_rotation,_blend,_sampler),frames(_frames) {}
AnimateObject::AnimateObject():frames(nullptr) {}
unsigned AnimateObject::GetTotalFrame() {
#ifdef _DEBUG
	if(!frames)throw AnimateBaseError(AnimateBaseError::Code::NullFrames);
#endif
	return frames->Size();
}
void AnimateObject::Draw() {
	if(visible) {
		Object::Draw();

		DrawImage((*frames)[cFrame].vertex,(*frames)[cFrame].uv, (*frames)[cFrame].index,(*frames)[cFrame].frame);
	}
}