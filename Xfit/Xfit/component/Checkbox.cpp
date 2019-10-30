#include "Checkbox.h"
#include "../system/System.h"

Checkbox::Checkbox(const CheckFrame& _checkFrame, const ButtonFrame& _upFrame, const ButtonFrame& _overFrame, const ButtonFrame& _downFrame, const ButtonFrame& _disableFrame, HitTest* _hitTest,
	PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler) :checked(false), checkFrame(_checkFrame), Button(_upFrame, _overFrame, _downFrame, _disableFrame, _hitTest, _pos, _scale, _rotation, _blend, _sampler) {
}
bool Checkbox::ButtonUp(PointF _mousePos, void* _data) { SetCheck(!IsChecked()); return true; }
Checkbox::Checkbox() : checked(false) {
#ifdef _DEBUG
	checkFrame.frame = nullptr;
	checkFrame.vertex = nullptr;
#endif
	checkFrame.uv = System::defaultUV;
}
bool Checkbox::IsChecked() const { return checked; }
void Checkbox::SetCheck(bool _on) { checked = _on; }
void Checkbox::Draw() {
#ifdef _DEBUG
	if (!checkFrame.vertex)throw CheckBoxError(CheckBoxError::Code::NullCheckVertex);
	if (!checkFrame.uv)throw CheckBoxError(CheckBoxError::Code::NullCheckUV);
	if (!checkFrame.frame)throw CheckBoxError(CheckBoxError::Code::NullCheckFrame);
#endif
	Button::Draw();
	if (visible&&checked) Button::DrawImage(checkFrame.vertex, checkFrame.uv, checkFrame.index,checkFrame.frame);
}