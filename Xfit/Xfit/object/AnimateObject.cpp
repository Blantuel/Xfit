#include "AnimateObject.h"
#include "../_system/_GraphicsBase.h"
#include "../_system/_Windows.h"
#include "../_system/_Android.h"
#include "../data/Memory.h"
#include "../resource/Sampler.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

using namespace _System::_OpenGL;

AnimateObject::AnimateObject() :cFrame(0), playing(false), len(0) {}
AnimateObject::~AnimateObject() {
	if (len > 0) {
		glDeleteBuffers(len, openGL.posUVs);
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

#ifdef _WIN32
	if (renderMode.activeShaderProg != imgVertProg) {
		glActiveShaderProgram(progPipeline, imgVertProg);
		renderMode.activeShaderProg = imgVertProg;
	}
	if (renderMode.vertProg != imgVertProg) {
		glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgVertProg);
		renderMode.vertProg = imgVertProg;
	}
	if (renderMode.fragProg != imgFragProg) {
		glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgFragProg);
		renderMode.fragProg = imgFragProg;
	}
	glUniformMatrix4fv(imgVert::matUniform, 1, GL_FALSE, mat.e);
#elif __ANDROID__
	if (versionNumber >= 301) {
		if (renderMode.activeShaderProg != imgVertProg) {
			glActiveShaderProgram(progPipeline, imgVertProg);
			renderMode.activeShaderProg = imgVertProg;
		}
		if (renderMode.vertProg != imgVertProg) {
			glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgVertProg);
			renderMode.vertProg = imgVertProg;
		}
		if (renderMode.fragProg != imgFragProg) {
			glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgFragProg);
			renderMode.fragProg = imgFragProg;
		}
		glUniformMatrix4fv(imgVert::matUniform, 1, GL_FALSE, mat.e);
	} else {
		if (renderMode.prog != imgProg) {
			glUseProgram(imgProg);
			renderMode.prog = imgProg;
		}
		glUniformMatrix4fv(img::matUniform, 1, GL_FALSE, mat.e);
	}
#endif


	glBindBuffer(GL_ARRAY_BUFFER, openGL.posUVs[cFrame]);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(_System::PosUV2D), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(_System::PosUV2D), (void*)offsetof(_System::PosUV2D, UV));

	if (renderMode.divisorSlot[0]) {
		glVertexAttribDivisor(0, 0);
		renderMode.divisorSlot[0] = false;
	}
	if (renderMode.divisorSlot[1]) {
		glVertexAttribDivisor(1, 0);
		renderMode.divisorSlot[1] = false;
	}

	if (renderMode.activeTextureSlot != 0) {
		glActiveTexture(GL_TEXTURE0);
		renderMode.activeTextureSlot = 0;
	}
	glBindTexture(GL_TEXTURE_2D, openGL.textures[cFrame]);

	if (renderMode.sampler != sampler) {
		glBindSampler(0, sampler->sampler);
		renderMode.sampler = sampler;
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
	glGenBuffers(len, openGL.posUVs);
	glGenTextures(len, openGL.textures);

	for (unsigned i = 0; i < len; i++) {
		_System::PosUV2D vertexP[4] = { {_rects[i].GetPoint1(),_UVs[i][0]},{_rects[i].GetPoint2(),_UVs[i][1] },{_rects[i].GetPoint3(), _UVs[i][2]},{_rects[i].GetPoint4(), _UVs[i][3]} };

		glBindBuffer(GL_ARRAY_BUFFER, openGL.posUVs[i]);
#ifdef _WIN32
		if (glBufferStorage) {
			glBufferStorage(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, 0);
		} else {
			glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, GL_STATIC_DRAW);
		}
#elif __ANDROID__
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, GL_STATIC_DRAW);
#endif

		glBindTexture(GL_TEXTURE_2D, openGL.textures[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, _widths[i], _heights[i]);
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