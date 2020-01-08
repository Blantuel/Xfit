#include "ScaleImage.h"
#include "../resource/Frame.h"
#include "../resource/Vertex.h"
#include "../system/System.h"

ScaleImage::ScaleImage(PosType _posType, Frame* _frame, PointF _pos/* = PointF(0.f, 0.f)*/, PointF _scale /*= PointF(1.f, 1.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/,  float _rotation/* = 0.f*/, bool _isVertical /*= false*/):
	Image(_pos, PointF(1.f,1.f),_rotation, System::defaultBlend,nullptr,_frame ,SelectVertex2D(_centerPointPos),System::defaultUV,System::defaultIndex), 
	basePos(_pos), baseScale(_scale), centerPointPos(_centerPointPos),posType(_posType), isVertical(_isVertical){
	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatioPoint(posType);
	}

	scale = baseScale * PointF((float)frame->GetWidth(), (float)frame->GetHeight()) * WindowRatio();
	UpdateMatrix();
}
void ScaleImage::Size(float _scaleRatio /*= 1.f*/) {
	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatioPoint(posType);
	}

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight()) * WindowRatio();
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
	scale = baseScale * PointF((float)frame->GetWidth(), (float)frame->GetHeight()) * WindowRatio();
	UpdateMatrix();
}

void ScaleImage::SetScaleX(float _scaleX) {
	baseScale.x = _scaleX;
	scale = baseScale * PointF((float)frame->GetWidth(), (float)frame->GetHeight()) * WindowRatio();
	UpdateMatrix();
}

void ScaleImage::SetScaleY(float _scaleY) {
	baseScale.y = _scaleY;
	scale = baseScale * PointF((float)frame->GetWidth(), (float)frame->GetHeight()) * WindowRatio();
	UpdateMatrix();
}

//virtual void SetRotation(float _rotation);
void ScaleImage::SetTransform(PointF _pos, PointF _scale, float _rotation) {
	baseScale = _scale;
	scale = baseScale * PointF((float)frame->GetWidth(), (float)frame->GetHeight()) * WindowRatio();

	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatioPoint(posType);
	}
	rotation = _rotation;

	UpdateMatrix();
}
