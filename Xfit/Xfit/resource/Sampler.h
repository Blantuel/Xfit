#pragma once

#include "../stdafx.h"


class Sampler  {
	friend class Button;
	friend class Image;
	friend class Image3D;
	friend class ImageInstance;
	friend class ImageInstance3D;
	friend class AnimateObject;

public:
#ifdef OPENGL
	enum class MagFilter {
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
	};
#elif VULKAN
#endif
	Sampler();

	void SetMinFilter(MinFilter _minFilter);
	void SetMagFilter(MagFilter _magFilter);
	void SetWrapModeU(WarpMode _warpModeU);
	void SetWrapModeV(WarpMode _warpModeV);

	~Sampler();
private:
	GLuint sampler;
};