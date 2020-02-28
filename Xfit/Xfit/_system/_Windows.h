#pragma once


#ifdef WIN_DESKTOP

#include "../system/System.h"
#include "../text/Text.h"
#include "../system/Input.h"

namespace _System::_Windows {
	inline System::OSversion _osversion = { System::OS::None, };//_osversion.os not nullptr == Created
	inline System::ScreenMode screenMode = System::ScreenMode::Window;
	inline HINSTANCE hInstance;
	inline HWND hWnd = nullptr;

	inline unsigned prevWindowWidth, prevWindowHeight;

	inline bool consoleOpened=false;

	inline MSG msg;

	inline unsigned processorCoreNum=0;
	inline unsigned short processorArchitecture=0;

	inline HANDLE clipBoardMem = 0;

	inline HDROP hDrop;

	inline HDC hdc;

	inline bool resizeWindow;
	inline bool maximized;
	inline bool minimized;

	void Create();
	void Init(System::CreateInfo* _info);

	inline bool pause=false, activated=false;

	inline unsigned char click=0, click2=0, click3=0;
	inline std::wstring chars;
	inline EnterCharState enterCharState;
	inline unsigned char keyState[256] = {};
	inline unsigned char keyDownUpState[256] = {};
	inline int zScroll=0;
	inline bool mouseOut;
	inline unsigned prevCharsLen = 0;

	bool Loop();
	void Release();

	void SetFullScreenMode(unsigned _displayIndex, unsigned _displayModeIndex);
	void SetBorderlessScreenMode(unsigned _displayIndex);
	void SetWindowMode(Point _pos, PointU _size, System::WindowState _state, bool _maximized, bool _minimized, bool _resizeWindow);
}
#endif