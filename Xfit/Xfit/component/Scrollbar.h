#pragma once

#include "../resource/Vertex.h"
#include "../object/ScaleHeader.h"
#include "../object/ScaleImage.h"


class Scrollbar {
	bool isDisable;
	bool scrolling;
	bool isVertical;
	bool visible;

	float mouseStartPos;
	float stickStartPos;
	float value;
	float contentRatio;
	float wheelScrollStrength;

	PointF barBasePos;
	ScaleImage* bar;
	ScaleImage* stick;

	RectF baseContentArea;
	RectF contentArea;
public:
	PosType posType;
	void (*controlFinish)(Scrollbar* _target);
	void (*controlling)(Scrollbar* _target);

	//_contentRatio 보여지는 객체 크기 / 스크롤하는 전체 객체 크기
	Scrollbar(bool _isVertical, PosType _posType, ScaleImage* _bar, ScaleImage* _stick, PointF _pos, float _contentRatio, float _value, RectF _contentArea, float _wheelScrollStrength = 1.f);
	~Scrollbar();

	float GetValue()const;
	float GetContentRatio()const;

	void SetValue(float _value);//0~1
	void SetContentRatio(float _contentRatio);

	PointF GetPos()const;
	void SetPos(PointF _pos);

	float GetWidth()const;

	bool Update();
	void Draw();
	void Size();

	ScaleImage* GetBar()const;
	ScaleImage* GetStick()const;

	void Disable(bool _disable = true);
	void SetVisible(bool _visible);
	bool GetVisible()const;
};