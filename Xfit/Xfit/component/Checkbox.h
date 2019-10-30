#pragma once

#include "Button.h"

class Vertex;
class Frame;

typedef AnimateFrame CheckFrame;

class CheckBoxError : public Error {
public:
	enum class Code {
		NullCheckVertex,
		NullCheckFrame,
		NullCheckUV
	};
protected:
	Code code;
public:
	Code GetCode()const { return code; }
	CheckBoxError(CheckBoxError::Code _code) :code(_code) {}
};

class Checkbox : public Button {
protected:
	bool checked;

	virtual bool ButtonUp(PointF _mousePos, void* _data);
public:
	CheckFrame checkFrame;

	Checkbox(const CheckFrame& _checkFrame, const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame, HitTest* _hitTest,
		PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler);

	Checkbox();
	bool IsChecked() const;
	virtual void SetCheck(bool _on);
	virtual void Draw();
};