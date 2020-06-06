#pragma once

#include "../stdafx.h"


class Sampler  {
	friend class ImageBase;
	friend class Image3D;
	friend class ImageInstance;
	friend class ImageMultiInstance;
	friend class ImageInstance3D;
	friend class RTAnimateObjectBase;

#ifdef _WIN32
	static constexpr D3D11_FILTER DirectX11Filter[] = {
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
		D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
		D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_ANISOTROPIC
	};
	static constexpr D3D11_TEXTURE_ADDRESS_MODE DirectX11TextureAdressMode[] = {
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_BORDER,
		D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
	};
#elif __ANDROID__
#endif
public:
	enum class Filter {
		MinMagMipPoint,
		MinMagPoint_MipLinear,
		MinPoint_MagLinear_MipPoint,
		MinPoint_MagMipLinear,
		MinLinear_MagMipPoint,
		MinLinear_MagPoint_MipLinear,
		MinMagLinear_MipPoint,
		MinMagMipLinear,
		Anisotropic
	};
	enum class TextureAdressMode {
		Warp,
		Mirror,
		Clamp,
		Border,
		MirrorOnce
	};

	/*enum class MagFilter {
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	};
	enum class MinFilter {
		Nearest = GL_NEAREST,
		Linear=GL_LINEAR,
		NearestMipNearest = GL_NEAREST_MIPMAP_NEAREST,
		NearestMipLinear=GL_NEAREST_MIPMAP_LINEAR,
		LinearMipNearest=GL_LINEAR_MIPMAP_NEAREST,
		LinearMipLinear=GL_LINEAR_MIPMAP_LINEAR
	};
	enum class WarpMode {
		Repeat=GL_REPEAT,
		ClampToEdge=GL_CLAMP_TO_EDGE,
		MirroredRepeat=GL_MIRRORED_REPEAT
	};*/

	Sampler(Filter _filter = Filter::MinMagMipLinear, TextureAdressMode _textureAdressModeU = TextureAdressMode::Warp
		,TextureAdressMode _textureAdressModeV = TextureAdressMode::Warp, TextureAdressMode _textureAdressModeW = TextureAdressMode::Warp,
		unsigned _maxAnisotropy = 16);

	//void SetMinFilter(MinFilter _minFilter);
	//void SetMagFilter(MagFilter _magFilter);
	//void SetWrapModeU(WarpMode _warpModeU);
	//void SetWrapModeV(WarpMode _warpModeV);

	~Sampler();
private:
#ifdef _WIN32
	union {
		ID3D11SamplerState* sampler;
	};
#elif __ANDROID__
	void SetWarpMode(TextureAdressMode _mode, GLenum _name);

	GLuint sampler;
#endif
};