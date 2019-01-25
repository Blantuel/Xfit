#pragma once

#include "Checkbox.h"
#include "../data/Array.h"

class RadioButton : public Checkbox {
protected:
	virtual void ButtonUp(PointF _mousePos);
public:
	Array<RadioButton> otherRadioButtons;
	virtual void SetCheck(bool _on);
};