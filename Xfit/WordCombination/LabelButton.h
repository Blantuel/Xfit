#pragma once

#include "main.h"
#include <component/Button.h>
#include <resource/CenterPointPos.h>

class SizeLabel;

class LabelButton : public Button {
public:
	virtual bool ButtonDown(Point _mousePos, void* _data);
	virtual bool ButtonUp(Point _mousePos, void* _data);
	virtual bool ButtonOut(Point _mousePos, void* _data);

	PosType posType;

	CenterPointPos centerPointPos;

	LabelButton(PosType _posType, SizeLabel* _label, PointF _pos = PointF(0.f, 0.f), CenterPointPos _centerPointPos = CenterPointPos::Center);
	virtual ~LabelButton();

	PointF basePos;

	void SetRectHitTest();
	SizeLabel* GetLabel()const;

	virtual void Size(bool _scale = true);
	virtual void PrepareDraw();
};


