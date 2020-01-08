#include "Slider.h"

#include "../math/Rect.h"
#include "../system/Input.h"
#include "../resource/Frame.h"

Slider::Slider(PosType _posType, Frame* _barFrame, Frame* _stickFrame, PointF _pos, float _value):
bar(PosType::Center, _barFrame),
stick(PosType::Center, _stickFrame),
value(_value), sliding(false), controlFinish(nullptr), controlling(nullptr), posType(_posType) {
	SetPos(_pos);

}
PointF Slider::GetPos()const {
	return barBasePos;
}

void Slider::SetPos(PointF _pos) {
	barBasePos = _pos;

	bar.SetPos(_pos);
	stick.SetPos(_pos + PointF(-(float)bar.frame->GetWidth() / 2.f + (value / 1.f) * (float)bar.frame->GetWidth(), 0.f));
}
float Slider::GetWidth()const {
	return (float)bar.frame->GetWidth();
}

float Slider::GetValue()const {
	return value;
}

void Slider::SetValue(float _value) {
	value = _value;
	stick.pos = (barBasePos + PointF(-(float)bar.frame->GetWidth() / 2.f + value * (float)bar.frame->GetWidth(), 0.f)) * WindowRatioPoint(posType);
	stick.UpdateMatrix();

	if (controlling)controlling(this);
	if (controlFinish)controlFinish(this);
}

bool Slider::Update() {
	if (Input::IsLMouseClick()) {
		const float x = XToMouseX(stick.pos.x);
		const float y = YToMouseY(stick.pos.y);

		const float x2 = XToMouseX(bar.pos.x);
		const float y2 = YToMouseY(bar.pos.y);
		
		const Point mousePos = Input::GetMousePosScreen();
		const PointF mousePosF = PointF((float)mousePos.x, (float)mousePos.y);
		const RectF rect = RectF(x - ((float)stick.frame->GetWidth() / 2.f) * WindowRatio(), x + ((float)stick.frame->GetWidth() / 2.f) * WindowRatio(),
			y + ((float)stick.frame->GetHeight() / 2.f) * WindowRatio(), y - ((float)stick.frame->GetHeight() / 2.f) * WindowRatio());
		const RectF rect2 = RectF(x2 - bar.scale.x / 2.f, x2 + bar.scale.x / 2.f,
			y2 + bar.scale.y / 2.f, y2 - bar.scale.y / 2.f);

		if (rect.IsPointIn(mousePosF)) {
			sliding = true;
		} else if (rect2.IsPointIn(mousePosF)) {
			stick.pos.x = MouseXToX(mousePos.x);

			value = ((stick.pos.x / WindowRatio() - barBasePos.x) + (float)bar.frame->GetWidth() / 2.f) / (float)bar.frame->GetWidth();

			stick.UpdateMatrix();

			sliding = true;
			return true;
		}
	} else if (Input::IsLMouseClicking() && sliding) {
		if (Input::IsMouseOut()) {
			sliding = false;
			if(controlFinish)controlFinish(this);
			return false;
		}
		const Point mousePos = Input::GetMousePosScreen();
		stick.pos.x = MouseXToX(mousePos.x);

		const float minX = bar.pos.x - bar.scale.x / 2.f;
		const float maxX = bar.pos.x + bar.scale.x / 2.f;
		if (stick.pos.x < minX) {
			stick.pos.x = minX;
		} else if (stick.pos.x > maxX) {
			stick.pos.x = maxX;
		}

		value = ((stick.pos.x / WindowRatio() - barBasePos.x) + (float)bar.frame->GetWidth() / 2.f) / (float)bar.frame->GetWidth();

		stick.UpdateMatrix();

		if (controlling)controlling(this);
		return true;
	} else if (Input::IsLMouseClicked() && sliding) {
		if (controlFinish)controlFinish(this);
		sliding = false;
	}
	return false;
}
void Slider::Draw() {
	bar.Draw();
	stick.Draw();
}
void Slider::Size() {
	stick.basePos = (barBasePos + PointF(-(float)bar.frame->GetWidth() / 2.f + (value / 1.f) * (float)bar.frame->GetWidth(), 0.f));

	bar.Size();
	stick.Size();
}