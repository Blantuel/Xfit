#include "LabelToggleButton.h"

bool LabelToggleButton::ButtonDown(Point _mousePos, void* _data) {
	colorMat.e[15] = 0.5f;
	return true;
}
bool LabelToggleButton::ButtonUp(Point _mousePos, void* _data) {
	SetToogle(!toggle);
	return true;
}
bool LabelToggleButton::ButtonOut(Point _mousePos, void* _data) {
	colorMat.e[15] = 1.f;
	return true;
}
void LabelToggleButton::SetToogle(bool _toggle) {
	toggle = _toggle;

	if (toggle)colorMat.e[3] = 1.f;
	else colorMat.e[3] = 0.f;

	colorMat.e[15] = 1.f;
}
bool LabelToggleButton::IsToggle()const { return toggle; }
LabelToggleButton::LabelToggleButton(PosType _posType, SizeLabel* _label, PointF _pos/* = PointF(0.f, 0.f)*/, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/) :
	LabelButton(_posType, _label,_pos, _centerPointPos), toggle(false) {
}