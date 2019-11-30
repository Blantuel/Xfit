#pragma once

#include "../math/Point.h"
#include "../system/Error.h"

template <typename T> class Array;

class Vertex;
class ShapeVertex;
class Index;
class Sampler;
class Blend;

class SystemError : public Error {
public:
	enum class Code {

	};
protected:
	Code code;
public:
	Code GetCode()const { return code; }
	SystemError(Code _code) :code(_code) {}
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

	struct CreateInfo {
		bool vSync;

#ifdef _WIN32
		bool maximized;
		bool minimized;
		bool resizeWindow;
#endif

		ScreenMode screenMode;
		WindowShow windowShow;

		float maxFrame;
		unsigned msaaCount, msaaQuality;
		unsigned screenIndex;
		unsigned refleshRate;

#ifdef _WIN32
		const Tchar* title;
		const Tchar* cursorResource;
		Point windowPos;
#endif
		PointU windowSize;

	};
	enum class RendererName {
		None,
		OpenGL,
		Vulkan,
		DirectX
	};
	enum class WindowState {
		Restore,
		Maximized,
		Minimized
	};
	struct RendererVersion {
		RendererName name;
		unsigned majorVersion;
		unsigned minorVersion;
	};
	
	inline void(*createFunc)();
	inline void(*activateFunc)(void* _data);
	inline void(*sizeFunc)(void* _data);
	inline void(*updateFuncs)(void* _data);
	inline void(*destroyFunc)();
	inline void* activateData = nullptr;
	inline void* sizeData = nullptr;
	inline void* updateData = nullptr;

	inline Vertex* defaultUV;
	inline Sampler* defaultSampler;
	inline Sampler* pointSampler;
	inline Blend* defaultBlend;
	inline Vertex* defaultVertex2D;
	inline ShapeVertex* rectShapeVertex2D;

	inline Vertex* reverseUV;

	inline Index* defaultIndex;

	RendererVersion GetCurrentRendererVersion();

	
	OSversion GetOSVersion();

	void Exit();

#ifdef _WIN32
	void Create(HINSTANCE _hInstance);
#elif __ANDROID__
	void Create(ANativeActivity* _activity);
#endif

	void Init(CreateInfo* _info);

	bool IsVSync();

	float GetDeltaTime();


	unsigned GetProcessorCoresNum();

	unsigned short GetProcessorArchitecture();

	bool IsExiting();
	unsigned GetWindowWidth();
	unsigned GetWindowHeight();
	PointU GetWindowSize();
	Point GetWindowPos();

	bool IsResizeWindow();
	bool IsMaximizedWindow();
	bool IsMinimizedWindow();
	void SetTitle(const Tchar* _title);
	void SetWindowShow(WindowShow _windowShow);

	void MsgBox(const Tchar* _msg, const Tchar* _title);
	
	void ResizeWindow(unsigned _width, unsigned _height);
	ScreenMode GetScreenMode();

	bool IsPause();

	TCHAR* GetClipboardData();
	void ClipboardClose();

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
	void SetClearColor(float _r,float _g,float _b,float _a);

	void Render();

	int GetDisplayNum();
	int GetCurrentDisplayIndex();
	int GetCurrentDisplayModeIndex();//게임에 적용된 현재 해상도 가져옴.
	int GetCurrentDisplayModeIndex(unsigned _displayIndex);//해당 모니터 기본 해상도 가져옴.
	int GetDisplayFromWindow();

	PointU GetDisplayModeSize(unsigned _displayIndex, unsigned _displayModeIndex);

	void SetFullScreenMode(unsigned _displayIndex, unsigned _displayModeIndex);
	void SetWindowMode(PointU _size, Point _pos, WindowState _state, bool _maximized, bool _minimized, bool _resizeWindow);
	WindowState GetWindowState();

	void MoveWindow(Point _pos);

	void SetMaxFrame(double _maxFrame);
	float GetMaxFrame();

	bool IsActivated();

#ifdef _WIN32
	void Wait(unsigned _milisecond);
#elif __ANDROID__
	void Wait(unsigned _time);
#endif
};
