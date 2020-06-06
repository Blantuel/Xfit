#pragma once

#include "LabelButton.h"

class LabelToggleButton : public LabelButton {
protected:
	bool _toggle;
public:
	bool(*toggle)(LabelToggleButton* _target, bool _isToggle, void* _data);

	virtual bool ButtonDown(PointF _mousePos, void* _data);
	virtual bool ButtonUp(PointF _mousePos, void* _data);
	virtual bool ButtonOut(PointF _mousePos, void* _data);

	LabelToggleButton(SizeLabel* _label,  PointF _pos = PointF(0.f, 0.f), CenterPointPos _centerPointPos = CenterPointPos::Center);
	
	bool SetToogle(bool _toggle);
	bool IsToggle()const;
};

