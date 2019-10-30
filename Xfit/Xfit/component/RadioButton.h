#pragma once

#include "Checkbox.h"
#include "../data/Array.h"

class RadioButton : public Checkbox {
protected:
	virtual bool ButtonUp(PointF _mousePos, void* _data);
public:
	Array<RadioButton*> otherRadioButtons;
	virtual void SetCheck(bool _on);
};