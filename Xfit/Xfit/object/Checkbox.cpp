#include "Checkbox.h"
#include "../_system/_GraphicsBase.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

using namespace _System::_OpenGL;

void Checkbox::ButtonUp(PointF _mousePos) { SetCheck(!IsChecked());}
Checkbox::Checkbox():checked(false) {openGL.checkPosUV = 0;}
Checkbox::~Checkbox() {
	if (openGL.checkPosUV) {
		glGenBuffers(1, &openGL.checkPosUV);
		glGenTextures(1, &openGL.checkTexture);
	}
}
bool Checkbox::IsChecked() const { return checked; }
void Checkbox::SetCheck(bool _on) { checked = _on; }
void Checkbox::Draw() {
#ifdef _DEBUG
	if (!openGL.checkPosUV);
#endif
	Button::Draw();
	if (checked) Button::DrawImage(openGL.checkPosUV, openGL.checkTexture);
}
void Checkbox::CheckBuild(const void * _data, unsigned _width, unsigned _height, const RectF & _rect, const PointF * _UVs) {
	Build(_data, _width, _height, _rect, _UVs, &openGL.checkPosUV, &openGL.checkTexture);
}
void Checkbox::CheckBuild(const void * _data, unsigned _width, unsigned _height, const RectF & _rect) {
	CheckBuild(_data, _width, _height, _rect, _System::defaultUV2D);
}