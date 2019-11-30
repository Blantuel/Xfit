#include "Slider.h"


#include <math/Rect.h>
#include <system/Input.h>

Slider::Slider(PosType _posType, PointF _pos, float _value, float _width /*= 200.f*/):
bar(PointF(0.f,0.f), PointF(WindowRatio(),1.f),0.f,nullptr,&barVertex, Point3DwF(0.f, 0.f, 0.f, 1.f)),
stick(PointF(0.f, 0.f), WindowRatioPoint(),0.f,nullptr,&stickVertex, Point3DwF(1.f, 1.f, 1.f, 1.f),Point3DwF(0.f,0.f,0.f,1.f)),
width(_width), value(_value), sliding(false), controlFinish(nullptr), controlling(nullptr), posType(_posType) {
	SetPos(_pos);

	stick.lineWidth = GetLineWidth();
	bar.lineWidth = stick.lineWidth;

	barVertex.vertices.SetData(barVertexData, 0, 2);
	stickVertex.vertices.SetData(stickVertexData, 0, 4);

	barVertex.vertices.EmplaceLast(-width / 2.f, 0.f);
	barVertex.vertices.EmplaceLast(width / 2.f, 0.f);

	barVertex.Build();

	stickVertex.MakeImageVertex2D(PointF(20.f, 30.f));
	
	stickVertex.Build();
}
PointF Slider::GetPos()const {
	return barBasePos;
}
void Slider::SetPos(PointF _pos) {
	barBasePos = _pos;

	bar.SetPos(_pos * WindowRatioPoint(posType));
	stick.SetPos((_pos + PointF(-width / 2.f + (value / 1.f) * width, 0.f)) * WindowRatioPoint(posType));
}
float Slider::GetWidth()const {
	return width;
}

float Slider::GetValue()const {
	return value;
}

void Slider::SetValue(float _value) {
	value = _value;
	stick.pos = (barBasePos + PointF(-width / 2.f + value * width, 0.f)) * WindowRatioPoint(posType);
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
		const RectF rect = RectF(x + stickVertex.vertices[0].x*WindowRatio(), x + stickVertex.vertices[1].x * WindowRatio(),
			y + stickVertex.vertices[0].y * WindowRatio(), y + stickVertex.vertices[2].y * WindowRatio());
		const RectF rect2 = RectF(x2 + barVertex.vertices[0].x * WindowRatio(), x2 + barVertex.vertices[1].x * WindowRatio(),
			y2 + stickVertex.vertices[0].y * WindowRatio(), y2 + stickVertex.vertices[2].y * WindowRatio());

		if (rect.IsPointIn(mousePosF)) {
			sliding = true;
		} else if (rect2.IsPointIn(mousePosF)) {
			stick.pos.x = MouseXToX(mousePos.x);

			value = ((stick.pos.x / WindowRatio() - barBasePos.x) + width / 2.f) / width;

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

		const float minX = bar.pos.x + barVertex.vertices[0].x*WindowRatio();
		const float maxX = bar.pos.x + barVertex.vertices[1].x * WindowRatio();
		if (stick.pos.x < minX) {
			stick.pos.x = minX;
		} else if (stick.pos.x > maxX) {
			stick.pos.x = maxX;
		}

		value = ((stick.pos.x / WindowRatio() - barBasePos.x) + width / 2.f) / width;

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
	bar.pos = barBasePos * WindowRatioPoint(posType);
	stick.pos = (barBasePos + PointF(-width / 2.f + (value / 1.f) * width, 0.f)) * WindowRatioPoint(posType);

	bar.scale.x = WindowRatio();
	stick.scale = WindowRatioPoint();

	bar.UpdateMatrix();
	stick.UpdateMatrix();

	stick.lineWidth = GetLineWidth();
	bar.lineWidth = stick.lineWidth;
}