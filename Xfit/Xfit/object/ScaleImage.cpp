#include "ScaleImage.h"
#include "../resource/Frame.h"
#include "../resource/Vertex.h"
#include "../system/System.h"

ScaleImage::ScaleImage(PosType _posType, PointF _pos, PointF _scale, float _rotation, CenterPointPos _centerPointPos, bool _isVertical,
	Blend* _blend, Sampler* _sampler, Frame* _frame, Vertex* _uv, Index* _index):
	Image(_pos, PointF(1.f,1.f), _rotation, _blend, _sampler, _frame ,SelectVertex2D(_centerPointPos), _uv, _index), 
	basePos(_pos), baseScale(_scale), centerPointPos(_centerPointPos),posType(_posType), isVertical(_isVertical){

	Size();
}
void ScaleImage::Size(float _scaleRatio /*= 1.f*/) {
	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatioPoint(posType);
	}

	scale = baseScale * PointF((float)frame->GetWidth(), (float)frame->GetHeight()) * WindowRatio();
	UpdateMatrix();
}
void ScaleImage::SetPos(PointF _pos) {
	basePos = _pos;
	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatioPoint(posType);
	}

	UpdateMatrix();
}
void ScaleImage::SetX(float _x) {
	basePos.x = _x;
	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatioPoint(posType);
	}

	UpdateMatrix();
}
void ScaleImage::SetY(float _y) {
	basePos.y = _y;
	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatioPoint(posType);
	}

	UpdateMatrix();
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

//virtual void SetRotation(float _rotation);
void ScaleImage::SetTransform(PointF _pos, PointF _scale, float _rotation) {
	basePos = _pos;
	baseScale = _scale;
	rotation = _rotation;
	Size();
}