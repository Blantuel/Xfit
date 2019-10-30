#include "LabelButton.h"
#include "SizeLabel.h"
#include <physics/RectHitTest.h>
#include <resource/Vertex.h>
#include <text/Font.h>
#include <system/System.h>

bool LabelButton::ButtonDown(Point _mousePos, void* _data) {
	colorMat.e[15] = 0.5f;
	return true;
}
bool LabelButton::ButtonUp(Point _mousePos, void* _data) {
	colorMat.e[15] = 1.f;
	return true;
}
bool LabelButton::ButtonOut(Point _mousePos, void* _data) {
	colorMat.e[15] = 1.f;
	return true;
}

LabelButton::LabelButton(PosType _posType, SizeLabel* _label, PointF _pos/* = PointF(0.f, 0.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/) :
	Button(new RectHitTest, _pos, PointF(1.f, 1.f), 0.f, System::defaultBlend, nullptr),centerPointPos(_centerPointPos), basePos(_pos),posType(_posType) {
	_label->SizePrepareDraw(WindowRatio() * textPx);

	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), _label->GetWidth(), _label->GetHeight(), _centerPointPos);


	scale = PointF(_label->GetWidth(), _label->GetHeight());
	UpdateMatrix();

	upFrame.frame = _label;
	upFrame.vertex = SelectVertex2D(_centerPointPos);

	upFrame.uv = System::defaultUV;
	upFrame.index = System::defaultIndex;

	SetRectHitTest();
}

void LabelButton::SetRectHitTest() {
	const float x = XToMouseX(pos.x);
	const float y = YToMouseY(pos.y);

	switch (centerPointPos) {
	case CenterPointPos::Center:
		((RectHitTest*)hitTest)->rect = Rect(-(float)upFrame.frame->GetWidth() / 2.f + x, (float)upFrame.frame->GetWidth() / 2.f + x,
			(float)upFrame.frame->GetHeight() / 2.f + y, -(float)upFrame.frame->GetHeight() / 2.f + y);
		break;
	case CenterPointPos::TopLeft:
		((RectHitTest*)hitTest)->rect = Rect(x, (float)upFrame.frame->GetWidth() + x,
			(float)upFrame.frame->GetHeight() + y, y);
		break;
	case CenterPointPos::TopRight:
		((RectHitTest*)hitTest)->rect = Rect(-(float)upFrame.frame->GetWidth() + x, x,
			(float)upFrame.frame->GetHeight() + y, y);
		break;
	case CenterPointPos::Left:
		((RectHitTest*)hitTest)->rect = Rect(x, (float)upFrame.frame->GetWidth() + x,
			(float)upFrame.frame->GetHeight() / 2.f + y, -(float)upFrame.frame->GetHeight() / 2.f + y);
		break;
	case CenterPointPos::Right:
		((RectHitTest*)hitTest)->rect = Rect(-(float)upFrame.frame->GetWidth() + x, x,
			(float)upFrame.frame->GetHeight() / 2.f + y, -(float)upFrame.frame->GetHeight() / 2.f + y);
		break;
	case CenterPointPos::BottomLeft:
		((RectHitTest*)hitTest)->rect = Rect(x, (float)upFrame.frame->GetWidth() + x,
			y, -(float)upFrame.frame->GetHeight() + y);
		break;
	case CenterPointPos::BottomRight:
		((RectHitTest*)hitTest)->rect = Rect(-(float)upFrame.frame->GetWidth() + x, x,
			y, -(float)upFrame.frame->GetHeight() + y);
		break;

	}
}

void LabelButton::Size(bool _scale /*= true*/) {
	float s = WindowRatio();
	if (_scale) {
		((SizeLabel*)upFrame.frame)->SizePrepareDraw(WindowRatio() * textPx);

		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), upFrame.frame->GetWidth(), upFrame.frame->GetHeight(),centerPointPos);
		scale = PointF(upFrame.frame->GetWidth(), upFrame.frame->GetHeight());
	} else {
		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), upFrame.frame->GetWidth(), upFrame.frame->GetHeight(), centerPointPos);
	}
	UpdateMatrix();

	SetRectHitTest();
}

void LabelButton::PrepareDraw() {
	((SizeLabel*)upFrame.frame)->SizePrepareDraw(WindowRatio() * textPx);

	pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), upFrame.frame->GetWidth(), upFrame.frame->GetHeight(), centerPointPos);
	scale = PointF(upFrame.frame->GetWidth(), upFrame.frame->GetHeight());
}

LabelButton::~LabelButton() {
	delete hitTest;
}

SizeLabel* LabelButton::GetLabel()const {
	return (SizeLabel*)upFrame.frame;
}