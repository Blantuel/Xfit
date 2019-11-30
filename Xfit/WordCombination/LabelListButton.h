#pragma once

#include "LabelButton.h"

class Label;

class LabelListButton : public LabelButton {
protected:
	bool toggle;
public:
	unsigned index;
	virtual bool ButtonDown(Point _mousePos, void* _data);
	virtual bool ButtonUp(Point _mousePos, void* _data);
	virtual bool ButtonOut(Point _mousePos, void* _data);

	LabelListButton(PosType _posType, Label* _label, unsigned _labelSizeLen, PointF _pos = PointF(0.f, 0.f), CenterPointPos _centerPointPos = CenterPointPos::Center);
};