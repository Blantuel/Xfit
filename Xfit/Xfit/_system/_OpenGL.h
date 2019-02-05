#pragma once

#include "../system/System.h"

class Sampler;
class Blend;

namespace _System::_OpenGL {
	inline bool vSync;
	inline unsigned msaaCount, msaaQuality;
	inline float refleshRate;
	inline System::RendererVersion version = { System::RendererName::None,0,0 };
	inline unsigned versionNumber;

	void Release();

	void Init(System::RendererInfo* _info);

	void Resize();

#ifdef _WIN32
	inline HGLRC context;
	inline HDC tempDC;
	inline HWND tempHWnd = nullptr;//(is nullptr and version init) == RendererInited
	inline HGLRC tempContext;

	inline thread_local HGLRC threadContext = nullptr;
#elif __ANDROID__
#endif

	inline GLuint imgProg;
	inline GLuint imgInsProg;

	inline GLuint imgVertProg;
	inline GLuint imgInsVertProg;
	inline GLuint imgFragProg;
	inline GLuint progPipeline;

	inline GLuint vao;

	inline GLint uniformShapeColor;

	namespace img {
		inline GLint matUniform;
		inline GLint samplerUniform;
	}
	namespace imgIns {
		inline GLint matUniform;
		inline GLint samplerUniform;
	}

	namespace imgVert {
		inline GLint matUniform;
	}
	namespace imgFrag {
		inline GLint samplerUniform;
	}
	namespace imgInsVert {
		inline GLint matUniform;
	}
	struct RenderMode {
		GLint activeShaderProg;
		GLint vertProg;
		GLint fragProg;
		GLint prog;
		Blend* blend;
		Sampler* sampler;
		unsigned short activeTextureSlot;
		bool divisorSlot[8];
	};

	inline RenderMode renderMode = {
		INT_MAX,
		INT_MAX,
		INT_MAX,
		INT_MAX,
		nullptr,
		nullptr,
		USHRT_MAX,
	{false,false,false,false,false,false,false,false}
	};


#ifdef _WIN32
	inline PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
	inline PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
	inline PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

	inline PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
	inline PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
	inline PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
	inline PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
	inline PFNGLBUFFERDATAPROC glBufferData = nullptr;
	inline PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
	inline PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
	inline PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
	inline PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
	inline PFNGLBUFFERSTORAGEPROC glBufferStorage = nullptr;
	inline PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;

	inline PFNGLCREATESHADERPROC glCreateShader = nullptr;
	inline PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
	inline PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
	inline PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
	inline PFNGLATTACHSHADERPROC glAttachShader = nullptr;
	inline PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
	inline PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
	inline PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
	inline PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
	inline PFNGLDETACHSHADERPROC glDetachShader = nullptr;
	inline PFNGLDELETESHADERPROC glDeleteShader = nullptr;

	inline PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
	inline PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
	inline PFNGLUNIFORM1FPROC glUniform1f = nullptr;
	inline PFNGLUNIFORM1IPROC glUniform1i = nullptr;
	inline PFNGLUNIFORM4FPROC glUniform4f = nullptr;
	inline PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
	inline PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;

	inline PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
	inline PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
	inline PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
	inline PFNGLGENSAMPLERSPROC glGenSamplers = nullptr;
	inline PFNGLDELETESAMPLERSPROC glDeleteSamplers = nullptr;
	inline PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = nullptr;
	inline PFNGLBINDSAMPLERPROC glBindSampler = nullptr;
	inline PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines = nullptr;
	inline PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline = nullptr;
	inline PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines = nullptr;
	inline PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages = nullptr;
	inline PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = nullptr;
	inline PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
	inline PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram = nullptr;

	inline PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = nullptr;
	inline PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = nullptr;

	inline PFNGLTEXSTORAGE2DPROC glTexStorage2D = nullptr;

	inline PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;
	inline PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = nullptr;
	inline PFNGLBLENDCOLORPROC glBlendColor = nullptr;

#ifdef _DEBUG
	inline PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
#endif

#elif __ANDROID__
#endif

	inline thread_local bool contextLinked = false;
}