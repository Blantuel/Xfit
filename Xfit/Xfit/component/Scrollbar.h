#pragma once

#include "../resource/Vertex.h"
#include "../object/ScaleHeader.h"
#include "../object/ScaleImage.h"


class Scrollbar {
	bool isDisable;
	bool scrolling;
	float wheelScrolling;
	bool isVertical;
	bool visible;
	bool leftOrRight;

	float mouseStartPos;
	float stickStartPos;
	float value;
	float contentRatio;
	float wheelScrollStrength;

	PointF plusScrollArea;
	PointF plusScrollArea2;

	PointF barBasePos;
	ScaleImage* bar;
	ScaleImage* stick;

	RectF baseContentArea;
	RectF contentArea;
public:
	//left가 false, right가 true
	void (*controlFinish)(Scrollbar* _target, bool _leftOrRight);
	//left가 false, right가 true
	void (*controlling)(Scrollbar* _target, bool _leftOrRight);

	//_contentRatio 보여지는 객체 크기 / 스크롤하는 전체 객체 크기
	Scrollbar(bool _isVertical, ScaleImage* _bar, ScaleImage* _stick, PointF _pos, float _contentRatio, float _value, RectF _contentArea, PointF _plusScrollArea = PointF(0.f, 0.f), PointF _plusScrollArea2 = PointF(0.f, 0.f), float _wheelScrollStrength = 1.f);
	~Scrollbar();

	float GetValue()const;
	float GetContentRatio()const;

	void SetValue(float _value, bool _noCallback = false);//0~1
	//_contentRatio 보여지는 객체 크기 / 스크롤하는 전체 객체 크기
	void SetContentRatio(float _contentRatio);

	PointF GetPos()const;
	float GetX()const;
	float GetY()const;
	void SetPos(PointF _pos);
	void SetX(float _x);
	void SetY(float _y);

	float GetWidth()const;

	void SetVertical(bool _vertical);
	bool IsVertical()const;

	bool Update();
	void Draw();
	void Size();

	ScaleImage* GetBar()const;
	ScaleImage* GetStick()const;

	void Disable(bool _disable = true);
	bool IsDisable()const;
	void SetVisible(bool _visible);
	bool GetVisible()const;
};