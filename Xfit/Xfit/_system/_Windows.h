#pragma once

#ifdef _WIN32

#include "../system/System.h"
#include "../text/Text.h"


namespace _System::_Windows {
	inline System::OSversion _osversion = { System::OS::None, };//_osversion.os not nullptr == Created
	inline System::ScreenMode screenMode = System::ScreenMode::Window;
	inline HINSTANCE hInstance;
	inline HWND hWnd = nullptr;//is not nullptr == WindowInited

	inline bool consoleOpened=false;

	inline MSG msg;

	inline unsigned processorCoreNum=0;
	inline unsigned short processorArchitecture=0;

	inline unsigned windowWidth, windowHeight;

	inline HDC hdc;

	inline bool resizeWindow;
	inline bool maximized;
	inline bool minimized;
	inline DEVMODE fullscreenMode, restoreMode;

	void Init(System::WindowInfo* _info);

	inline bool pause=false, activated=false;

	inline unsigned char click=0, click2=0, click3=0;
	inline Tchar tchar=0;
	inline unsigned char keyState[256] = {};
	inline short zScroll=0;

	bool Loop();
	void Release();
}
#endif