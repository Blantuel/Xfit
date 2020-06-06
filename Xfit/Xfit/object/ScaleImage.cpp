#include "ScaleImage.h"
#include "../resource/Frame.h"
#include "../resource/Vertex.h"
#include "../system/System.h"

ScaleImage::ScaleImage(PointF _pos, PointF _scale, float _rotation, CenterPointPos _centerPointPos, bool _isVertical,
	Blend* _blend, Sampler* _sampler, Frame* _frame, Vertex* _uv, Index* _index):
	Image(_pos * WindowRatio(), _scale * PointF((float)_frame->GetWidth(), (float)_frame->GetHeight()) * WindowRatio(),
		_rotation, _blend, _sampler, _frame ,SelectVertex2D(_centerPointPos), _uv, _index),
	basePos(_pos), baseScale(_scale), centerPointPos(_centerPointPos), isVertical(_isVertical){
}
void ScaleImage::Size() {
	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatio(), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatio();
	}

	scale = baseScale * PointF((float)frame->GetWidth(), (float)frame->GetHeight()) * WindowRatio();
	UpdateMatrix();
}
void ScaleImage::SetPos(PointF _pos) {
	basePos = _pos;
	Size();
}
void ScaleImage::SetX(float _x) {
	basePos.x = _x;
	Size();
}
void ScaleImage::SetY(float _y) {
	basePos.y = _y;
	Size();
}

void ScaleImage::SetScale(PointF _scale) {
	baseScale = _scale;
	Size();
}

void ScaleImage::SetScaleX(float _scaleX) {
	baseScale.x = _scaleX;
	Size();
}

void ScaleImage::SetScaleY(float _scaleY) {
	baseScale.y = _scaleY;
	Size();
}

void ScaleImage::SetRotation(float _rotation) {
	rotation = _rotation;
	Size();
}
void ScaleImage::SetTransform(PointF _pos, PointF _scale, float _rotation) {
	basePos = _pos;
	baseScale = _scale;
	rotation = _rotation;
	Size();
}