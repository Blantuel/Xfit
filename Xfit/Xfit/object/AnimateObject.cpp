#include "AnimateObject.h"
#include "../_system/_GraphicsBase.h"
#include "../_system/_Windows.h"
#include "../data/Memory.h"
#include "../resource/Sampler.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

AnimateObject::AnimateObject() :cFrame(0), playing(false), len(0) {}
AnimateObject::~AnimateObject() {
	if (len > 0) {
		_System::_OpenGL::glDeleteBuffers(len, openGL.posUVs);
		glDeleteTextures(len, openGL.textures);
		delete[]openGL.posUVs;
		delete[]openGL.textures;
	}
}
void AnimateObject::Play() { playing = true; }
void AnimateObject::Stop() { playing = false; }
bool AnimateObject::IsPlay()const { return playing; }
void AnimateObject::GoToAndStop(unsigned _frame/* = 0*/) { cFrame = _frame; playing = false; }
void AnimateObject::GoToAndPlay(unsigned _frame/* = 0*/) {
	cFrame = _frame;
	playing = true;
}
unsigned AnimateObject::NextFrame() {
	cFrame++;
	return cFrame;
}
unsigned AnimateObject::PrevFrame() {
	cFrame--;
	return cFrame;
}
unsigned AnimateObject::GetCurrectFrame()const { return cFrame; }
unsigned AnimateObject::GetTotalFrame()const { return len; }
void AnimateObject::Draw() {
	Object::Draw();
	if (_System::_OpenGL::renderMode.activeShaderProg != _System::_OpenGL::imgVertProg) {
		_System::_OpenGL::glActiveShaderProgram(_System::_OpenGL::progPipeline, _System::_OpenGL::imgVertProg);
		_System::_OpenGL::renderMode.activeShaderProg = _System::_OpenGL::imgVertProg;
	}
	_System::_OpenGL::glUniformMatrix4fv(_System::_OpenGL::imgVert::matUniform, 1, GL_FALSE, mat.e);

	_System::_OpenGL::glBindBuffer(GL_ARRAY_BUFFER, openGL.posUVs[cFrame]);

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
	glBindTexture(GL_TEXTURE_2D, openGL.textures[cFrame]);

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
void AnimateObject::Build(unsigned _len, const void *const* _datas, const unsigned * _widths, const unsigned * _heights, const RectF * _rects, const PointF *const* _UVs){
#ifdef _DEBUG
	if (len > 0);
#endif
	len = _len;

	openGL.posUVs = new GLuint[len];
	openGL.textures = new GLuint[len];
	_System::_OpenGL::glGenBuffers(len, openGL.posUVs);
	glGenTextures(len, openGL.textures);

	for (unsigned i = 0; i < len; i++) {
		_System::PosUV2D vertexP[4] = { {_rects[i].GetPoint1(),_UVs[i][0]},{_rects[i].GetPoint2(),_UVs[i][1] },{_rects[i].GetPoint3(), _UVs[i][2]},{_rects[i].GetPoint4(), _UVs[i][3]} };

		_System::_OpenGL::glBindBuffer(GL_ARRAY_BUFFER, openGL.posUVs[i]);
		_System::_OpenGL::glBufferStorage(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, 0);

		glBindTexture(GL_TEXTURE_2D, openGL.textures[i]);
		_System::_OpenGL::glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, _widths[i], _heights[i]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _widths[i], _heights[i], GL_RGBA, GL_UNSIGNED_BYTE, _datas[i]);
	}
}
void AnimateObject::Build(unsigned _len, const void *const* _datas, const unsigned * _widths, const unsigned * _heights, const RectF * _rects){
#ifdef _DEBUG
	if (len > 0);
#endif
	PointF** UVs = new PointF*[_len];
	for (unsigned i = 0; i < _len; i++) {
		UVs[i] = _System::defaultUV2D;
	}
	Build(_len, _datas, _widths, _heights, _rects, UVs);
	delete[]UVs;
}
void AnimateObject::Update() {
	if (playing) {
		if (cFrame >= (len - 1)) {
			playing = false;
		} else {
			cFrame++;
		}
	}
}