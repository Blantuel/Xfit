#pragma once

#include "sampler.h"
#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"
#include "../_system/_DirectX11.h"

#ifdef __ANDROID__

using namespace _System::_OpenGL;

void Sampler::SetWarpMode(TextureAdressMode _mode, GLenum _name) {
	switch(_mode) {
		case TextureAdressMode::Clamp:
			glSamplerParameteri(sampler, _name, GL_CLAMP_TO_EDGE);
			break;
		case TextureAdressMode::Warp:
			glSamplerParameteri(sampler, _name, GL_REPEAT);
			break;
		case TextureAdressMode::Mirror:
			glSamplerParameteri(sampler, _name, GL_MIRRORED_REPEAT);
			break;
		case TextureAdressMode::Border:
			glSamplerParameteri(sampler, _name, GL_CLAMP_TO_BORDER);
			break;
		case TextureAdressMode::MirrorOnce:
			break;
	}
}

Sampler::Sampler(Filter _filter /*= Filter::MinMagMipLinear*/, TextureAdressMode _textureAdressModeU /*= TextureAdressMode::Clamp*/
		,TextureAdressMode _textureAdressModeV /*= TextureAdressMode::Clamp*/, TextureAdressMode _textureAdressModeW /*= TextureAdressMode::Clamp*/,
				 unsigned _maxAnisotropy /*= 16*/) {
	glGenSamplers(1, &sampler);
	glBindSampler(0, sampler);

	switch(_filter) {
		case Filter::MinMagMipPoint:
			glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case Filter::MinMagPoint_MipLinear:
			glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case Filter::MinPoint_MagLinear_MipPoint:
			glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case Filter::MinPoint_MagMipLinear:
			glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case Filter::MinLinear_MagMipPoint:
			glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case Filter::MinLinear_MagPoint_MipLinear:
			glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case Filter::MinMagLinear_MipPoint:
			glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case Filter::MinMagMipLinear:
			glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case Filter::Anisotropic:
			break;
	}
	SetWarpMode(_textureAdressModeU, GL_TEXTURE_WRAP_S);
	SetWarpMode(_textureAdressModeV, GL_TEXTURE_WRAP_T);
	SetWarpMode(_textureAdressModeW, GL_TEXTURE_WRAP_R);
}

/*void Sampler::SetMinFilter(MinFilter _minFilter) {
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
}*/

Sampler::~Sampler() {
	glDeleteSamplers(1, &sampler);
}

#elif _WIN32

using namespace _System::_DirectX11;

Sampler::Sampler(Filter _filter /*= Filter::MinMagMipLinear*/, TextureAdressMode _textureAdressModeU /*= TextureAdressMode::Clamp*/
	,TextureAdressMode _textureAdressModeV /*= TextureAdressMode::Clamp*/, TextureAdressMode _textureAdressModeW /*= TextureAdressMode::Clamp*/,
	unsigned _maxAnisotropy /*= 16*/) {
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = DirectX11Filter[(int)_filter];
	samplerDesc.AddressU = DirectX11TextureAdressMode[(int)_textureAdressModeU];
	samplerDesc.AddressV = DirectX11TextureAdressMode[(int)_textureAdressModeV];
	samplerDesc.AddressW = DirectX11TextureAdressMode[(int)_textureAdressModeW];
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = _maxAnisotropy;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.f;
	samplerDesc.BorderColor[1] = 1.f;
	samplerDesc.BorderColor[2] = 1.f;
	samplerDesc.BorderColor[3] = 1.f;

	device->CreateSamplerState(&samplerDesc, &sampler);
}
Sampler::~Sampler() {
	sampler->Release();
}
#endif
