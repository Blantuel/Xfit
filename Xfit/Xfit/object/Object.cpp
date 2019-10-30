#include "Object.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"
#include "../_system/_DirectX11.h"

#include "../system/System.h"

#include "../effect/Blend.h"

#ifdef _WIN32
using namespace _System::_DirectX11;
#elif __ANDROID__
using namespace _System::_OpenGL;
#endif

Object::Object(Blend* _blend):blend(_blend),visible(true) {}
Object::Object():blend(System::defaultBlend),visible(true) {}
void Object::Draw() {
#ifdef _WIN32
	if (blend) {
		context->OMSetBlendState(blend->blendState, blend->consts, 0xffffffff);
	} else {
		const float noBlendFactor[4] = { 0.f,0.f,0.f,0.f };
		context->OMSetBlendState(nullptr, noBlendFactor, 0xffffffff);//블랜딩 없음.
	}
#elif __ANDROID__
	if (blend) {
		glEnable(GL_BLEND);
		glBlendFuncSeparate((GLenum)blend->srcColor, (GLenum)blend->destColor, (GLenum)blend->destColor, (GLenum)blend->destAlpha);
		glBlendEquationSeparate((GLenum)blend->colorEquation, (GLenum)blend->alphaEquation);
		glBlendColor(blend->consts[0], blend->consts[1], blend->consts[2], blend->consts[3]);
	} else {
		glDisable(GL_BLEND);
	}
#endif
}