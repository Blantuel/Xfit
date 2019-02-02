#include "Button.h"
#include "../system/System.h"
#include "../_system/_OpenGL.h"
#include "../data/Array.h"
#include "../resource/Sampler.h"
#include "../_system/_GraphicsBase.h"
#include "../system/Input.h"

using namespace _System::_OpenGL;


void Button::DrawImage(GLuint _posUV, GLuint _texture) {
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

	glBindBuffer(GL_ARRAY_BUFFER, _posUV);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	if (renderMode.divisorSlot[0]) {
		glVertexAttribDivisor(0, 0);
		renderMode.divisorSlot[0] = false;
	}

	if (renderMode.activeTextureSlot != 0) {
		glActiveTexture(GL_TEXTURE0);
		renderMode.activeTextureSlot = 0;
	}
	glBindTexture(GL_TEXTURE_2D, _texture);

	if (renderMode.sampler != sampler) {
		glBindSampler(0, sampler->sampler);
		renderMode.sampler = sampler;
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

Button::Button() :state(State::UP),buttonOver(nullptr),buttonDown(nullptr),buttonUp(nullptr) {
	openGL.upPosUV = 0;
	openGL.overPosUV = 0;
	openGL.downPosUV = 0;
	openGL.disablePosUV = 0;
}
void Button::Disible(bool _on) {
	if (_on) state = State::DISABLE;
	else state = State::UP;
}
Button::State Button::GetState() const { return state; }

Button::~Button() {
	if (openGL.upPosUV) {
		glDeleteBuffers(1, &openGL.upPosUV);
		glDeleteTextures(1, &openGL.upTexture);
	}
	if (openGL.overPosUV) {
		glDeleteBuffers(1, &openGL.overPosUV);
		glDeleteTextures(1, &openGL.overTexture);
	}
	if (openGL.downPosUV) {
		glDeleteBuffers(1, &openGL.downPosUV);
		glDeleteTextures(1, &openGL.downTexture);
	}
	if (openGL.disablePosUV) {
		glDeleteBuffers(1, &openGL.disablePosUV);
		glDeleteTextures(1, &openGL.disableTexture);
	}
}
void Button::Update() {
	if (state != State::DISABLE) {
#ifdef _DEBUG
		if (!collision);
#endif
		PointF mousePos = Input::GetMousePos();
		if (collision->HitTest(mousePos)) {
			if (Input::IsLMouseClick()) {
				state = State::DOWN;
				if (buttonDown)buttonDown(this, mousePos);
				ButtonDown(mousePos);
			} else if (Input::IsLMouseClicked()) {
				state = State::OVER;
				if (buttonUp)buttonUp(this, mousePos);
				ButtonUp(mousePos);
			} else if ((state != State::DOWN) && (state != State::OVER)) {
				state = State::OVER;
				if (buttonOver)buttonOver(this, mousePos);
				ButtonOver(mousePos);
			}
		} else {
			state = State::UP;
		}
	}
}
void Button::Draw() {
	Object::Draw();
	GLuint posUV;
	GLuint texture;
	switch (state) {
	case State::UP:
#ifdef _DEBUG
		if (!openGL.upPosUV);
#endif
		up:
		posUV = openGL.upPosUV;
		texture = openGL.upTexture;
		break;
	case State::OVER:
		over:
		if (!openGL.overPosUV)goto up;
		posUV = openGL.overPosUV;
		texture = openGL.overTexture;
		break;
	case State::DOWN:
		if (!openGL.downPosUV)goto over;
		posUV = openGL.downPosUV;
		texture = openGL.downTexture;
		break;
	case State::DISABLE:
		if (!openGL.disablePosUV)goto up;
		posUV = openGL.disablePosUV;
		texture = openGL.disableTexture;
		break;
#ifdef _DEBUG
	default:;
#endif
	}
	DrawImage(posUV, texture);
}
void Button::UpBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs) {
	Build(_data, _width, _height, _rect, _UVs, &openGL.upPosUV, &openGL.upTexture);
}
void Button::UpBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect) {
	UpBuild(_data, _width, _height, _rect, _System::defaultUV2D);
}

void Button::OverBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs) {
	Build(_data, _width, _height, _rect, _UVs, &openGL.overPosUV, &openGL.overTexture);
}
void Button::OverBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect) {
	OverBuild(_data, _width, _height, _rect, _System::defaultUV2D);
}

void Button::DownBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs) {
	Build(_data, _width, _height, _rect, _UVs, &openGL.downPosUV, &openGL.downTexture);
}
void Button::DownBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect) {
	DownBuild(_data, _width, _height, _rect, _System::defaultUV2D);
}

void Button::DisableBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs) {
	Build(_data, _width, _height, _rect, _UVs, &openGL.disablePosUV, &openGL.disableTexture);
}
void Button::DisableBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect) {
	DisableBuild(_data, _width, _height, _rect, _System::defaultUV2D);
}

void Button::Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs, GLuint* _outPosUV, GLuint* _outTexture) {
#ifdef _DEBUG
	if (*_outPosUV);
#endif
	glGenBuffers(1, _outPosUV);
	glGenTextures(1, _outTexture);

	_System::PosUV2D vertexP[4] = { {_rect.GetPoint1(),_UVs[0]},{_rect.GetPoint2(),_UVs[1] },{_rect.GetPoint3(), _UVs[2]},{_rect.GetPoint4(), _UVs[3]} };

	glBindBuffer(GL_ARRAY_BUFFER, *_outPosUV);
#ifdef _WIN32
	glBufferStorage(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, 0);
#elif __ANDROID__
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, GL_STATIC_DRAW);
#endif

	glBindTexture(GL_TEXTURE_2D, *_outTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, _width, _height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _data);
}