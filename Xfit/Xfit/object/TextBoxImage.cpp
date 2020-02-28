#include "TextBoxImage.h"
#include "../text/SizeTextBox.h"
#include "../resource/Vertex.h"
#include "../text/Font.h"
#include "../system/System.h"

TextBoxImage::TextBoxImage(PosType _posType, SizeTextBox* _label, PointF _pos/* = PointF(0.f, 0.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/, float _rotation/* = 0.f*/, bool _isVertical /*= false*/, float _scale /*= 1.f*/) :
	Image(_pos, PointF(1.f, 1.f), _rotation, System::defaultBlend, System::pointSampler, _label, SelectVertex2D(_centerPointPos), System::defaultUV, System::defaultIndex),
	basePos(_pos), centerPointPos(_centerPointPos), posType(_posType), isVertical(_isVertical), baseScale(_scale) {

	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), _centerPointPos, _isVertical);


	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());
	UpdateMatrix();
}

void TextBoxImage::SetScale(float _scale) {
	baseScale = _scale;
	Size();
}

SizeTextBox* TextBoxImage::GetTextBox()const {
	return (SizeTextBox*)frame;
}
void TextBoxImage::Size(float _scaleRatio /*= 1.f*/) {
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());
	UpdateMatrix();
}
void TextBoxImage::PrepareDraw(float _scaleRatio /*= 1.f*/) {
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());
	UpdateMatrix();
}
void TextBoxImage::SetPos(PointF _pos) {
	basePos = _pos;
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void TextBoxImage::SetX(float _x) {
	basePos.x = _x;
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void TextBoxImage::SetY(float _y) {
	basePos.y = _y;
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
//virtual void SetRotation(float _rotation);
void TextBoxImage::SetTransform(PointF _pos, float _scale, float _rotation) {
	basePos = _pos;
	baseScale = _scale;
	rotation = _rotation;
	Size();
}