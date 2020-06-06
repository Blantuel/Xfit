#pragma once

#include "SizeButton.h"

class ToggleButton : public SizeButton {
protected:
	bool _toggle;
public:
	virtual bool ButtonUp(PointF _mousePos, void* _data);

	bool(*toggle)(ToggleButton* _target, bool _isToggle, void* _data);

	ToggleButton(const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame,
		HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, CenterPointPos _centerPointPos = CenterPointPos::Center, bool _isVertical = false);

	bool SetToogle(bool _toggle);
	bool IsToggle()const;
};

