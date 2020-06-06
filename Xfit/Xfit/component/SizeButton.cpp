#include "SizeButton.h"
#include "../text/SizeLabel.h"
#include "../physics/RectHitTest.h"
#include "../resource/Vertex.h"
#include "../text/Font.h"
#include "../system/System.h"

#include "../physics/SizeRectHitTest.h"

SizeButton::SizeButton(const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame,
	HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/, bool _isVertical /*= false*/) :
	Button(_upFrame, _overFrame, _downFrame, _disableFrame, _hitTest, _pos, PointF(1.f, 1.f), 0.f, _blend, _sampler), isVertical(_isVertical),
	centerPointPos(_centerPointPos), basePos(_pos), baseScale(_scale) {

	Size();
}



void SizeButton::Size() {
	hitTest->Size();

	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatio(), upFrame.frame->GetWidth(), upFrame.frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatio();
	}

	scale = baseScale * WindowRatio();
	UpdateMatrix();
}


SizeButton* MakeSizeButton(Frame* _upFrame, Frame* _overFrame, Frame* _downFrame, Vertex* _vertex, PointF _pos, bool _noUseMipmap/* = false*/, PointF _scale /*= PointF(1.f, 1.f)*/) {
	ButtonFrame upFrame(_upFrame, _vertex, System::defaultUV);
	ButtonFrame overFrame(_overFrame, _vertex, System::defaultUV);
	ButtonFrame downFrame(_downFrame, _vertex, System::defaultUV);
	ButtonFrame disableFrame;
	upFrame.index = System::defaultIndex;
	overFrame.index = System::defaultIndex;
	downFrame.index = System::defaultIndex;

	SizeRectHitTest* buttonSizeRectHitTest = new SizeRectHitTest;
	buttonSizeRectHitTest->baseRect = RectF::MakeRect(_pos.x, _pos.y, (float)_upFrame->GetWidth() * _scale.x, (float)_upFrame->GetHeight() * _scale.y);

	return new SizeButton(upFrame, overFrame, downFrame, disableFrame, buttonSizeRectHitTest, _pos, _scale, 0.f,
		System::defaultBlend, System::defaultSampler);
}


void SizeButton::SetPos(PointF _pos) {
	basePos = _pos;
	Size();
}
void SizeButton::SetX(float _x) {
	basePos.x = _x;
	Size();
}
void SizeButton::SetY(float _y) {
	basePos.y = _y;
	Size();
}