#include "Object.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

#include "../effect/Blend.h"

void Object::Draw() {
	if (_System::_OpenGL::renderMode.blend != blend) {
		const Blend* const blend2 = _System::_OpenGL::renderMode.blend;
		if (!blend2) {
			if (blend) {
				glEnable(GL_BLEND);
				_System::_OpenGL::glBlendFuncSeparate((GLenum)blend->srcColor, (GLenum)blend->dstColor, (GLenum)blend->dstColor, (GLenum)blend->dstAlpha);
				_System::_OpenGL::glBlendEquationSeparate((GLenum)blend->colorEquation, (GLenum)blend->alphaEquation);
				_System::_OpenGL::glBlendColor(blend->constants[0], blend->constants[1], blend->constants[2], blend->constants[3]);
			} else {
				glDisable(GL_BLEND);
			}
		} else {
			if (blend) {
				if ((blend2->srcColor != blend->srcColor) || (blend2->dstColor != blend->dstColor) || (blend2->srcAlpha != blend->dstColor) || (blend2->dstAlpha != blend->dstAlpha)) {
					_System::_OpenGL::glBlendFuncSeparate((GLenum)blend->srcColor, (GLenum)blend->dstColor, (GLenum)blend->dstColor, (GLenum)blend->dstAlpha);
				}
				if ((blend2->colorEquation != blend->colorEquation) || (blend2->alphaEquation != blend->alphaEquation)) {
					_System::_OpenGL::glBlendEquationSeparate((GLenum)blend->colorEquation, (GLenum)blend->alphaEquation);
				}
				if ((blend2->constants[0] != blend->constants[0]) || (blend2->constants[1] != blend->constants[1]) || (blend2->constants[2] != blend->constants[2]) || (blend2->constants[3] != blend->constants[3])) {
					_System::_OpenGL::glBlendColor(blend->constants[0], blend->constants[1], blend->constants[2], blend->constants[3]);
				}
			} else {
				glDisable(GL_BLEND);
			}
		}
		
		_System::_OpenGL::renderMode.blend = blend;
	}
}