#pragma once

#include "sampler.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

Sampler::Sampler() {
	_System::_OpenGL::glGenSamplers(1, &sampler);
}

void Sampler::SetMinFilter(MinFilter _minFilter) {
	_System::_OpenGL::glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, (GLint)_minFilter);
}
void Sampler::SetMagFilter(MagFilter _magFilter) {
	_System::_OpenGL::glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, (GLint)_magFilter);
}
void Sampler::SetWrapModeU(WarpMode _warpModeU) {
	_System::_OpenGL::glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, (GLint)_warpModeU);
}
void Sampler::SetWrapModeV(WarpMode _warpModeV) {
	_System::_OpenGL::glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, (GLint)_warpModeV);
}
Sampler::~Sampler() {
	_System::_OpenGL::glDeleteSamplers(1, &sampler);
}