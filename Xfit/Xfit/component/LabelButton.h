#pragma once

#include "../object/ScaleHeader.h"
#include "Button.h"
#include "../object/CenterPointPos.h"

#include "../text/SizeLabel.h"

class LabelButton : public Button {
public:
	virtual bool ButtonDown(PointF _mousePos, void* _data);
	virtual bool ButtonUp(PointF _mousePos, void* _data);
	virtual bool ButtonOut(PointF _mousePos, void* _data);

	CenterPointPos centerPointPos;

	LabelButton(SizeLabel* _label, PointF _pos = PointF(0.f, 0.f), CenterPointPos _centerPointPos = CenterPointPos::Center);
	virtual ~LabelButton();

	PointF basePos;

	void SetRectHitTest();
	SizeLabel* GetLabel()const;

	virtual void Size();

	virtual void SetPos(PointF _pos);
	virtual void SetX(float _x);
	virtual void SetY(float _y);

	static void LABELBUTTON_SIZE(LabelButton* _labelButton) { if (_labelButton->GetLabel()->IsChangeSize(WindowRatio())) { _labelButton->GetLabel()->SizePrepareDraw(WindowRatio()); _labelButton->Size(); } }
	static void LABELBUTTON_SIZE2(LabelButton* _labelButton, float _scale) { if (_labelButton->GetLabel()->IsChangeSize(WindowRatio() * _scale)) { _labelButton->GetLabel()->SizePrepareDraw(WindowRatio() * _scale); _labelButton->Size(); } }
};


