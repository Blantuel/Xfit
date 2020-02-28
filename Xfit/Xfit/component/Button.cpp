#include "Button.h"
#include "../system/Input.h"
#include "../physics/HitTest.h"
#include "../system/System.h"

#ifdef _WIN32

Button::Button(const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame, HitTest* _hitTest,
	PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler) :
	state(State::UP), buttonOver(nullptr), buttonDown(nullptr), buttonUp(nullptr), buttonOut(nullptr), data(nullptr),
	upFrame(_upFrame), overFrame(_overFrame), downFrame(_downFrame), disableFrame(_disableFrame), hitTest(_hitTest), ImageBase(_pos, _scale, _rotation, _blend, _sampler) {}

Button::Button(HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler):
	state(State::UP), buttonOver(nullptr), buttonDown(nullptr), buttonUp(nullptr), buttonOut(nullptr), data(nullptr),
	upFrame(nullptr, nullptr, System::defaultUV), overFrame(nullptr, nullptr, System::defaultUV), downFrame(nullptr, nullptr, System::defaultUV), disableFrame(nullptr, nullptr, System::defaultUV),
	hitTest(_hitTest), ImageBase(_pos, _scale, _rotation, _blend, _sampler) {}

Button::Button() : state(State::UP), buttonOver(nullptr), buttonDown(nullptr), buttonUp(nullptr), buttonOut(nullptr), hitTest(nullptr), data(nullptr),
upFrame(nullptr,nullptr,System::defaultUV), overFrame(nullptr, nullptr, System::defaultUV), downFrame(nullptr, nullptr, System::defaultUV), disableFrame(nullptr, nullptr, System::defaultUV) {}

#elif __ANDROID__

Button::Button(const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame, HitTest* _hitTest,
			   PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler) :
		state(State::UP), buttonDown(nullptr), buttonUp(nullptr), buttonOut(nullptr), data(nullptr),
		upFrame(_upFrame), overFrame(_overFrame), downFrame(_downFrame), disableFrame(_disableFrame), hitTest(_hitTest), ImageBase(_pos, _scale, _rotation, _blend, _sampler) {}

Button::Button(HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler):
		state(State::UP), buttonDown(nullptr), buttonUp(nullptr), buttonOut(nullptr), data(nullptr),
		upFrame(nullptr, nullptr, System::defaultUV), overFrame(nullptr, nullptr, System::defaultUV), downFrame(nullptr, nullptr, System::defaultUV), disableFrame(nullptr, nullptr, System::defaultUV),
		hitTest(_hitTest), ImageBase(_pos, _scale, _rotation, _blend, _sampler) {}

Button::Button() : state(State::UP), buttonDown(nullptr), buttonUp(nullptr), buttonOut(nullptr), hitTest(nullptr), data(nullptr),
				   upFrame(nullptr,nullptr,System::defaultUV), overFrame(nullptr, nullptr, System::defaultUV), downFrame(nullptr, nullptr, System::defaultUV), disableFrame(nullptr, nullptr, System::defaultUV) {}

#endif

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
		const PointF mousePos = Input::GetMousePos();

#ifdef _WIN32
		if (hitTest->Test(mousePos)) {
			if (Input::IsLMouseClick()) {
				if (downFrame.frame && downFrame.uv && downFrame.vertex) {
					result = true;
				} else {
					result = overFrame.frame && overFrame.uv && overFrame.vertex && (state == State::UP);
				}

				state = State::DOWN;

				if (result) ButtonDown(mousePos, data);
				 else result = ButtonDown(mousePos, data);
				if (buttonDown) {
					if (result) buttonDown(this, mousePos, data);
					else result = buttonDown(this, mousePos, data);
				}
				return result;
			} else if (Input::IsLMouseClicked()) {
				state = State::OVER;

				result = ButtonUp(mousePos, data);
				if (buttonUp) {
					if (result) buttonUp(this, mousePos, data);
					else result = buttonUp(this, mousePos, data);
				}

				if (!result) result = downFrame.frame && downFrame.uv && downFrame.vertex;
				return result;
			} else if ((state != State::DOWN) && (state != State::OVER)) {
				state = State::OVER;

				result = ButtonOver(mousePos, data);
				if (buttonOver) {
					if (result) buttonOver(this, mousePos, data);
					else result = buttonOver(this, mousePos, data);
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

			if (result) ButtonOut(mousePos, data);
			else result = ButtonOut(mousePos, data);

			if (buttonOut) {
				if (result) buttonOut(this, mousePos, data);
				else result = buttonOut(this, mousePos, data);
			}
			return result;
		}
#elif __ANDROID__
		if (hitTest->Test(mousePos)) {
			if (Input::IsLMouseClick()) {
				if (downFrame.frame && downFrame.uv && downFrame.vertex) {
					result = true;
				} else {
					result = overFrame.frame && overFrame.uv && overFrame.vertex && (state == State::UP);
				}
				state = State::DOWN;

				if (result) ButtonDown(mousePos, data);
				else result = ButtonDown(mousePos, data);
				if (buttonDown) {
					if (result) buttonDown(this, mousePos, data);
					else result = buttonDown(this, mousePos, data);
				}
				return result;
			} else if (Input::IsLMouseClicked()) {
				state = State::UP;

				result = ButtonUp(mousePos, data);
				if (buttonUp) {
					if (result) buttonUp(this, mousePos, data);
					else result = buttonUp(this, mousePos, data);
				}

				if (!result) result = downFrame.frame && downFrame.uv && downFrame.vertex;
				return result;
			}
		} else if (state == State::DOWN) {
			result = downFrame.frame && downFrame.uv && downFrame.vertex;
			state = State::UP;

			if (result) ButtonOut(mousePos, data);
			else result = ButtonOut(mousePos, data);

			if (buttonOut) {
				if (result) buttonOut(this, mousePos, data);
				else result = buttonOut(this, mousePos, data);
			}
			return result;
		}
#endif
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
#ifdef _WIN32
		case State::OVER:
		over:;
			if (!overFrame.frame || !overFrame.uv || !overFrame.vertex)goto up;
			frame = &overFrame;
			break;
		case State::DOWN:
			if (!downFrame.frame || !downFrame.uv || !downFrame.vertex)goto over;
			frame = &downFrame;
			break;
#elif __ANDROID__
		case State::DOWN:
			if (!downFrame.frame || !downFrame.uv || !downFrame.vertex)goto up;
			frame = &downFrame;
			break;
#endif
		case State::DISABLE:
			if (!disableFrame.frame || !disableFrame.uv || !disableFrame.vertex)goto up;
			frame = &disableFrame;
			break;
		default:throw ButtonError(ButtonError::Code::InvalidState);
		}
		DrawImage(frame->vertex, frame->uv,frame->index, frame->frame);
	}
}