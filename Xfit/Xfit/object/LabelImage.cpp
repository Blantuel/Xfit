#include "LabelImage.h"
#include "../text/SizeLabel.h"
#include "../resource/Vertex.h"
#include "../text/Font.h"
#include "../system/System.h"

LabelImage::LabelImage(PosType _posType, SizeLabel* _label, PointF _pos/* = PointF(0.f, 0.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/,  float _rotation/* = 0.f*/, bool _isVertical /*= false*/, float _scale /*= 1.f*/):
	Image(_pos, PointF(1.f,1.f),_rotation, System::defaultBlend,System::pointSampler,_label,SelectVertex2D(_centerPointPos),System::defaultUV,System::defaultIndex),baseScale(_scale),
	basePos(_pos), centerPointPos(_centerPointPos),posType(_posType), isVertical(_isVertical){

	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), _centerPointPos, _isVertical);
	

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());
	UpdateMatrix();
}
void LabelImage::SetPos(PointF _pos) {
	basePos = _pos;
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void LabelImage::SetX(float _x) {
	basePos.x = _x;
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void LabelImage::SetY(float _y) {
	basePos.y = _y;
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void LabelImage::SetTransform(PointF _pos, float _scale, float _rotation) {
	basePos = _pos;
	baseScale = _scale;
	rotation = _rotation;
	Size();
}
void LabelImage::SetScale(float _scale) {
	baseScale = _scale;
	Size();
}
SizeLabel* LabelImage::GetLabel()const {
	return (SizeLabel*)frame;
}
void LabelImage::Size() {
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());


	UpdateMatrix();
}