#ifdef _WIN32
#include "_Windows.h"
#include "_Loop.h"
#include "../data/Array.h"

#include "_OpenGL.h"
#include "_Vulkan.h"

namespace _System::_Windows {
	void Release() {}

	bool sizeInited = false;//WM_SIZE가 처음 윈도우가 보여졌을 때 호출되는 것을 방지
	bool activateInited = false;//WM_ACTIVATE가 처음 윈도우가 보여졌을 때 호출되는 것을 방지

	LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
		switch (_message) {
		case WM_ACTIVATE:
			if (activateInited) {
				_Windows::pause = HIWORD(_wParam);
				_Windows::activated = (LOWORD(_wParam) != WA_INACTIVE);
				if (System::GetScreenMode() == System::ScreenMode::Fullscreen) {
					_Windows::pause = !_Windows::activated;
					if (_Windows::activated) {
						if (ChangeDisplaySettings(&fullscreenMode, CDS_FULLSCREEN | CDS_RESET) != DISP_CHANGE_SUCCESSFUL)
						{
						}
					} else {
						if (ChangeDisplaySettings(&restoreMode, CDS_RESET) != DISP_CHANGE_SUCCESSFUL)
						{
						}
						CloseWindow(hWnd);
					}
				}
				System::activateFunc(_Windows::activated, _Windows::pause);
			}
			activateInited = true;
			return 0;
		case WM_SIZE:
			if ((System::GetScreenMode() != System::ScreenMode::Fullscreen) && (_wParam != SIZE_MINIMIZED) && sizeInited) {
				_Windows::windowWidth = LOWORD(_lParam);
				_Windows::windowHeight = HIWORD(_lParam);
				_System::_OpenGL::Resize();
			}
			sizeInited = true;
			return 0;
		case WM_KEYDOWN:
			if (keyState[_wParam] == 0)keyState[_wParam] = 1;
			return 0;
		case WM_KEYUP:
			keyState[_wParam] = 3;
			return 0;
		case WM_LBUTTONDOWN:
			click = 1;
			return 0;
		case WM_LBUTTONUP:
			click = 3;
			return 0;
		case WM_RBUTTONDOWN:
			click2 = 1;
			return 0;
		case WM_RBUTTONUP:
			click2 = 3;
			return 0;
		case WM_MBUTTONDOWN:
			click3 = 1;
			return 0;
		case WM_MBUTTONUP:
			click3 = 3;
			return 0;
		case WM_MOUSEWHEEL:
			zScroll = GET_WHEEL_DELTA_WPARAM(_wParam);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	void Init(System::WindowInfo* _info) {
		RTL_OSVERSIONINFOEXW osversioninfo;
		osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);
		const auto RtlGetVersion = (NTSTATUS(NTAPI*)(PRTL_OSVERSIONINFOEXW))GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "RtlGetVersion");

		RtlGetVersion(&osversioninfo);
		_osversion.os = System::OS::Windows;
		_osversion.windows.buildnumber = osversioninfo.dwBuildNumber;
		_osversion.windows.servicepack = osversioninfo.wServicePackMajor;
		const bool serverOS = osversioninfo.wProductType != VER_NT_WORKSTATION;
		if (osversioninfo.dwMajorVersion == 6 && osversioninfo.dwMinorVersion == 1) _osversion.windows.version = serverOS ? System::WindowsVersion::WindowsServer2008R2 : System::WindowsVersion::Windows7;
		else if (osversioninfo.dwMajorVersion == 6 && osversioninfo.dwMinorVersion == 2) _osversion.windows.version = serverOS ? System::WindowsVersion::WindowsServer2012 : System::WindowsVersion::Windows8;
		else if (osversioninfo.dwMajorVersion == 6 && osversioninfo.dwMinorVersion == 3) _osversion.windows.version = serverOS ? System::WindowsVersion::WindowsServer2012R2 : System::WindowsVersion::Windows8Point1;
		else if (osversioninfo.dwMajorVersion == 10 && osversioninfo.dwMinorVersion == 0) _osversion.windows.version = serverOS ? System::WindowsVersion::WindowsServer2016 : System::WindowsVersion::Windows10;

		SYSTEM_INFO infoi;
		GetSystemInfo(&infoi);

		processorCoreNum = infoi.dwNumberOfProcessors;
		processorArchitecture = infoi.wProcessorArchitecture;

		maximized = _info->maximized;
		minimized = _info->minimized;

		WNDCLASS wc;
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = _Windows::hInstance;
		wc.hIcon = nullptr;
		if (_info->cursorResource)wc.hCursor = LoadCursor(hInstance, _info->cursorResource);
		else wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = _T("WGXR");
		RegisterClass(&wc);
	
		windowWidth = _info->windowWidth;
		windowHeight = _info->windowHeight;
		resizeWindow = _info->resizeWindow;

		screenMode = _info->screenMode;

		DWORD style;
		DWORD exStyle=0;
		if (screenMode == System::ScreenMode::Fullscreen) {
			style = WS_POPUP;
			exStyle= WS_EX_APPWINDOW;
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &restoreMode);

			memset(&fullscreenMode, 0, sizeof(fullscreenMode));// DEVMODE 구조체를 초기화
			fullscreenMode.dmSize = sizeof(fullscreenMode);// 구조체의 크기 
			fullscreenMode.dmPelsWidth = windowWidth;
			fullscreenMode.dmPelsHeight = windowHeight;
			fullscreenMode.dmBitsPerPel = 32;// set the bits per pixel
			fullscreenMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		}else style = WS_CAPTION | WS_SYSMENU | (resizeWindow?WS_THICKFRAME:0) | (maximized ? WS_MAXIMIZEBOX : 0) | (minimized ? WS_MINIMIZEBOX : 0);


		hWnd = CreateWindowEx(exStyle,wc.lpszClassName, _info->title.data(), style, _info->windowPos.x, _info->windowPos.y, _info->windowWidth, _info->windowHeight, nullptr, nullptr, hInstance, nullptr);
		if (hWnd == nullptr);

		ShowWindow(hWnd, (int)_info->windowShow);

		if (screenMode == System::ScreenMode::Fullscreen)ChangeDisplaySettings(&fullscreenMode, CDS_FULLSCREEN | CDS_RESET);
		_Windows::pause = false;
		_Windows::activated = true;

		hdc = GetDC(hWnd);
	}
	bool Loop() {
		if (click == 1)click = 2;
		else if (click == 3)click = 0;
		if (click2 == 1)click2 = 2;
		else if (click2 == 3)click2 = 0;
		if (click3 == 1)click3 = 2;
		else if (click3 == 3)click3 = 0;
		for (unsigned i = 0; i < 256; i++) {
			if (keyState[i] == 1)keyState[i] = 2;
			else if (keyState[i] == 3)keyState[i] = 0;
		}
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				_System::_Loop::exited = true;
				return false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}
}
#endif
