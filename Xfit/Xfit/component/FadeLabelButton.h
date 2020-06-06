#pragma once

#include "LabelButton.h"

class SizeLabel;

class FadeLabelButton : public LabelButton {
protected:
	bool fading;
	const float mag, fadeTime;
	size_t fadeFrame;

public:
	virtual bool ButtonDown(PointF _mousePos, void* _data);
	void(*faded)(FadeLabelButton* _target, void* _data);

	virtual bool Update();
	FadeLabelButton(SizeLabel* _label, PointF _pos = PointF(0.f, 0.f), CenterPointPos _centerPointPos = CenterPointPos::Center, float _mag = 2.f, float _fadeTime = 0.5f);

	virtual void Size();

	void Remake();
};