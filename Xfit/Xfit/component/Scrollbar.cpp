#include "Scrollbar.h"

#include "../math/Rect.h"
#include "../system/Input.h"
#include "../resource/Frame.h"

Scrollbar::Scrollbar(bool _isVertical, ScaleImage* _bar, ScaleImage* _stick, PointF _pos, float _contentRatio, float _value, RectF _contentArea, PointF _plusScrollArea /*= PointF(0.f, 0.f)*/, PointF _plusScrollArea2 /*= PointF(0.f, 0.f)*/, float _wheelScrollStrength /*= 1.f*/) :
	isVertical(_isVertical), bar(_bar), stick(_stick), value(_value), contentRatio(_contentRatio), scrolling(false), controlFinish(nullptr), controlling(nullptr), 
	baseContentArea(_contentArea), isDisable(false), visible(true), wheelScrollStrength(_wheelScrollStrength), plusScrollArea(_plusScrollArea), plusScrollArea2(_plusScrollArea2), leftOrRight(false), wheelScrolling(-1.f){
	if (isVertical) {
		bar->rotation = 90.f;
		stick->rotation = 90.f;
	}
	stick->baseScale = PointF(contentRatio, 1.f);
	SetPos(_pos);

	contentArea = baseContentArea;
	const PointF ratioPoint = WindowRatioPoint();
	contentArea.MoveRatio(ratioPoint.x, ratioPoint.y);

	contentArea.ExtendRatio(ratioPoint.x, ratioPoint.y);
}

Scrollbar::~Scrollbar() {
	bar->visible = true;
	stick->visible = true;
}

PointF Scrollbar::GetPos()const {
	return barBasePos;
}
float Scrollbar::GetX()const {
	return barBasePos.x;
}
float Scrollbar::GetY()const {
	return barBasePos.y;
}
void Scrollbar::SetX(float _x) {
	SetPos(PointF(_x, GetY()));
}
void Scrollbar::SetY(float _y) {
	SetPos(PointF(GetX(), _y));
}
ScaleImage* Scrollbar::GetBar()const {
	return bar;
}
ScaleImage* Scrollbar::GetStick()const {
	return stick;
}
void Scrollbar::SetContentRatio(float _contentRatio) {
	contentRatio = _contentRatio;

	const float stickWidth = (float)stick->frame->GetWidth() * contentRatio;
	const float barWidth = (float)bar->frame->GetWidth();
	stick->SetTransform(barBasePos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f), PointF(_contentRatio, 1.f), stick->rotation);

	if (contentRatio >= 1.f || contentRatio == 0.f) {
		Disable(true);
	} else Disable(false);
}

void Scrollbar::SetPos(PointF _pos) {
	barBasePos = _pos;

	const float stickWidth = (float)stick->frame->GetWidth() * contentRatio;
	const float barWidth = (float)bar->frame->GetWidth();

	bar->SetPos(_pos);
	if (isVertical) {
		stick->SetPos(_pos + PointF(0.f, -(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value)));
	} else {
		stick->SetPos(_pos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f));
	}
	
}

void Scrollbar::SetVertical(bool _vertical) {
	isVertical = _vertical;
	if (isVertical) {
		bar->rotation = 90.f;
		stick->rotation = 90.f;
	} else {
		bar->rotation = 0.f;
		stick->rotation = 0.f;
	}
	SetPos(barBasePos);
}
bool Scrollbar::IsVertical()const {
	return isVertical;
}

float Scrollbar::GetWidth()const {
	return (float)bar->frame->GetWidth();
}

float Scrollbar::GetValue()const {
	return value;
}
float Scrollbar::GetContentRatio()const {
	return contentRatio;
}

void Scrollbar::SetValue(float _value, bool _noCallback /*= false*/) {
	leftOrRight = (value - _value) > 0 ? false : true;
	value = _value;
	const float stickWidth = (float)stick->frame->GetWidth() * contentRatio;
	const float barWidth = (float)bar->frame->GetWidth();

	if (isVertical) {
		stick->SetPos(barBasePos + PointF(0.f, -(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value)));
	} else {
		stick->SetPos(barBasePos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f));
	}
	
	if (!_noCallback) {
		if (controlling)controlling(this, leftOrRight);
		if (controlFinish)controlFinish(this, leftOrRight);
	}
}

bool Scrollbar::Update() {
	if (isDisable || !visible)return false;
	const PointF mousePos = Input::GetMousePos();
	const int wheelScroll = Input::GetWheelScrolling();
	const float stickWidth = (float)stick->frame->GetWidth() * contentRatio;
	const float barWidth = (float)bar->frame->GetWidth();

	if (Input::IsLMouseClick()) {
		RectF rect;
		RectF rect2;
		if (isVertical) {
			rect = stick->GetRect90();
			rect2 = bar->GetRect90();
		} else {
			rect = stick->GetRect();
			rect2 = bar->GetRect();
		}

		rect.top += plusScrollArea.x * WindowRatio();
		rect2.top += plusScrollArea2.x * WindowRatio();
		rect.bottom -= plusScrollArea.y * WindowRatio();
		rect2.bottom -= plusScrollArea2.y * WindowRatio();

		if (rect.IsPointIn(mousePos)) {
			if (isVertical) {
				mouseStartPos = mousePos.y;
				stickStartPos = stick->pos.y;
			} else {
				mouseStartPos = mousePos.x;
				stickStartPos = stick->pos.x;
			}
			scrolling = true;
		} else if (rect2.IsPointIn(mousePos)) {
			const float stickWidth = (float)stick->frame->GetWidth() * contentRatio;
			const float barWidth = (float)bar->frame->GetWidth();
			if (isVertical) {
				if (mousePos.y > stick->pos.y) {//왼쪽으로 이동해야 될 때
					value -= contentRatio / (1.f - contentRatio);
					if (value < 0.f)value = 0.f;
					leftOrRight = false;
				} else {
					value += contentRatio / (1.f - contentRatio);
					if (value > 1.f)value = 1.f;
					leftOrRight = true;
				}
				stick->SetPos(barBasePos + PointF(0.f, -(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value)));
			} else {
				if (mousePos.x < stick->pos.x) {//왼쪽으로 이동해야 될 때
					value -= contentRatio / (1.f - contentRatio);
					if (value < 0.f)value = 0.f;
					leftOrRight = false;
				} else {
					value += contentRatio / (1.f - contentRatio);
					if (value > 1.f)value = 1.f;
					leftOrRight = true;
				}
				stick->SetPos(barBasePos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f));
			}
			
			if (controlling)controlling(this, leftOrRight);
			return true;
		}
	} else if(Input::IsMouseOut() && scrolling) {
		if (controlFinish)controlFinish(this, leftOrRight);
		scrolling = false;
	} else if (Input::IsLMouseClicking() && scrolling) {
		if (isVertical) {
			float posY = (stickStartPos + mousePos.y - mouseStartPos) / WindowRatio();
			const float minY = barBasePos.y + barWidth / 2.f - stickWidth / 2.f;
			const float maxY = barBasePos.y - barWidth / 2.f + stickWidth / 2.f;
			if (posY > minY) {
				posY = minY;
				value = 0.f;
				leftOrRight = false;
			} else if (posY < maxY) {
				posY = maxY;
				value = 1.f;
				leftOrRight = true;
			} else {
				const float valueT = -(posY - barBasePos.y + stickWidth / 2.f - barWidth / 2.f) / (barWidth - stickWidth);
				leftOrRight = (value - valueT) > 0 ? false : true;
				value = valueT;
			}
			stick->SetPos(PointF(barBasePos.x, posY));
		} else {
			float posX = (stickStartPos + mousePos.x - mouseStartPos) / WindowRatio();
			const float minX = barBasePos.x - barWidth / 2.f + stickWidth / 2.f;
			const float maxX = barBasePos.x + barWidth / 2.f - stickWidth / 2.f;
			if (posX < minX) {
				posX = minX;
				value = 0.f;
				leftOrRight = false;
			} else if (posX > maxX) {
				posX = maxX;
				value = 1.f;
				leftOrRight = true;
			} else {
				const float valueT = (posX - barBasePos.x - stickWidth / 2.f + barWidth / 2.f) / (barWidth - stickWidth);
				leftOrRight = (value - valueT) > 0 ? false : true;
				value = valueT;
			}
			stick->SetPos(PointF(posX, barBasePos.y));
		}
		
		if(controlling)controlling(this, leftOrRight);

		return true;
	} else if (Input::IsLMouseClicked() && scrolling) {
		if (controlFinish)controlFinish(this, leftOrRight);
		scrolling = false;
	} else if (wheelScroll != 0 && contentArea.IsPointIn(mousePos)) {
		const float valueT = -wheelScroll * (stickWidth / (barWidth - stickWidth) * 0.002f) * wheelScrollStrength;
		value += valueT;
		if (value < 0.f)value = 0.f;
		else if (value > 1.f)value = 1.f;
		leftOrRight = valueT > 0 ? true : false;


		if (isVertical) {
			stick->SetPos(barBasePos + PointF(0.f, -(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value)));
		} else {
			stick->SetPos(barBasePos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f));
		}

		if (controlling)controlling(this, leftOrRight);
		wheelScrolling = 0.f;
		return true;
	} else if (wheelScrolling >= 0.f) {
		wheelScrolling += System::GetDeltaTime();

		if (wheelScrolling >= 0.3f) {
			if (controlFinish)controlFinish(this, leftOrRight);
			wheelScrolling = -1.f;
		}
	}
	return false;
}
void Scrollbar::Draw() {
	bar->Draw();
	stick->Draw();
}
void Scrollbar::Size() {
	if (!visible)return;
	bar->Size();
	stick->Size();

	contentArea = baseContentArea;
	const PointF ratioPoint = WindowRatioPoint();
	contentArea.MoveRatio(ratioPoint.x, ratioPoint.y);

	contentArea.ExtendRatio(ratioPoint.x, ratioPoint.y);
}
void Scrollbar::Disable(bool _disable /*= true*/) {
	isDisable = _disable;

	if (isDisable) {
		stick->visible = false;
	} else {
		stick->visible = true;
	}
}
bool Scrollbar::IsDisable()const {
	return isDisable;
}
void Scrollbar::SetVisible(bool _visible) {
	visible = _visible;

	stick->visible = visible;
	bar->visible = visible;

	Size();
}
bool Scrollbar::GetVisible()const {
	return visible;
}