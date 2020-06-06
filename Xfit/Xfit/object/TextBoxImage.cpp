#include "TextBoxImage.h"
#include "../resource/Vertex.h"
#include "../text/Font.h"
#include "../system/System.h"

TextBoxImage::TextBoxImage(SizeTextBox* _label, PointF _pos/* = PointF(0.f, 0.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/, float _rotation/* = 0.f*/, bool _isVertical /*= false*/) :
	Image(_pos, PointF(1.f, 1.f), _rotation, System::defaultBlend, System::pointSampler, _label, SelectVertex2D(_centerPointPos), System::defaultUV, System::defaultIndex),
	basePos(_pos), centerPointPos(_centerPointPos), isVertical(_isVertical) {

	Size();
}

SizeTextBox* TextBoxImage::GetTextBox()const {
	return (SizeTextBox*)frame;
}
void TextBoxImage::Size() {
	pos = PixelPerfectPoint(basePos * WindowRatio(), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());
	UpdateMatrix();
}
void TextBoxImage::SetPos(PointF _pos) {
	basePos = _pos;
	pos = PixelPerfectPoint(basePos * WindowRatio(), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void TextBoxImage::SetX(float _x) {
	basePos.x = _x;
	pos = PixelPerfectPoint(basePos * WindowRatio(), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void TextBoxImage::SetY(float _y) {
	basePos.y = _y;
	pos = PixelPerfectPoint(basePos * WindowRatio(), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void TextBoxImage::SetRotation(float _rotation) {
	rotation = _rotation;

	Size();
}
void TextBoxImage::SetTransform(PointF _pos, float _rotation) {
	basePos = _pos;
	rotation = _rotation;
	Size();
}