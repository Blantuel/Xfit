#include "LabelOptionButton.h"

bool LabelOptionButton::ButtonDown(PointF _mousePos, void* _data) {
	colorMat.e[15] = 0.5f;
	return true;
}
bool LabelOptionButton::ButtonUp(PointF _mousePos, void* _data) {
	option++;
	if (option >= optionLen)option = 0;
	return true;
}
bool LabelOptionButton::ButtonOut(PointF _mousePos, void* _data) {
	colorMat.e[15] = 1.f;
	return true;
}

void LabelOptionButton::SetOption(unsigned _option) {
	option = _option;
}
unsigned LabelOptionButton::GetOptionIndex()const {
	return option;
}
unsigned LabelOptionButton::GetOptionInLen()const {
	return optionLen;
}

LabelOptionButton::LabelOptionButton(SizeLabel* _label, unsigned _option, unsigned _optionLen, PointF _pos /*= PointF(0.f, 0.f)*/,
	CenterPointPos _centerPointPos /*= CenterPointPos::Center*/) :
	LabelButton(_label,_pos, _centerPointPos), option(_option), optionLen(_optionLen) {
}