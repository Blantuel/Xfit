#include "ToggleButton.h"


bool ToggleButton::ButtonUp(PointF _mousePos, void* _data) {
	return SetToogle(!_toggle);
}

bool ToggleButton::SetToogle(bool _toggle) {
	this->_toggle = _toggle;
	if (toggle) {
		return toggle(this, this->_toggle, nullptr);
	}
	return false;
}
bool ToggleButton::IsToggle()const { return _toggle; }
ToggleButton::ToggleButton(const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame,
	HitTest* _hitTest, PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/, bool _isVertical /*= false*/) :
	SizeButton(_upFrame, _overFrame, _downFrame, _disableFrame, _hitTest, _pos, _scale, _rotation, _blend, _sampler, _centerPointPos, _isVertical), _toggle(false), toggle(nullptr) {
}