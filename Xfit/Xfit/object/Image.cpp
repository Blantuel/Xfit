#include "Image.h"
#include "../resource/Sampler.h"

#include "../_system/_GraphicsBase.h"

#include "../_system/_Windows.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

Image::Image() {
	openGL.posUV = 0;
	_System::_OpenGL::glGenBuffers(1, &openGL.posUV);
	glGenTextures(1, &openGL.texture);
}
Image::~Image() {
	_System::_OpenGL::glDeleteBuffers(1, &openGL.posUV);
	glDeleteTextures(1, &openGL.texture);
}
void Image::Draw() {
	if(_System::_OpenGL::renderMode.activeShaderProg!= _System::_OpenGL::imgVertProg) {
		_System::_OpenGL::glActiveShaderProgram(_System::_OpenGL::progPipeline, _System::_OpenGL::imgVertProg);
		_System::_OpenGL::renderMode.activeShaderProg = _System::_OpenGL::imgVertProg;
	}
	_System::_OpenGL::glUniformMatrix4fv(_System::_OpenGL::imgVert::matUniform, 1, GL_FALSE, mat.e);

	_System::_OpenGL::glBindBuffer(GL_ARRAY_BUFFER, openGL.posUV);

	_System::_OpenGL::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(_System::PosUV2D), 0);
	_System::_OpenGL::glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(_System::PosUV2D), (void*)offsetof(_System::PosUV2D, UV));

	if (_System::_OpenGL::renderMode.divisorSlot[0]) {
		_System::_OpenGL::glVertexAttribDivisor(0, 0);
		_System::_OpenGL::renderMode.divisorSlot[0] = false;
	}
	if (_System::_OpenGL::renderMode.divisorSlot[1]) {
		_System::_OpenGL::glVertexAttribDivisor(1, 0);
		_System::_OpenGL::renderMode.divisorSlot[1] = false;
	}

	if (_System::_OpenGL::renderMode.activeTextureSlot != 0) {
		_System::_OpenGL::glActiveTexture(GL_TEXTURE0);
		_System::_OpenGL::renderMode.activeTextureSlot = 0;
	}
	glBindTexture(GL_TEXTURE_2D, openGL.texture);

	if (_System::_OpenGL::renderMode.sampler != sampler) {
		_System::_OpenGL::glBindSampler(0, sampler->sampler);
		_System::_OpenGL::renderMode.sampler = sampler;
	}

	if (_System::_OpenGL::renderMode.vertProg != _System::_OpenGL::imgVertProg) {
		_System::_OpenGL::glUseProgramStages(_System::_OpenGL::progPipeline, GL_VERTEX_SHADER_BIT, _System::_OpenGL::imgVertProg);
		_System::_OpenGL::renderMode.vertProg = _System::_OpenGL::imgVertProg;
	}
	if (_System::_OpenGL::renderMode.fragProg != _System::_OpenGL::imgFragProg) {
		_System::_OpenGL::glUseProgramStages(_System::_OpenGL::progPipeline, GL_FRAGMENT_SHADER_BIT, _System::_OpenGL::imgFragProg);
		_System::_OpenGL::renderMode.fragProg = _System::_OpenGL::imgFragProg;
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
void Image::Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs) {
#ifdef _DEBUG
	if (openGL.posUV);
#endif
	_System::_OpenGL::glGenBuffers(1, &openGL.posUV);
	glGenTextures(1, &openGL.texture);

	_System::PosUV2D vertexP[4] = { {_rect.GetPoint1(),_UVs[0]},{_rect.GetPoint2(),_UVs[1] },{_rect.GetPoint3(), _UVs[2]},{_rect.GetPoint4(), _UVs[3]} };

	_System::_OpenGL::glBindBuffer(GL_ARRAY_BUFFER, openGL.posUV);
	_System::_OpenGL::glBufferStorage(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, 0);

	glBindTexture(GL_TEXTURE_2D, openGL.texture);
	_System::_OpenGL::glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, _width, _height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _data);
}
void Image::Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect) {Build(_data, _width, _height, _rect, _System::defaultUV2D);}