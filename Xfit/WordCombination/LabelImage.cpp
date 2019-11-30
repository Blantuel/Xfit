#include "LabelImage.h"
#include "SizeLabel.h"
#include <resource/Vertex.h>
#include <text/Font.h>
#include <system/System.h>

LabelImage::LabelImage(PosType _posType, SizeLabel* _label, PointF _pos/* = PointF(0.f, 0.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/,  float _rotation/* = 0.f*/, bool _isVertical /*= false*/):
	Image(_pos, PointF(1.f,1.f),_rotation, System::defaultBlend,nullptr,_label,SelectVertex2D(_centerPointPos),System::defaultUV,System::defaultIndex), 
	basePos(_pos), centerPointPos(_centerPointPos),posType(_posType), isVertical(_isVertical){

	_label->SizePrepareDraw(WindowRatio() * textPx);

	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), _centerPointPos, _isVertical);
	

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());
	UpdateMatrix();
}
SizeLabel* LabelImage::GetLabel()const {
	return (SizeLabel*)frame;
}
void LabelImage::Size() {
	((SizeLabel*)frame)->SizePrepareDraw(WindowRatio() * textPx);

	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());
	UpdateMatrix();
}
void LabelImage::SetPos(PointF _pos) {
	basePos = _pos;
	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);
	UpdateMatrix();
}
void LabelImage::PrepareDraw() {
	((SizeLabel*)frame)->PrepareDraw();

	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), frame->GetWidth(), frame->GetHeight(), centerPointPos, isVertical);

	scale = PointF((float)frame->GetWidth(), (float)frame->GetHeight());
	UpdateMatrix();
}