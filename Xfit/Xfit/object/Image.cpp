#include "Image.h"
#include "../resource/Sampler.h"

#include "../_system/_GraphicsBase.h"

#include "../_system/_Windows.h"
#include "../_system/_Android.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

using namespace _System::_OpenGL;

Image::Image() {
	posUV = 0;
	glGenBuffers(1, &posUV);
	glGenTextures(1, &texture);
}
Image::~Image() {
	glDeleteBuffers(1, &posUV);
	glDeleteTextures(1, &texture);
}
void Image::Draw() {
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
#elif __ANDROID__
	if ((version.majorVersion >= 3) && (version.minorVersion >= 1)) {
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
	} else {
		if (renderMode.prog != imgProg) {
			glUseProgram(imgProg);
			renderMode.prog = imgProg;
		}
		}
#endif

	glUniformMatrix4fv(_System::_OpenGL::imgVert::matUniform, 1, GL_FALSE, mat.e);

	glBindBuffer(GL_ARRAY_BUFFER, posUV);

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
	glBindTexture(GL_TEXTURE_2D, texture);

	if (renderMode.sampler != sampler) {
		glBindSampler(0, sampler->sampler);
		renderMode.sampler = sampler;
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
void Image::Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs) {
#ifdef _DEBUG
	if (posUV);
#endif
	_System::PosUV2D vertexP[4] = { {_rect.GetPoint1(),_UVs[0]},{_rect.GetPoint2(),_UVs[1] },{_rect.GetPoint3(), _UVs[2]},{_rect.GetPoint4(), _UVs[3]} };

	glBindBuffer(GL_ARRAY_BUFFER, posUV);

#ifdef _WIN32
	if ((version.majorVersion >= 4) && (version.minorVersion >= 4)) {
		glBufferStorage(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, 0);
	} else {
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, GL_STATIC_DRAW);
	}
#elif __ANDROID__
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(_System::PosUV2D), vertexP, GL_STATIC_DRAW);
#endif


	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, _width, _height);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _data);
}
void Image::Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect) {Build(_data, _width, _height, _rect, _System::defaultUV2D);}
void Image::BuildEdit(const void* _data, unsigned _width, unsigned _height, unsigned _offsetX/* = 0*/, unsigned _offsetY/* = 0*/) {
#ifdef _DEBUG
	if (!posUV);
#endif
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, _offsetX, _offsetY, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _data);
}