#include "RadioButton.h"

void RadioButton::SetCheck(bool _on) {
	Checkbox::SetCheck(_on);
	if (_on) {
		for (size_t i = 0; i < otherRadioButtons.Size(); i++) {
			otherRadioButtons[i]->SetCheck(false);
		}
	}
}
bool RadioButton::ButtonUp(PointF _mousePos, void* _data) {
	if (!IsChecked()) return Checkbox::ButtonUp(_mousePos, _data);
	return false;
}