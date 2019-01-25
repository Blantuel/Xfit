#include "RadioButton.h"

void RadioButton::SetCheck(bool _on) {
	Checkbox::SetCheck(_on);
	if (_on) {
		for (size_t i = 0; i < otherRadioButtons.Size(); i++) {
			otherRadioButtons[i].SetCheck(false);
		}
	}
}
void RadioButton::ButtonUp(PointF _mousePos) {
	if (!IsChecked()) Checkbox::ButtonUp(_mousePos);
}