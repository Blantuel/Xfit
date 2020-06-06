#include "FadeLabelButton.h"
#include "../physics/RectHitTest.h"
#include "../text/Label.h"
#include "../resource/Vertex.h"
#include "../text/Font.h"
#include "../system/System.h"
#include "../text/SizeLabel.h"


void FadeLabelButton::Remake() {
	visible = true;
	Disable(false);
	SizeLabel* label = (SizeLabel*)upFrame.frame;
	label->SizePrepareDraw(WindowRatio());

	fadeFrame = 0;

	colorMat.e[15] = 1.f;

	pos = PixelPerfectPoint(basePos * WindowRatio(), upFrame.frame->GetWidth(), upFrame.frame->GetHeight(), centerPointPos);

	scale = PointF((float)label->GetWidth(), (float)label->GetHeight());

	fading = false;

	UpdateMatrix();
}

bool FadeLabelButton::Update() {
	const bool result = Button::Update();
	if (fading) {
		const unsigned totalFrame = (unsigned)(1.f/System::GetDeltaTime()*fadeTime);
		if (fadeFrame >= totalFrame) {
			fading = false;
			visible = false;
			Disable(true);
			if(faded)faded(this, nullptr);
		} else {
			SizeLabel* label = (SizeLabel*)upFrame.frame;

			const float ratio = (float)fadeFrame / (float)totalFrame;
			const float mul = ratio * (mag - 1.f) + 1.f;
			
			label->SizePrepareDraw(WindowRatio() * mul);

			colorMat.e[15] = 1.f - ratio;

			pos = PixelPerfectPoint(basePos * WindowRatio(), upFrame.frame->GetWidth(), upFrame.frame->GetHeight(), centerPointPos);

			scale = PointF((float)label->GetWidth(), (float)label->GetHeight());

			UpdateMatrix();

			fadeFrame++;
		}
		return true;
	}
	return result;
}
void FadeLabelButton::Size() {
	if (fading) LabelButton::Size();
	else LabelButton::Size();
}
bool FadeLabelButton::ButtonDown(PointF _mousePos, void* _data) {
	if (!fading) {
		fading = true;
		fadeFrame = 0;
	}
	return false;
}
FadeLabelButton::FadeLabelButton(SizeLabel* _label, PointF _pos/* = PointF(0.f, 0.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/,
	float _mag/* = 2.f*/, float _fadeTime /*= 0.5f*/) :
	LabelButton(_label, _pos, _centerPointPos), fading(false), mag(_mag), fadeTime(_fadeTime), fadeFrame(0), faded(nullptr) {
}