#include "../object/Object.h"
#include "../object3D/Object3D.h"
#include "../sound/Sound.h"
//#include "../text/Font.h"
#include "../resource/Sampler.h"
#include "../effect/Blend.h"
#include "../effect/Light.h"
#include "../_system/_Loop.h"

#ifdef _WIN32
#include "../_system/_Windows.h"
#elif __ANDROID__
#include "../_system/_Android.h"
#endif

#include "../_system/_GraphicsBase.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif

SystemError::SystemError(Code _code) :Error((int)_code) {}

namespace System {
#ifdef _WIN32
	void WindowCreate(HINSTANCE _hInstance) {
		_System::_Windows::Create(_hInstance);
	}
#elif __ANDROID__
	void WindowCreate() {
	}
#endif

	void WindowInit(WindowInfo* _info) {
#ifdef _DEBUG
		if (!System::IsCreated());
#endif
#ifdef _WIN32
		_System::_Windows::Init(_info);
#elif __ANDROID__
#endif
	}
	void Loop() {
		_System::_Loop::Loop();
	}
	void WindowRelease() {
#ifdef _WIN32
		_System::_Windows::Release();
#elif __ANDROID__
#endif
	}
	void RendererCreate() {
		_System::_OpenGL::Create();
	}
	void RendererRelease() {
		_System::_OpenGL::Release();
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
#endif
	}

	float GetRefleshRate() { return _System::_OpenGL::refleshRate; }
	bool IsVSync() { return _System::_OpenGL::vSync; }

	unsigned GetMsaaCount() { return _System::_OpenGL::msaaCount; }
	unsigned GetMsaaQuality() { return _System::_OpenGL::msaaQuality; }

	RendererVersion GetCurrentRendererVersion() { return _System::_OpenGL::version; }
	bool IsRendererInited() {
		return _System::_OpenGL::tempHWnd == nullptr;
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
		ShowWindow(_System::_Windows::hWnd, (int)_windowShow);
	}
	unsigned GetProcessorCoresNum() {
#ifdef _DEBUG
		if (!System::IsCreated());
#endif
		return _System::_Windows::processorCoreNum;
	}

	unsigned short GetProcessorArchitecture() {
#ifdef _DEBUG
		if (!System::IsCreated());
#endif
		return _System::_Windows::processorArchitecture;
	}

	bool IsExiting() { return _System::_Loop::exited; }
	unsigned GetWindowWidth() { return _System::_Windows::windowWidth; }
	unsigned GetWindowHeight() { return _System::_Windows::windowHeight; }


	void GetTitle(Tstring& _title) {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
		_title.resize(GetWindowTextLength(_System::_Windows::hWnd));
		GetWindowText(_System::_Windows::hWnd, _title.data(), _title.size());
	}
	void SetTitle(const Tchar* _title) { SetWindowText(_System::_Windows::hWnd, _title); }

	bool IsResizeWindow() { return _System::_Windows::resizeWindow; }
	bool IsMaximizedWindow() { return _System::_Windows::minimized; }
	bool IsMinimizedWindow() { return _System::_Windows::maximized; }
	void ResizeWindow(unsigned _width, unsigned _height) {
#ifdef _DEBUG
		if (!System::IsWindowInited());
		if (!_System::_Windows::resizeWindow);
		if ((_width == 0) || (_height == 0));
#endif
		RECT r, rs = { 0,0,_width,_height };
		GetWindowRect(_System::_Windows::hWnd, &r);
		AdjustWindowRect(&rs, GetWindowLong(_System::_Windows::hWnd, GWL_STYLE), false);
		MoveWindow(_System::_Windows::hWnd, r.left, r.top, rs.right, rs.bottom, false);
	}
	ScreenMode GetScreenMode() { return _System::_Windows::screenMode; }
	void OpenColsole() {
		FILE* handle;
		freopen_s(&handle, "CONIN$", "r", stdin);
		freopen_s(&handle, "CONOUT$", "w", stdout);
		freopen_s(&handle, "CONERR$", "w", stderr);
		AllocConsole();
		_System::_Windows::consoleOpened = true;
	}
	bool IsConsoleOpened() { return _System::_Windows::consoleOpened; }
	void CloseConsole() {
		FreeConsole();
		_System::_Windows::consoleOpened = false;
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
	void Exit() { DestroyWindow(_System::_Windows::hWnd); }
	bool IsPause() { return _System::_Windows::pause; }
	OSversion GetOSVersion() {
#ifdef _DEBUG
		if (!System::IsCreated());
#endif
		return _System::_Windows::_osversion;
	}

	bool IsWindowInited() { return (bool)_System::_Windows::hWnd; }
	bool IsCreated() { return _System::_Windows::_osversion.os != OS::None; }
	chrono::nanoseconds GetDeltaTime() { return _System::_Loop::deltaTime; }
}

