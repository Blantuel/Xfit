#pragma once

#include "LabelButton.h"

class SizeLabel;

class LabelToggleButton : public LabelButton {
protected:
	bool toggle;
public:
	virtual bool ButtonDown(Point _mousePos, void* _data);
	virtual bool ButtonUp(Point _mousePos, void* _data);
	virtual bool ButtonOut(Point _mousePos, void* _data);

	LabelToggleButton(PosType _posType, SizeLabel* _label,  PointF _pos = PointF(0.f, 0.f), CenterPointPos _centerPointPos = CenterPointPos::Center);
	
	void SetToogle(bool _toggle);
	bool IsToggle()const;
};

