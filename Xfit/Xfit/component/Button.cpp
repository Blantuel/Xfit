#include "Button.h"
#include "../system/Input.h"
#include "../physics/HitTest.h"
#include "../system/System.h"

Button::Button(const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame, HitTest* _hitTest,
	PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler) :
	state(State::UP), buttonOver(nullptr), buttonDown(nullptr), buttonUp(nullptr), buttonOut(nullptr),
	upFrame(_upFrame), overFrame(_overFrame), downFrame(_downFrame), disableFrame(_disableFrame), hitTest(_hitTest), ImageBase(_pos, _scale, _rotation, _blend, _sampler) {}

Button::Button(HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler):
	state(State::UP), buttonOver(nullptr), buttonDown(nullptr), buttonUp(nullptr), buttonOut(nullptr),
	upFrame(nullptr, nullptr, System::defaultUV), overFrame(nullptr, nullptr, System::defaultUV), downFrame(nullptr, nullptr, System::defaultUV), disableFrame(nullptr, nullptr, System::defaultUV),
	hitTest(_hitTest), ImageBase(_pos, _scale, _rotation, _blend, _sampler) {}

Button::Button() : state(State::UP), buttonOver(nullptr), buttonDown(nullptr), buttonUp(nullptr), buttonOut(nullptr), hitTest(nullptr),
upFrame(nullptr,nullptr,System::defaultUV), overFrame(nullptr, nullptr, System::defaultUV), downFrame(nullptr, nullptr, System::defaultUV), disableFrame(nullptr, nullptr, System::defaultUV) {}

void Button::Disable(bool _on) {
	if (_on) state = State::DISABLE;
	else state = State::UP;
}

Button::State Button::GetState() const { return state; }

bool Button::Update() {
	if (state != State::DISABLE) {
#ifdef _DEBUG
		if (!hitTest)throw ButtonError(ButtonError::Code::NullHitTest);
#endif
		bool result = false;
		const Point mousePos = Input::GetMousePosScreen();
		if (hitTest->Test(mousePos)) {
			if (Input::IsLMouseClick()) {
				if (downFrame.frame && downFrame.uv && downFrame.vertex) {
					result = true;
				} else {
					result = overFrame.frame && overFrame.uv && overFrame.vertex && (state == State::UP);
				}

				state = State::DOWN;

				if (result) ButtonDown(mousePos, nullptr);
				 else result = ButtonDown(mousePos, nullptr);
				if (buttonDown) {
					if (result) buttonDown(this, mousePos, nullptr);
					else result = buttonDown(this, mousePos, nullptr);
				}
				return result;
			} else if (Input::IsLMouseClicked()) {
				state = State::OVER;

				result = ButtonUp(mousePos, nullptr);
				if (buttonUp) {
					if (result) buttonUp(this, mousePos, nullptr);
					else result = buttonUp(this, mousePos, nullptr);
				}

				if (!result) result = downFrame.frame && downFrame.uv && downFrame.vertex;
				return result;
			} else if ((state != State::DOWN) && (state != State::OVER)) {
				state = State::OVER;

				result = ButtonOver(mousePos, nullptr);
				if (buttonOver) {
					if (result) buttonOver(this, mousePos, nullptr);
					else result = buttonOver(this, mousePos, nullptr);
				}

				if (!result) result = overFrame.frame && overFrame.uv && overFrame.vertex;
				return result;
			}
		} else if (state == State::DOWN || state == State::OVER) {
			if (state == State::DOWN) {
				if (downFrame.frame && downFrame.uv && downFrame.vertex) {
					result = true;
				} else {
					result = overFrame.frame && overFrame.uv && overFrame.vertex;
				}
			} else {//State::OVER
				result = overFrame.frame && overFrame.uv && overFrame.vertex;
			}

			state = State::UP;

			if (result) ButtonOut(mousePos, nullptr);
			else result = ButtonOut(mousePos, nullptr);

			if (buttonOut) {
				if (result) buttonOut(this, mousePos, nullptr);
				else result = buttonOut(this, mousePos, nullptr);
			}
			return result;
		}
	}
	return false;
}
void Button::Draw() {
	if (visible) {
		Object::Draw();
		ButtonFrame* frame = nullptr;
		switch (state) {
		case State::UP:
#ifdef _DEBUG
			if (!upFrame.frame)throw ButtonError(ButtonError::Code::NullUpFrame);
			if (!upFrame.uv)throw ButtonError(ButtonError::Code::NullUpUV);
			if (!upFrame.vertex)throw ButtonError(ButtonError::Code::NullUpVertex);
#endif
		up:;
			frame = &upFrame;
			break;
		case State::OVER:
		over:;
			if (!overFrame.frame || !overFrame.uv || !overFrame.vertex)goto up;
			frame = &overFrame;
			break;
		case State::DOWN:
			if (!downFrame.frame || !downFrame.uv || !downFrame.vertex)goto over;
			frame = &downFrame;
			break;
		case State::DISABLE:
			if (!disableFrame.frame || !disableFrame.uv || !disableFrame.vertex)goto up;
			frame = &disableFrame;
			break;
		default:throw ButtonError(ButtonError::Code::InvalidState);
		}
		DrawImage(frame->vertex, frame->uv,frame->index, frame->frame);
	}
}