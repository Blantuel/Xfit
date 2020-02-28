#pragma once

#include "LabelButton.h"

class LabelOptionButton : public LabelButton {
protected:
	unsigned option;
	unsigned optionLen;
public:
	virtual bool ButtonDown(PointF _mousePos, void* _data);
	virtual bool ButtonUp(PointF _mousePos, void* _data);
	virtual bool ButtonOut(PointF _mousePos, void* _data);

	LabelOptionButton(PosType _posType, SizeLabel* _label, unsigned _option, unsigned _optionLen, PointF _pos = PointF(0.f, 0.f), CenterPointPos _centerPointPos = CenterPointPos::Center);
	
	void SetOption(unsigned _option);
	unsigned GetOptionIndex()const;
	unsigned GetOptionInLen()const;
};

