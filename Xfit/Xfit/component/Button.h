#pragma once

#include "../object/ImageBase.h"
#include "../math/Rect.h"
#include "../resource/AnimateFrame.h"
#include "../system/Error.h"

class HitTest;

typedef AnimateFrame ButtonFrame;

class ButtonError : public Error {
public:
	enum class Code {
		NullHitTest,
		NullUpVertex,
		NullUpFrame,
		NullUpUV,
		InvalidState
	};
protected:
	Code code;
public:
	Code GetCode()const { return code; }
	ButtonError(ButtonError::Code _code) :code(_code) {}
};
class Button : public ImageBase {
public:
	virtual bool ButtonOver(Point _mousePos, void* _data) { return false; }
	virtual bool ButtonDown(Point _mousePos, void* _data) { return false; }
	virtual bool ButtonUp(Point _mousePos, void* _data) { return false; }
	virtual bool ButtonOut(Point _mousePos, void* _data) { return false; }

	virtual bool Update();
	HitTest* hitTest;
	void* data;
	ButtonFrame upFrame, overFrame, downFrame, disableFrame;

	enum class State {
		UP,
		OVER,
		DOWN,
		DISABLE
	};
	Button();
	Button(HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler);
	Button(const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame, HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler);
	void Disable(bool _on);
	State GetState() const;

	virtual void Draw();

	bool(*buttonOver)(Button* _target, Point _mousePos, void* _data);
	bool(*buttonDown)(Button* _target, Point _mousePos, void* _data);
	bool(*buttonUp)(Button* _target, Point _mousePos, void* _data);
	bool(*buttonOut)(Button* _target, Point _mousePos, void* _data);
private:
	State state;
};