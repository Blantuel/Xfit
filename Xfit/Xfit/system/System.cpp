#include "../object/Object.h"
#include "../resource/Sampler.h"
#include "../effect/Blend.h"
#include "../effect/Light.h"
#include "../_system/_Loop.h"

#include "../_system/_Windows.h"
#include "../_system/_Android.h"

#include "../_system/_GraphicsBase.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

SystemError::SystemError(Code _code) :Error((int)_code) {}


namespace System {
#ifdef _WIN32
	void Create(HINSTANCE _hInstance) {
		_System::_Windows::hInstance = _hInstance;

		System::createFunc();

		while (true) {
			if (!_System::_Windows::Loop())break;
			_System::_Loop::Loop();
		}
	}
#elif __ANDROID__
	void Create(ANativeActivity* _activity) {
		_System::_Android::Create(_activity);
	}
#endif

	void WindowInit(WindowInfo* _info) {
#ifdef _WIN32
		_System::_Windows::Init(_info);
#elif __ANDROID__
		_System::_Android::WindowInit(_info);
#endif
	}
	void RendererInit(RendererInfo* _info) {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
		_System::_OpenGL::Init(_info);
	}
	void Render() {
#ifdef _DEBUG
		if (!System::IsRendererInited());
#endif
#ifdef _WIN32
		SwapBuffers(_System::_Windows::hdc);
#elif __ANDROID__
		eglSwapBuffers(_System::_Android::engine.display, _System::_Android::engine.surface);
#endif
	}

	float GetRefleshRate() { return _System::_OpenGL::refleshRate; }
	bool IsVSync() { return _System::_OpenGL::vSync; }

	unsigned GetMsaaCount() { return _System::_OpenGL::msaaCount; }
	unsigned GetMsaaQuality() { return _System::_OpenGL::msaaQuality; }

	RendererVersion GetCurrentRendererVersion() { return _System::_OpenGL::version; }
	bool IsRendererInited() {
		return _System::_OpenGL::version.name == System::RendererName::OpenGL;
	}
	void Clear(bool _clearDepth/* = false*/) {
		glClear(_clearDepth ? (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) : GL_COLOR_BUFFER_BIT);
	}
	void SetScreenMode(ScreenMode _screenMode) {
	}
	void SetWindowShow(WindowShow _windowShow) {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
#ifdef _WIN32
		ShowWindow(_System::_Windows::hWnd, (int)_windowShow);
#elif __ANDROID__

#endif
	}
	unsigned GetProcessorCoresNum() {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
#ifdef _WIN32
		return _System::_Windows::processorCoreNum;
#elif __ANDROID__
		return 0;
#endif
	}

	unsigned short GetProcessorArchitecture() {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
#ifdef _WIN32
		return _System::_Windows::processorArchitecture;
#elif __ANDROID__
		return 0;
#endif
	}

	bool IsExiting() { return _System::_Loop::exited; }
	unsigned GetWindowWidth() {
#ifdef _WIN32
		return _System::_Windows::windowWidth;
#elif __ANDROID__
		return _System::_Android::engine.width;
#endif
	}
	unsigned GetWindowHeight() { 
#ifdef _WIN32
		return _System::_Windows::windowHeight; 
#elif __ANDROID__
		return _System::_Android::engine.height;
#endif
	}


	void GetTitle(Tstring& _title) {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
#ifdef _WIN32
		_title.resize(GetWindowTextLength(_System::_Windows::hWnd));
		GetWindowText(_System::_Windows::hWnd, _title.data(), _title.size());
#elif __ANDROID__
#endif
	}
	void SetTitle(const Tchar* _title) {
#ifdef _WIN32
		SetWindowText(_System::_Windows::hWnd, _title);
#elif __ANDROID__
#endif
	}

	bool IsResizeWindow() {
#ifdef _WIN32
		return _System::_Windows::resizeWindow; 
#elif __ANDROID__
		return false;
#endif
	}
	bool IsMaximizedWindow() {
#ifdef _WIN32
		return _System::_Windows::minimized;
#elif __ANDROID__
		return false;
#endif
	}
	bool IsMinimizedWindow() {
#ifdef _WIN32
		return _System::_Windows::maximized;
#elif __ANDROID__
		return false;
#endif
	}
	void ResizeWindow(unsigned _width, unsigned _height) {
#ifdef _WIN32
#ifdef _DEBUG
		if (!System::IsWindowInited());
		if (!_System::_Windows::resizeWindow);
		if ((_width == 0) || (_height == 0));
#endif
		RECT r, rs = { 0,0,_width,_height };
		GetWindowRect(_System::_Windows::hWnd, &r);
		AdjustWindowRect(&rs, GetWindowLong(_System::_Windows::hWnd, GWL_STYLE), false);
		MoveWindow(_System::_Windows::hWnd, r.left, r.top, rs.right, rs.bottom, false);
#elif __ANDROID__
#endif
	}
	ScreenMode GetScreenMode() { 
#ifdef _WIN32
		return _System::_Windows::screenMode;
#elif __ANDROID__
		return _System::_Android::screenMode;
#endif
	}
	void OpenColsole() {
#ifdef _WIN32
		FILE* handle;
		freopen_s(&handle, "CONIN$", "r", stdin);
		freopen_s(&handle, "CONOUT$", "w", stdout);
		freopen_s(&handle, "CONERR$", "w", stderr);
		AllocConsole();
		_System::_Windows::consoleOpened = true;
#elif __ANDROID__
#endif
	}
	bool IsConsoleOpened() { 
#ifdef _WIN32
		return _System::_Windows::consoleOpened;
#elif __ANDROID__
#endif
	}
	void CloseConsole() {
#ifdef _WIN32
		FreeConsole();
		_System::_Windows::consoleOpened = false;
#elif __ANDROID__
#endif
	}
	void DebugMemoryLeak() {
#ifdef _WIN32
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#else
#endif
	}
	void DebugMemoryLeakBreakPoint(int _value) {
#ifdef _WIN32
#ifdef _DEBUG
		_CrtSetBreakAlloc(_value);
#endif
#else
#endif
	}
	void Exit() { 
#ifdef _WIN32
		DestroyWindow(_System::_Windows::hWnd); 
#elif __ANDROID__
		_System::_Android::app.destroyRequested = true;
#endif
	}
	bool IsPause() { 
#ifdef _WIN32
		return _System::_Windows::pause; 
#elif __ANDROID__
#endif
	}
	OSversion GetOSVersion() {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
#ifdef _WIN32
		return _System::_Windows::_osversion;
#elif __ANDROID__
		return _System::_Android::_osVersion;
#endif
	}

	bool IsWindowInited() { 
#ifdef _WIN32
		return (bool)_System::_Windows::hWnd;
#elif __ANDROID__
		return (bool)_System::_Android::engine.context;
#endif
	}
	double GetDeltaTime() { return 0.0; }
}

