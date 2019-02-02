#pragma once

#include "sampler.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

using namespace _System::_OpenGL;

Sampler::Sampler() {
	glGenSamplers(1, &sampler);
}

void Sampler::SetMinFilter(MinFilter _minFilter) {
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, (GLint)_minFilter);
}
void Sampler::SetMagFilter(MagFilter _magFilter) {
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, (GLint)_magFilter);
}
void Sampler::SetWrapModeU(WarpMode _warpModeU) {
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, (GLint)_warpModeU);
}
void Sampler::SetWrapModeV(WarpMode _warpModeV) {
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, (GLint)_warpModeV);
}
Sampler::~Sampler() {
	glDeleteSamplers(1, &sampler);
}