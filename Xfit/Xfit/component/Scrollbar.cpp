#include "Scrollbar.h"

#include "../math/Rect.h"
#include "../system/Input.h"
#include "../resource/Frame.h"

Scrollbar::Scrollbar(bool _isVertical, PosType _posType, ScaleImage* _bar, ScaleImage* _stick, PointF _pos, float _contentRatio, float _value, RectF _contentArea, float _wheelScrollStrength /*= 1.f*/) :
	isVertical(_isVertical), bar(_bar), stick(_stick), value(_value), contentRatio(_contentRatio), scrolling(false), controlFinish(nullptr), controlling(nullptr), 
	posType(_posType), baseContentArea(_contentArea), isDisable(false), visible(true), wheelScrollStrength(_wheelScrollStrength){

	stick->SetScale(PointF(contentRatio, 1.f));
	SetPos(_pos);

	contentArea = baseContentArea;
	const PointF ratioPoint = WindowRatioPoint(posType);
	contentArea.MoveRatio(ratioPoint.x, ratioPoint.y);

	contentArea.ExtendRatio(WindowRatio(), WindowRatio());
}

Scrollbar::~Scrollbar() {
	bar->visible = true;
	stick->visible = true;
}

PointF Scrollbar::GetPos()const {
	return barBasePos;
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
	stick->SetTransform(barBasePos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f), PointF(_contentRatio, 1.f), 0.f);

	if (contentRatio == 1.f || contentRatio == 0.f) {
		Disable(true);
	} else Disable(false);
}

void Scrollbar::SetPos(PointF _pos) {
	barBasePos = _pos;

	const float stickWidth = (float)stick->frame->GetWidth() * contentRatio;
	const float barWidth = (float)bar->frame->GetWidth();

	bar->SetPos(_pos);
	stick->SetPos(_pos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f));
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

void Scrollbar::SetValue(float _value) {
	value = _value;
	const float stickWidth = (float)stick->frame->GetWidth() * contentRatio;
	const float barWidth = (float)bar->frame->GetWidth();

	stick->SetPos(barBasePos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f));

	if (controlling)controlling(this);
	if (controlFinish)controlFinish(this);
}

bool Scrollbar::Update() {
	if (isDisable || !visible)return false;
	const PointF mousePos = Input::GetMousePos();
	const int wheelScroll = Input::GetWheelScrolling();
	const float stickWidth = (float)stick->frame->GetWidth() * contentRatio;
	const float barWidth = (float)bar->frame->GetWidth();

	if (Input::IsLMouseClick()) {
		const RectF rect = stick->GetRect();
		const RectF rect2 = bar->GetRect();

		if (rect.IsPointIn(mousePos)) {
			mouseStartPos = mousePos.x;
			stickStartPos = stick->pos.x;

			scrolling = true;
		} else if (rect2.IsPointIn(mousePos)) {
			if (mousePos.x < stick->pos.x) {//왼쪽으로 이동해야 될 때
				value -= contentRatio / (1.f - contentRatio);
				if (value < 0.f)value = 0.f;
			} else {
				value += contentRatio / (1.f - contentRatio);
				if (value > 1.f)value = 1.f;
			}
			const float stickWidth = (float)stick->frame->GetWidth() * contentRatio;
			const float barWidth = (float)bar->frame->GetWidth();

			stick->SetPos(barBasePos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f));
			if (controlling)controlling(this);
			return true;
		}
	} else if(Input::IsMouseOut() && scrolling) {
		if (controlFinish)controlFinish(this);
		scrolling = false;
	} else if (Input::IsLMouseClicking() && scrolling) {
		float posX = (stickStartPos + mousePos.x - mouseStartPos) / WindowRatioPointX(posType);
		const float minX = barBasePos.x - barWidth / 2.f + stickWidth/2.f;
		const float maxX = barBasePos.x + barWidth / 2.f - stickWidth / 2.f;
		if (posX < minX) {
			posX = minX;
			value = 0.f;
		} else if (posX > maxX) {
			posX = maxX;
			value = 1.f;
		} else {
			value = (posX - barBasePos.x - stickWidth / 2.f + barWidth / 2.f) / (barWidth - stickWidth);
		}
		stick->SetPos(PointF(posX, barBasePos.y));
		if(controlling)controlling(this);

		return true;
	} else if (Input::IsLMouseClicked() && scrolling) {
		if (controlFinish)controlFinish(this);
		scrolling = false;
	} else if (wheelScroll != 0 && contentArea.IsPointIn(mousePos)) {
		value += -wheelScroll * (stickWidth / (barWidth - stickWidth) * 0.002f) * wheelScrollStrength;
		if (value < 0.f)value = 0.f;
		else if (value > 1.f)value = 1.f;

		stick->SetPos(barBasePos + PointF(-barWidth / 2.f + stickWidth / 2.f + (barWidth - stickWidth) * value, 0.f));
		if (controlling)controlling(this);
		return true;
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
	const PointF ratioPoint = WindowRatioPoint(posType);
	contentArea.MoveRatio(ratioPoint.x, ratioPoint.y);

	contentArea.ExtendRatio(WindowRatio(), WindowRatio());
}
void Scrollbar::Disable(bool _disable /*= true*/) {
	isDisable = _disable;

	if (isDisable) {
		stick->visible = false;
	} else {
		stick->visible = true;
	}
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