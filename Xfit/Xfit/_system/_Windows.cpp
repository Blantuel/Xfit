#ifdef WIN_DESKTOP

#include "_Windows.h"
#include "_Loop.h"
#include "../data/Array.h"
#include "_Renderer.h"

#include "_DirectX11.h"
#include "_DXGI.h"


namespace _System::_Windows {
	void Release() {}

	bool activateInited = false;//WM_ACTIVATE가 처음 윈도우가 보여졌을 때 호출되는 것을 방지

	LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
		switch (_message) {
		case WM_ACTIVATE:
			if (activateInited) {
				_Windows::pause = HIWORD(_wParam);//윈도우가 최소화 확인
				_Windows::activated = (LOWORD(_wParam) != WA_INACTIVE);

				if (screenMode == System::ScreenMode::Fullscreen) {
					if (!_Windows::activated) {
						CloseWindow(_hWnd);
						_Windows::pause = true;
					}

				}

				System::activateFunc();
			}
			activateInited = true;
			return 0;
		case WM_SIZE:
			_Renderer::windowSize.width = LOWORD(_lParam);
			_Renderer::windowSize.height = HIWORD(_lParam);

			if (_System::_DXGI::swapChain)_System::_DirectX11::Resize();
			if (_wParam != SIZE_MINIMIZED && System::sizeFunc)System::sizeFunc();
			return 0;
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)_lParam)->ptMinTrackSize.x = 320;
			((MINMAXINFO*)_lParam)->ptMinTrackSize.y = 240;
			return 0;
		case WM_KEYDOWN:
			if (keyState[_wParam] == 0)keyState[_wParam] = 1;
			keyDownUpState[_wParam] = 1;
			return 0;
		case WM_KEYUP:
			keyState[_wParam] = 3;
			keyDownUpState[_wParam] = 2;
			return 0;
		case WM_IME_COMPOSITION:
		{
			HIMC hIMC = ImmGetContext(hWnd);

			unsigned len;
			if (_lParam & GCS_RESULTSTR) {
				if ((len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0)) > 0) {
					wchar_t* tempChars = new wchar_t[len];
					ImmGetCompositionString(hIMC, GCS_RESULTSTR, (LPVOID)tempChars, len);

					tempChars[len - 1] = 0;
					chars.resize(chars.size() - prevCharsLen);
					chars += tempChars;
					prevCharsLen = 0;
				}
				enterCharState = EnterCharState::Finish;
			} else if (_lParam & GCS_COMPSTR) {
				if ((len = ImmGetCompositionString(hIMC, GCS_COMPSTR, NULL, 0)) > 0) {
					wchar_t* tempChars = new wchar_t[len];
					ImmGetCompositionString(hIMC, GCS_COMPSTR, (LPVOID)tempChars, len);

					tempChars[len - 1] = 0;
					chars += tempChars;
					prevCharsLen = len - 1;
				}
				enterCharState = EnterCharState::Making;
			}

			ImmReleaseContext(hWnd, hIMC);

			//{
			//	wchar_t buf[20];
			//	swprintf_s(buf, L"%c : %d\n", (wchar_t)_wParam, _wParam);
			//	OutputDebugString(buf);
			//}
			return 0;
		}
		case WM_CHAR:
			if (!Input::IsKeyPressing(Input::Key::Control)) {
				chars += (wchar_t)_wParam;
				enterCharState = EnterCharState::Finish;
			}
			return 0;
		case WM_IME_CHAR:
			return 0;
		case WM_KILLFOCUS:
			memset(keyState, 0, 256);
			memset(keyDownUpState, 0, 256);
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
		case WM_MOUSEMOVE:
			TRACKMOUSEEVENT mouseEvent;
			mouseEvent.cbSize = sizeof(mouseEvent);
			mouseEvent.dwFlags = TME_LEAVE | TME_HOVER;
			mouseEvent.hwndTrack = _hWnd;
			mouseEvent.dwHoverTime = 10;
			TrackMouseEvent(&mouseEvent);
			return 0;
		case WM_MOUSELEAVE://MOUSEMOVE 메시지에서 TrackMouseEvent를 호출하면 호출되는 메시지
			mouseOut = true;
			return 0;
		case WM_MOUSEHOVER:
			mouseOut = false;
			return 0;
		case WM_MOUSEWHEEL:
			zScroll = (int)GET_WHEEL_DELTA_WPARAM(_wParam);
			return 0;
		case WM_DROPFILES:
			hDrop = (HDROP)_wParam;
			if (System::dragDropFuncs)System::dragDropFuncs();
			DragFinish(hDrop);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	void SetFullScreenMode(unsigned _displayIndex, unsigned _displayModeIndex) {
		//SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
		//SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
		/*
		WS_EX_APPWINDOW : 창이 표시되면 최상위 창을 작업 표시 줄에 강제로 표시합니다.
		윈도우가 작업 표시 줄에 들어가는 몇 가지 기본 규칙이 있습니다. 한마디로 :

		WS_EX_APPWINDOW 확장 스타일이 설정되면 (표시 될 때) 표시됩니다.
		윈도우가 최상위 레벨의 소유되지 않은 윈도우 인 경우 (표시 될 때)가 표시됩니다.
		그렇지 않으면 표시되지 않습니다.
		*/
		//HWND_TOPMOST : 윈도우를 최상위 윈도우로 표시합니다.
		/*SetWindowPos(hWnd, HWND_TOPMOST, _DXGI::displays[_displayIndex].displayPos.x, _DXGI::displays[_displayIndex].displayPos.y,
			_DXGI::displays[_displayIndex].modes[_displayModeIndex].width, _DXGI::displays[_displayIndex].modes[_displayModeIndex].height, 0);
		ShowWindow(hWnd, SW_SHOWNORMAL);

		DEVMODE mode = { 0 };
		mode.dmSize = sizeof(mode);
		mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
		mode.dmPelsWidth = _DXGI::displays[_displayIndex].modes[_displayModeIndex].width;
		mode.dmPelsHeight = _DXGI::displays[_displayIndex].modes[_displayModeIndex].height;
		mode.dmDisplayFrequency = _DXGI::displays[_displayIndex].modes[_displayModeIndex].refleshRate;

		DISPLAY_DEVICE displayDevice;
		displayDevice.cb = sizeof(displayDevice);
		EnumDisplayDevices(NULL, _displayIndex, &displayDevice, 0);
	
		ChangeDisplaySettingsEx(displayDevice.DeviceName, &mode, nullptr, CDS_FULLSCREEN | CDS_RESET, nullptr);*/

		DXGI_MODE_DESC mode;
		if (_DXGI::outputs[_displayIndex].output1) {
			mode.Width = _DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].Width;
			mode.Height = _DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].Height;
			mode.RefreshRate.Numerator = _DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].RefreshRate.Numerator;
			mode.RefreshRate.Denominator = _DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].RefreshRate.Denominator;
			mode.Format = _DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].Format;
			mode.Scaling = _DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].Scaling;
			mode.ScanlineOrdering = _DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].ScanlineOrdering;
		} else {
			mode.Width = _DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].Width;
			mode.Height = _DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].Height;
			mode.RefreshRate.Numerator = _DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].RefreshRate.Numerator;
			mode.RefreshRate.Denominator = _DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].RefreshRate.Denominator;
			mode.Format = _DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].Format;
			mode.Scaling = _DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].Scaling;
			mode.ScanlineOrdering = _DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].ScanlineOrdering;
		}
		_Renderer::windowSize.width = mode.Width;
		_Renderer::windowSize.height = mode.Height;
		_Renderer::refleshRateTop = mode.RefreshRate.Numerator;
		_Renderer::refleshRateBottom = mode.RefreshRate.Denominator;

		_DXGI::swapChain->ResizeTarget(&mode);

		if (_DXGI::outputs[_displayIndex].output1) {
			_DXGI::swapChain->SetFullscreenState(TRUE, _DXGI::outputs[_displayIndex].output1);
		} else {
			_DXGI::swapChain->SetFullscreenState(TRUE, _DXGI::outputs[_displayIndex].output);
		}

		screenMode = System::ScreenMode::Fullscreen;
	}
	void Create() {
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

		_DXGI::Create();
	}
	void Init(System::CreateInfo* _info) {
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
		wc.lpszClassName = _T("Xfit");
		RegisterClass(&wc);

		resizeWindow = _info->resizeWindow;

		screenMode = _info->screenMode;

		maximized = _info->maximized;
		minimized = _info->minimized;

		_DXGI::Init(_info);

		_DirectX11::Init(_info);

		ShowWindow(hWnd, (int)_info->windowShow);


		hdc = GetDC(hWnd);

		_Windows::activated = true;
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
			keyDownUpState[i] = 0;
		}
		zScroll = 0;
		prevCharsLen = 0;
		chars.clear();
		enterCharState = EnterCharState::None;
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				_System::_Loop::exited = true;
				return false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}
	void SetBorderlessScreenMode(unsigned _displayIndex) {
		_DXGI::swapChain->SetFullscreenState(FALSE, nullptr);

		const int left = _DXGI::outputs[_displayIndex].rect.left;
		const int right = _DXGI::outputs[_displayIndex].rect.right;
		const int top = _DXGI::outputs[_displayIndex].rect.top;
		const int bottom = _DXGI::outputs[_displayIndex].rect.bottom;

		SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);

		SetWindowPos(hWnd, 0, left, top, right - left, bottom - top, SWP_DRAWFRAME);

		ShowWindow(hWnd, SW_MAXIMIZE);

		screenMode = System::ScreenMode::BorderlessScreen;
	}
	void SetWindowMode(Point _pos, PointU _size, System::WindowState _state, bool _maximized, bool _minimized, bool _resizeWindow) {
		resizeWindow = _resizeWindow;
		maximized = _maximized;
		minimized = _minimized;

		/*DISPLAY_DEVICE displayDevice;
		displayDevice.cb = sizeof(displayDevice);
		EnumDisplayDevices(NULL, _DXGI::currentDisplay, &displayDevice, 0);

		ChangeDisplaySettingsEx(displayDevice.DeviceName, nullptr, nullptr, CDS_RESET, nullptr);*/

		_DXGI::swapChain->SetFullscreenState(FALSE, nullptr);

		DWORD style = WS_CAPTION | WS_SYSMENU | (resizeWindow ? WS_THICKFRAME : 0) | (maximized ? WS_MAXIMIZEBOX : 0)
			| (minimized ? WS_MINIMIZEBOX : 0);

		//SWP_DRAWFRAME : 윈도우 프레임(표시줄 등)을 그린다.

		RECT rect = {0, 0, _size.x, _size.y};
		AdjustWindowRect(&rect, style, FALSE);

		SetWindowLongPtr(hWnd, GWL_STYLE, style);
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);
		SetWindowPos(hWnd, 0, _pos.x, _pos.y, rect.right-rect.left, rect.bottom-rect.top, SWP_DRAWFRAME);

		if (_state == System::WindowState::Maximized)ShowWindow(hWnd, SW_MAXIMIZE);
		else if (_state == System::WindowState::Minimized)ShowWindow(hWnd, SW_MINIMIZE);
		else ShowWindow(hWnd, SW_RESTORE);
		
		screenMode = System::ScreenMode::Window;
	}
}
#endif
