#include "LabelToggleButton.h"

bool LabelToggleButton::ButtonDown(PointF _mousePos, void* _data) {
	colorMat.e[15] = 0.5f;
	return true;
}
bool LabelToggleButton::ButtonUp(PointF _mousePos, void* _data) {
	SetToogle(!_toggle);
	return true;
}
bool LabelToggleButton::ButtonOut(PointF _mousePos, void* _data) {
	colorMat.e[15] = 1.f;
	return true;
}
bool LabelToggleButton::SetToogle(bool _toggle) {
	bool result = false;
	this->_toggle = _toggle;

	if (this->_toggle) {
		if (colorMat.e[3] != 1.f) {
			colorMat.e[3] = 1.f;
			result = true;
		}
	} else {
		if (colorMat.e[3] != 0.f) {
			colorMat.e[3] = 0.f;
			result = true;
		}
	}
	if (colorMat.e[15] != 1.f) {
		colorMat.e[15] = 1.f;
		result = true;
	}
	if (toggle) {
		result = toggle(this, this->_toggle, nullptr) ? true : result;
	}
	return result;
}
bool LabelToggleButton::IsToggle()const { return _toggle; }
LabelToggleButton::LabelToggleButton(SizeLabel* _label, PointF _pos/* = PointF(0.f, 0.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/) :
	LabelButton(_label,_pos, _centerPointPos), _toggle(false), toggle(nullptr) {
}