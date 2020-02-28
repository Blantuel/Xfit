#include "SizeButton.h"
#include "../text/SizeLabel.h"
#include "../physics/RectHitTest.h"
#include "../resource/Vertex.h"
#include "../text/Font.h"
#include "../system/System.h"

SizeButton::SizeButton(PosType _posType, const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame,
	HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/, bool _isVertical /*= false*/) :
	Button(_upFrame, _overFrame, _downFrame, _disableFrame, _hitTest, _pos, PointF(1.f, 1.f), 0.f, _blend, _sampler), isVertical(_isVertical), posType(_posType),
	centerPointPos(_centerPointPos), basePos(_pos), baseScale(_scale) {

	Size();
}



void SizeButton::Size() {
	hitTest->Size();

	if (baseScale * WindowRatio() == PointF(1.f, 1.f)) {
		pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), upFrame.frame->GetWidth(), upFrame.frame->GetHeight(), centerPointPos, isVertical);
	} else {
		pos = basePos * WindowRatioPoint(posType);
	}

	scale = baseScale * WindowRatio();
	UpdateMatrix();
}