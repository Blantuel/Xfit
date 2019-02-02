#pragma once

#include "../math/Point.h"
#include "../system/Error.h"

template <typename T> class Array;


class SystemError : public Error {
public:
	enum class Code {

	};
	SystemError(Code _code);
};

namespace System {
	enum class OS {
		None,
		Windows,
		Android
	};
#ifdef _WIN32
	enum class WindowShow {
		Default=SW_SHOWDEFAULT,
		Show=SW_SHOW,
		Hide=SW_HIDE,
		Maximize=SW_MAXIMIZE,
		Minimize=SW_MINIMIZE,
		Restore=SW_RESTORE
	};
#elif __ANDROID__
	enum class WindowShow {
		Default,
		Show
	};
#endif
	enum class WindowsVersion {
		Windows7,
		WindowsServer2008R2,
		Windows8,
		WindowsServer2012,
		Windows8Point1,
		WindowsServer2012R2,
		Windows10,
		WindowsServer2016
	};
	enum class AndroidAPILevel {
		KitKat=19,
		KitKatWatch,
		Lollipop,
		Lollipop1,
		Marshmallow,
		Nougat,
		Nougat1,
		Oreo,
		Oreo1,
		P
	};
	struct OSversion {
		OS os;
		union {
			struct {
				WindowsVersion version;
				unsigned buildnumber;
				unsigned servicepack;
			}windows;
			struct {
				AndroidAPILevel apiLevel;
			}android;
		};
	};
	enum class ScreenMode {
		Window,
		Fullscreen
	};
#ifdef _WIN32
	constexpr int WindowDefaultPos = CW_USEDEFAULT;
#endif

	struct WindowInfo {
		ScreenMode screenMode;

		unsigned windowWidth, windowHeight;
		WindowShow windowShow;
		
#ifdef _WIN32
		Point windowPos;
		Tstring title;
		Tchar* cursorResource;

		bool maximized;
		bool minimized;
		bool resizeWindow;
#endif
	};
	struct RendererInfo {
		bool vSync;
		unsigned char msaaCount;
	};
	enum class RendererName {
		None,
		OpenGL,
		Vulkan
	};
	struct RendererVersion {
		RendererName name;
		unsigned majorVersion;
		unsigned minorVersion;
	};
	
	inline void(*createFunc)();
	inline void(*activateFunc)(bool _activated, bool _minimized);
	inline void(*updateFuncs)();
	inline void(*destroyFunc)();

	RendererVersion GetCurrentRendererVersion();

	
	OSversion GetOSVersion();

	void Exit();

#ifdef _WIN32
	void Create(HINSTANCE _hInstance);
#elif __ANDROID__
	void Create(ANativeActivity* _activity);
#endif

	void RendererInit(RendererInfo* _info);
	void WindowInit(WindowInfo* _info);

	float GetRefleshRate();
	bool IsVSync();

	constexpr double NotInitedDeltaTime = -1.0;
	double GetDeltaTime();

	void SetScreenMode(ScreenMode _screenMode);
	
	unsigned GetProcessorCoresNum();

	unsigned short GetProcessorArchitecture();

	bool IsExiting();
	unsigned GetWindowWidth();
	unsigned GetWindowHeight();

	void GetTitle(Tstring& _title);

#ifdef _WIN32
	bool IsResizeWindow();
	bool IsMaximizedWindow();
	bool IsMinimizedWindow();
	void SetTitle(const Tchar* _title);
	void SetWindowShow(WindowShow _windowShow);
#endif
	
	void ResizeWindow(unsigned _width, unsigned _height);
	ScreenMode GetScreenMode();

	bool IsPause();

	unsigned GetMsaaCount();
	unsigned GetMsaaQuality();

	bool IsRendererInited();
	bool IsWindowInited();

	void DebugMemoryLeak();
	void DebugMemoryLeakBreakPoint(int _value);

	bool IsConsoleOpened();
	void OpenColsole();
	void CloseConsole();

	void Clear(bool _clearDepth = false);
	void Render();
};
