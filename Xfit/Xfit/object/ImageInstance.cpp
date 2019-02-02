#include "ImageInstance.h"
#include "../math/Point3Dw.h"
#include "../data/Array.h"
#include "../resource/Sampler.h"

#include "../_system/_GraphicsBase.h"


#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

using namespace _System::_OpenGL;

unsigned ImageInstance::GetWidth() const { return width; }
unsigned ImageInstance::GetHeight() const { return height; }

ImageInstance::ImageInstance() {
	openGL.posUV = 0;
	openGL.ins = 0;
}
ImageInstance::~ImageInstance() {
	if (openGL.posUV) {
		glDeleteBuffers(1, &openGL.posUV);
		glDeleteTextures(1, &openGL.texture);
	}
	if (openGL.ins) glDeleteBuffers(1, &openGL.ins);
}
void ImageInstance::Draw() {
	Object::Draw();

#ifdef _WIN32
	if (renderMode.activeShaderProg != imgInsVertProg) {
		glActiveShaderProgram(progPipeline, imgInsVertProg);
		renderMode.activeShaderProg = imgInsVertProg;
	}
	if (renderMode.vertProg != imgInsVertProg) {
		glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgInsVertProg);
		renderMode.vertProg = imgInsVertProg;
	}
	if (renderMode.fragProg != imgFragProg) {
		glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgFragProg);
		renderMode.fragProg = imgFragProg;
	}
#elif __ANDROID__
	if ((version.majorVersion >= 3) && (version.minorVersion >= 1)) {
		if (renderMode.activeShaderProg != imgInsVertProg) {
			glActiveShaderProgram(progPipeline, imgInsVertProg);
			renderMode.activeShaderProg = imgInsVertProg;
		}
		if (renderMode.vertProg != imgInsVertProg) {
			glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgInsVertProg);
			renderMode.vertProg = imgInsVertProg;
		}
		if (renderMode.fragProg != imgFragProg) {
			glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgFragProg);
			renderMode.fragProg = imgFragProg;
		}
	} else {
		if (renderMode.prog != imgInsProg) {
			glUseProgram(imgInsProg);
			renderMode.prog = imgInsProg;
		}
		}
#endif

	glUniformMatrix4fv(imgVert::matUniform, 1, GL_FALSE, mat.e);

	glBindBuffer(GL_ARRAY_BUFFER, openGL.posUV);

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

	glBindBuffer(GL_ARRAY_BUFFER, openGL.ins);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), 0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)offsetof(Matrix,e[4]));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)offsetof(Matrix, e[8]));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)offsetof(Matrix, e[12]));

	if (!renderMode.divisorSlot[2]) {
		glVertexAttribDivisor(2, 1);
		renderMode.divisorSlot[2] = true;
	}
	if (!renderMode.divisorSlot[3]) {
		glVertexAttribDivisor(3, 1);
		renderMode.divisorSlot[3] = true;
	}
	if (!renderMode.divisorSlot[4]) {
		glVertexAttribDivisor(4, 1);
		renderMode.divisorSlot[4] = true;
	}
	if (!renderMode.divisorSlot[5]) {
		glVertexAttribDivisor(5, 1);
		renderMode.divisorSlot[5] = true;
	}

	if (renderMode.activeTextureSlot != 0) {
		glActiveTexture(GL_TEXTURE0);
		renderMode.activeTextureSlot = 0;
	}
	glBindTexture(GL_TEXTURE_2D, openGL.texture);

	if (renderMode.sampler != sampler) {
		glBindSampler(0, sampler->sampler);
		renderMode.sampler = sampler;
	}

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4,(GLsizei)insNum);
}
void ImageInstance::Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs) {
#ifdef _DEBUG
	if (openGL.posUV);
#endif
	glGenBuffers(1, &openGL.posUV);
	glGenTextures(1, &openGL.texture);

	_System::PosUV2D vertexP[4] = { {_rect.GetPoint1(),_UVs[0]},{_rect.GetPoint2(),_UVs[1] },{_rect.GetPoint3(), _UVs[2]},{_rect.GetPoint4(), _UVs[3]} };
	glBindBuffer(GL_ARRAY_BUFFER, openGL.posUV);
#ifdef _WIN32
	if ((version.majorVersion >= 4) && (version.minorVersion >= 4)) {
		glBufferStorage(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, 0);
	} else {
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, GL_STATIC_DRAW);
	}
#elif __ANDROID__
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, GL_STATIC_DRAW);
#endif
	width = _width;
	height = _height;
	glBindTexture(GL_TEXTURE_2D, openGL.texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, _width, _height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _data);
}
void ImageInstance::Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect) {Build(_data, _width, _height, _rect, _System::defaultUV2D);}
void ImageInstance::BuildInstance(unsigned _maxInsNum, unsigned _insNum, Matrix* _insMatries) {
#ifdef _DEBUG
	if (openGL.ins);
#endif
	glGenBuffers(1, &openGL.ins);
	glBindBuffer(GL_ARRAY_BUFFER, openGL.ins);
	maxInsNum = _maxInsNum;
	insNum = _insNum;
	if (_maxInsNum == _insNum) {
#ifdef _WIN32
		if ((version.majorVersion >= 4) && (version.minorVersion >= 4)) {
			glBufferStorage(GL_ARRAY_BUFFER, _maxInsNum * sizeof(Matrix), _insMatries, GL_DYNAMIC_STORAGE_BIT);
		} else {
			glBufferData(GL_ARRAY_BUFFER, _maxInsNum * sizeof(Matrix), _insMatries, GL_DYNAMIC_DRAW);
		}
#elif __ANDROID__
		glBufferData(GL_ARRAY_BUFFER, _maxInsNum * sizeof(Matrix), _insMatries, GL_DYNAMIC_DRAW);
#endif
	} else {
#ifdef _WIN32
		if ((version.majorVersion >= 4) && (version.minorVersion >= 4)) {
			glBufferStorage(GL_ARRAY_BUFFER, _maxInsNum * sizeof(Matrix), nullptr, GL_DYNAMIC_STORAGE_BIT);
		} else {
			glBufferData(GL_ARRAY_BUFFER, _maxInsNum * sizeof(Matrix), nullptr, GL_DYNAMIC_DRAW);
		}
#elif __ANDROID__
		glBufferData(GL_ARRAY_BUFFER, _maxInsNum * sizeof(Matrix), nullptr, GL_DYNAMIC_DRAW);
#endif
		glBufferSubData(GL_ARRAY_BUFFER, 0, _insNum * sizeof(Matrix), _insMatries);
	}
}
void ImageInstance::UpdateInstance(unsigned _insNum, Matrix* _insMatries) {
#ifdef _DEBUG
	if (!openGL.ins);
#endif
	insNum = _insNum;
	glBindBuffer(GL_ARRAY_BUFFER, openGL.ins);
	glBufferSubData(GL_ARRAY_BUFFER, 0, _insNum * sizeof(Matrix), _insMatries);
}
