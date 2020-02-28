#include "Image.h"
#include "../data/Array.h"
#include "../file/File.h"
#include "../data/Compress.h"
#include "../resource/Frame.h"
#include "../system/System.h"
#include "../resource/Vertex.h"

Image::Image(PointF _pos,PointF _scale,float _rotation,Blend* _blend,Sampler* _sampler,Frame* _frame,Vertex* _vertex,Vertex* _uv, Index* _index):
	ImageBase(_pos,_scale,_rotation,_blend,_sampler),frame(_frame),vertex(_vertex),uv(_uv),index(_index) {}
Image::Image():uv(System::defaultUV),vertex(System::defaultVertex2D),frame(nullptr),index(System::defaultIndex) {}

void Image::Draw() {
	if(visible) {
		Object::Draw();

		DrawImage(vertex, uv, index, frame);
	}
}

RectF Image::GetRect()const {
	return RectF(pos.x - scale.x / 2.f, pos.x + scale.x / 2.f, pos.y + scale.y / 2.f, pos.y - scale.y / 2.f);
}