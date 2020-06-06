#include "LabelImage.h"
#include "../resource/Vertex.h"
#include "../text/Font.h"
#include "../system/System.h"

LabelImage::LabelImage(SizeLabel* _label, PointF _pos/* = PointF(0.f, 0.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/,  float _rotation/* = 0.f*/, bool _isVertical /*= false*/):
	Image(_pos, PointF(1.f,1.f),_rotation, System::defaultBlend,System::pointSampler,_label,SelectVertex2D(_centerPointPos),System::defaultUV,System::defaultIndex),
	basePos(_pos), centerPointPos(_centerPointPos),isVertical(_isVertical){

	Size();
}
void LabelImage::SetPos(PointF _pos) {
	basePos = _pos;
	pos = PixelPerfectPoint(basePos * WindowRatio(), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void LabelImage::SetX(float _x) {
	basePos.x = _x;
	pos = PixelPerfectPoint(basePos * WindowRatio(), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void LabelImage::SetY(float _y) {
	basePos.y = _y;
	pos = PixelPerfectPoint(basePos * WindowRatio(), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void LabelImage::SetRotation(float _rotation) {
	rotation = _rotation;
	
	Size();
}
void LabelImage::SetTransform(PointF _pos, float _rotation) {
	basePos = _pos;
	rotation = _rotation;
	Size();
}
SizeLabel* LabelImage::GetLabel()const {
	return (SizeLabel*)frame;
}
void LabelImage::Size() {
	pos = PixelPerfectPoint(basePos * WindowRatio(), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());


	UpdateMatrix();
}

void LabelImage::MakeLabelImageText(LabelImage* _labelImage, const wchar_t* _text, unsigned _color /*= 0*/, unsigned _fontSize /*= 0*/) {
	_labelImage->visible = true;
	_labelImage->GetLabel()->text = _text;
	_labelImage->GetLabel()->colors[0].color = _color;
	if (_fontSize != 0)_labelImage->GetLabel()->baseSizes[0] = _fontSize;
	LABELIMAGE_SIZE(_labelImage);
}
