#include "_OpenGL.h"

#ifdef _WIN32
#include "_Windows.h"
#elif __ANDROID__
#include "_Android.h"
#endif

#include "_Loop.h"

#ifdef _DEBUG
#include "../file/File.h"
#else
#endif


namespace _System::_OpenGL {
#ifdef _WIN32
#define LOADOPENGLFUNCTION(_name) {_name = (decltype(_name))wglGetProcAddress(#_name);}
#elif __ANDROID__
#endif

#ifdef _WIN32
	static void APIENTRY OpenglDebugCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const GLchar* _message, const void* _userParam) {
#elif __ANDROID__
#endif
	string str = "!!XFit OpenGL DebugLog ";

	switch (_source) {
	case GL_DEBUG_SOURCE_API:
		str += "source=API ";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		str += "source=WINDOW SYSTEM ";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		str += "source=SHADER COMPILER ";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		str += "source=THIRD PARTY ";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		str += "source=APPLICATION ";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		str += "source=OTHER ";
		break;
	default:
		str += "source=UNKNOWN ";
		break;
	}
	switch (_type) {
	case GL_DEBUG_TYPE_ERROR:
		str += "type=ERROR id=";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		str += "type=DEPRECATED_BEHAVIOR id=";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		str += "type=UNDEFINED_BEHAVIOR id=";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		str += "type=PORTABILITY id=";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		str += "type=PERFORMANCE id=";
		break;
	case GL_DEBUG_TYPE_OTHER:
		str += "type=OTHER id=";
		break;
	default:
		str += "type=UNKNOWN id=";
		break;
	}
	char idStr[20];
	_itoa_s(_id, idStr, 20, 10);
	str += idStr;
	switch (_severity) {
	case GL_DEBUG_SEVERITY_LOW:
		str += " severity=LOW message=";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		str += " severity=MEDIUM message=";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		str += " severity=HIGH message=";
		break;
	default:
		str += " severity=UNKNOWN message=";
		break;
	}
	str += _message;
	str += "\n";
	OutputDebugStringA(str.data());
	}
	static void LoadOpenGL() {
	#ifdef _WIN32
		LOADOPENGLFUNCTION(wglCreateContextAttribsARB);
		LOADOPENGLFUNCTION(wglChoosePixelFormatARB);
		LOADOPENGLFUNCTION(wglSwapIntervalEXT);

		LOADOPENGLFUNCTION(glGenVertexArrays);
		LOADOPENGLFUNCTION(glBindVertexArray);
		LOADOPENGLFUNCTION(glGenBuffers);
		LOADOPENGLFUNCTION(glBindBuffer);
		LOADOPENGLFUNCTION(glBufferData);
		LOADOPENGLFUNCTION(glVertexAttribPointer);
		LOADOPENGLFUNCTION(glEnableVertexAttribArray);
		LOADOPENGLFUNCTION(glDeleteBuffers);
		LOADOPENGLFUNCTION(glDeleteVertexArrays);
		LOADOPENGLFUNCTION(glBufferStorage);
		LOADOPENGLFUNCTION(glBufferSubData);

		LOADOPENGLFUNCTION(glCreateShader);
		LOADOPENGLFUNCTION(glShaderSource);
		LOADOPENGLFUNCTION(glCompileShader);
		LOADOPENGLFUNCTION(glCreateProgram);
		LOADOPENGLFUNCTION(glAttachShader);
		LOADOPENGLFUNCTION(glLinkProgram);
		LOADOPENGLFUNCTION(glUseProgram);
		LOADOPENGLFUNCTION(glGetShaderiv);
		LOADOPENGLFUNCTION(glDisableVertexAttribArray);
		LOADOPENGLFUNCTION(glDetachShader);
		LOADOPENGLFUNCTION(glDeleteShader);

		LOADOPENGLFUNCTION(glGetUniformLocation);
		LOADOPENGLFUNCTION(glUniform2fv);
		LOADOPENGLFUNCTION(glUniform1f);
		LOADOPENGLFUNCTION(glUniform1i);
		LOADOPENGLFUNCTION(glUniform4f);
		LOADOPENGLFUNCTION(glUniform4fv);
		LOADOPENGLFUNCTION(glUniformMatrix4fv);

		LOADOPENGLFUNCTION(glGetProgramiv);
		LOADOPENGLFUNCTION(glGetShaderInfoLog);
		LOADOPENGLFUNCTION(glActiveTexture);
		LOADOPENGLFUNCTION(glGenSamplers);
		LOADOPENGLFUNCTION(glDeleteSamplers);
		LOADOPENGLFUNCTION(glSamplerParameteri);
		LOADOPENGLFUNCTION(glBindSampler);
		LOADOPENGLFUNCTION(glGenProgramPipelines);
		LOADOPENGLFUNCTION(glBindProgramPipeline);
		LOADOPENGLFUNCTION(glDeleteProgramPipelines);
		LOADOPENGLFUNCTION(glUseProgramStages);
		LOADOPENGLFUNCTION(glProgramParameteri);
		LOADOPENGLFUNCTION(glGetProgramInfoLog);
		LOADOPENGLFUNCTION(glActiveShaderProgram);

		LOADOPENGLFUNCTION(glDrawArraysInstanced);
		LOADOPENGLFUNCTION(glVertexAttribDivisor);

		LOADOPENGLFUNCTION(glTexStorage2D);

	#ifdef _DEBUG
		LOADOPENGLFUNCTION(glDebugMessageCallback);
	#endif
	#elif __ANDROID__

	#endif
	}
	static GLuint LoadAndCompileShader(const Tchar* _path,GLenum _type) {
		const GLuint shader = glCreateShader(_type);
		File shaderFile(_path, false, true);
		int shaderSize = (int)shaderFile.GetSize();
		char* shaderData = new char[(size_t)shaderSize];
		shaderFile.ReadBytes((unsigned)shaderSize, shaderData);
		shaderFile.Close();
		glShaderSource(shader, 1, (const char**)&shaderData, &shaderSize);
		glCompileShader(shader);

		delete[]shaderData;

		GLint compileStatus;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
		if (!compileStatus) {
			GLint compileLogLen;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compileLogLen);
			char* compileLogCharAr = new char[compileLogLen];
			glGetShaderInfoLog(shader, compileLogLen, nullptr, compileLogCharAr);
			string compileLogDebugStr = "!!XFit OpenGL Shader Compile Error : ";
			compileLogDebugStr += compileLogCharAr;
			compileLogDebugStr += "\n";
			OutputDebugStringA(compileLogDebugStr.data());
			delete[]compileLogCharAr;
		}
		const GLuint prog = glCreateProgram();

		glAttachShader(prog, shader);

		glProgramParameteri(prog, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glLinkProgram(prog);

		GLint linkStatus;
		glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
		if (!linkStatus) {
			GLint linkLogLen;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &linkLogLen);
			char* linkLogCharAr = new char[linkLogLen];
			glGetProgramInfoLog(prog, linkLogLen, nullptr, linkLogCharAr);
			string linkLogDebugStr = "!!XFit OpenGL Shader Link Error : ";
			linkLogDebugStr += linkLogCharAr;
			linkLogDebugStr += "\n";
			OutputDebugStringA(linkLogDebugStr.data());
			delete[]linkLogCharAr;
		}

		glDetachShader(prog, shader);
		glDeleteShader(shader);

		return prog;
	}
	void Create() {
	#ifdef _DEBUG
	#endif
	#ifdef _WIN32
		WNDCLASS wndClass;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hbrBackground = nullptr;
		wndClass.hCursor = nullptr;
		wndClass.hIcon = nullptr;
		wndClass.lpszMenuName = nullptr;
		wndClass.hInstance = _System::_Windows::hInstance;
		wndClass.lpfnWndProc = DefWindowProc;
		wndClass.lpszClassName = _T("OpenGLTemp");
		wndClass.style = CS_OWNDC;//to use wgl
		RegisterClass(&wndClass);
		tempHWnd = CreateWindow(_T("OpenGLTemp"), _T("OpenGLTemp"), 0, 0, 0, 100, 100, nullptr, nullptr, _System::_Windows::hInstance, nullptr);

		PIXELFORMATDESCRIPTOR pfd;
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);//always
		pfd.nVersion = 1;//always

		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;

		pfd.cRedBits = 0;//unused
		pfd.cRedShift = 0;//unused
		pfd.cGreenBits = 0;//unused
		pfd.cGreenShift = 0;//unused
		pfd.cBlueBits = 0;//unused
		pfd.cBlueShift = 0;//unused
		pfd.cAlphaBits = 0;
		pfd.cAlphaShift = 0;//unused
		pfd.cAccumBits = 0;
		pfd.cAccumRedBits = 0;//unused
		pfd.cAccumGreenBits = 0;//unused
		pfd.cAccumBlueBits = 0;//unused
		pfd.cAccumAlphaBits = 0;//unused
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.cAuxBuffers = 0;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.bReserved = 0;//unused
		pfd.dwLayerMask = 0;//unused
		pfd.dwVisibleMask = 0;//unused
		pfd.dwDamageMask = 0;//unused
		tempDC = GetDC(tempHWnd);

		int choosePixelFormat = ChoosePixelFormat(tempDC, &pfd);
		SetPixelFormat(tempDC, choosePixelFormat, &pfd);

		tempContext = wglCreateContext(tempDC);
		wglMakeCurrent(tempDC, tempContext);
	#elif __ANDROID__
	#endif
		version.name = System::RendererName::OpenGL;

		int major, minor;
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		version.minorVersion = (unsigned)minor;
		version.majorVersion = (unsigned)major;
	}
	void Init(System::RendererInfo* _info) {
	#ifdef _DEBUG
		if (System::IsRendererInited());
	#endif
		LoadOpenGL();

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(context);
		ReleaseDC(tempHWnd, tempDC);
		DestroyWindow(tempHWnd);

#ifdef _WIN32
		int pixelFormat;
		unsigned numFormats;
		if (_info->msaaCount > 1) {
			const int attribList[] = {
			WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
			WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
			WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
			WGL_PIXEL_TYPE_ARB,WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,24,
			WGL_ALPHA_BITS_ARB,8,
			WGL_DEPTH_BITS_ARB,24,
			WGL_STENCIL_BITS_ARB,8,
			WGL_SAMPLE_BUFFERS_ARB,1,
			WGL_SAMPLES_ARB,_info->msaaCount,0 };

			wglChoosePixelFormatARB(_System::_Windows::hdc, attribList, nullptr, 1, &pixelFormat, &numFormats);
		}
		else {
			const int attribList[] = {
			WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
			WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
			WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
			WGL_PIXEL_TYPE_ARB,WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,24,
			WGL_ALPHA_BITS_ARB,8,
			WGL_DEPTH_BITS_ARB,24,
			WGL_STENCIL_BITS_ARB,8,
			0 };

			wglChoosePixelFormatARB(_System::_Windows::hdc, attribList, nullptr, 1, &pixelFormat, &numFormats);
		}
		SetPixelFormat(_System::_Windows::hdc, pixelFormat, nullptr);

#ifdef _DEBUG
		const int attribList2[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, (int)version.majorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, (int)version.minorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,0
		};
#else
		const int attribList2[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, (int)version.MajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, (int)version.MinorVersion,
			0 };
#endif
		context = wglCreateContextAttribsARB(_System::_Windows::hdc, nullptr, attribList2);

		wglMakeCurrent(_System::_Windows::hdc, context);

		tempHWnd = nullptr;
#elif __ANDROID__
#endif

#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenglDebugCallback, nullptr);
#endif
		if (_info->msaaCount > 1)glEnable(GL_MULTISAMPLE);


		glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);

		imgVertProg = LoadAndCompileShader(_T("../Xfit/_shader/imgVert.glsl"), GL_VERTEX_SHADER);
		imgInsVertProg = LoadAndCompileShader(_T("../Xfit/_shader/imgInsVert.glsl"), GL_VERTEX_SHADER);

		imgFragProg = LoadAndCompileShader(_T("../Xfit/_shader/imgFrag.glsl"), GL_FRAGMENT_SHADER);

		glGenProgramPipelines(1, &progPipeline);
		glBindProgramPipeline(progPipeline);

		imgVert::matUniform = glGetUniformLocation(imgVertProg, "matUniform");
		imgInsVert::matUniform = glGetUniformLocation(imgInsVertProg, "matUniform");

		imgFrag::samplerUniform = glGetUniformLocation(imgFragProg, "samplerUniform");

		glActiveShaderProgram(progPipeline, imgFragProg);
		glUniform1i(imgFrag::samplerUniform, 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);

		if (_info->vSync) wglSwapIntervalEXT(1);
		else wglSwapIntervalEXT(0);

		vSync = _info->vSync;
		msaaCount = _info->msaaCount;
	}
	void Release() {
#ifdef _WIN32
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(context);
		if (tempHWnd) {
			ReleaseDC(tempHWnd, tempDC);
			DestroyWindow(tempHWnd);
		}
#elif __ANDROID__
#endif
	}
}
