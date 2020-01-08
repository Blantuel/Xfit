#include "_DXGI.h"
#include "_Renderer.h"
#include "../data/Memory.h"

#include "_DirectX11.h"
#include "_Windows.h"

namespace _System::_DXGI {
	void Create() {
		HRESULT hr;

		const auto library = LoadLibrary(_T("dxgi.dll"));

		if (library == NULL) {//실패하면 NULL반환

		}
		
		const auto _CreateDXGIFactory2 = (HRESULT(*WINAPI)(UINT, REFIID, _COM_Outptr_ void**))GetProcAddress(library, "CreateDXGIFactory2");

		if (_CreateDXGIFactory2) {//실패하면 NULL반환
#ifdef _DEBUG
			constexpr UINT flag = DXGI_CREATE_FACTORY_DEBUG;
#else
			constexpr UINT flag = 0;
#endif
			_CreateDXGIFactory2(flag, __uuidof(IDXGIFactory2), (void**)& factory2);
			if (factory2 == nullptr);
		} else {
			CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)& factory1);
			if (factory1 == nullptr);
		}
		FreeLibrary(library);

		_DirectX11::Create();

		unsigned maxDisplaysLen = 10;
		displays = new Display[10];
		DISPLAY_DEVICE displayDevice;
		displayDevice.cb = sizeof(displayDevice);

		unsigned displayIndex = 0;

		struct MonitorData {
			HMONITOR hMonitor;
			Point displayPos;
			Tchar* devName;
		};

		DEVMODE devMode;
		while(EnumDisplayDevices(NULL, displayIndex, &displayDevice, 0)) {
			if (DISPLAY_DEVICE_ACTIVE & displayDevice.StateFlags) {
				MonitorData monitorData = { nullptr, Point(0,0), displayDevice.DeviceName };
				EnumDisplayMonitors(
					nullptr, nullptr,
					[](HMONITOR hMonitor, HDC hDC, LPRECT rc, LPARAM data) -> BOOL {
						auto monitorData = (MonitorData*)data;
						MONITORINFOEX mi;
						mi.cbSize = sizeof(mi);
						GetMonitorInfo(hMonitor, &mi);
#ifdef UNICODE
						if (wcscmp(mi.szDevice, monitorData->devName) == 0)
#else
						if (strcmp(mi.szDevice, monitorData->devName) == 0)
#endif
						{
							monitorData->hMonitor = hMonitor;
							monitorData->displayPos.x = mi.rcMonitor.left;
							monitorData->displayPos.y = mi.rcMonitor.top;
							return FALSE;
						}
						return TRUE;
					},
					(LPARAM)(&monitorData));
				
				if (monitorData.hMonitor) {
					displays[displaysLen].displayPos = monitorData.displayPos;
					displays[displaysLen].hMonitor = monitorData.hMonitor;
				}

				unsigned maxDisplayModesLen = 100;
				displays[displaysLen].modes = new DisplayMode[100];
				displays[displaysLen].modesLen = 0;


				while (EnumDisplaySettings(displayDevice.DeviceName, displays[displaysLen].modesLen, &devMode)) {
					displays[displaysLen].modes[displays[displaysLen].modesLen].width = devMode.dmPelsWidth;
					displays[displaysLen].modes[displays[displaysLen].modesLen].height = devMode.dmPelsHeight;
					displays[displaysLen].modes[displays[displaysLen].modesLen].refleshRate = devMode.dmDisplayFrequency;

					displays[displaysLen].modesLen++;
					if (displays[displaysLen].modesLen >= maxDisplayModesLen) {
						auto temp = displays[displaysLen].modes;
						maxDisplayModesLen += 100;
						displays[displaysLen].modes = new DisplayMode[maxDisplayModesLen];
						Memory::Copy(displays[displaysLen].modes, maxDisplayModesLen, temp, maxDisplayModesLen - 100);
						delete[]temp;
					}
				}
				EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode);
				for (unsigned i = 0; i < displays[displaysLen].modesLen; i++) {
					if (displays[displaysLen].modes[i].width == devMode.dmPelsWidth &&
						displays[displaysLen].modes[i].height == devMode.dmPelsHeight &&
						displays[displaysLen].modes[i].refleshRate == devMode.dmDisplayFrequency) {
						displays[displaysLen].currentMode = i;
						break;
					}
				}

				displaysLen++;
				if (displaysLen >= maxDisplaysLen) {
					auto temp = displays;
					maxDisplaysLen += 10;
					displays = new Display[maxDisplaysLen];
					Memory::Copy(displays, maxDisplaysLen, temp, maxDisplaysLen - 10);
					delete[]temp;
				}
			}
			displayIndex++;
		}

	}
	void Init(System::CreateInfo* _info) {
		HRESULT hr;

		int i;
		if (_info->screenMode == System::ScreenMode::Fullscreen) {
			currentDisplay = _info->screenIndex;

			if (_info->refleshRate == 0) {
				_info->refleshRate = displays[currentDisplay].modes[displays[currentDisplay].currentMode].refleshRate;
			}
			unsigned i = 0;
			for (; i < displays[currentDisplay].modesLen; i++) {
				if (_info->windowSize.width == displays[currentDisplay].modes[i].width &&
					_info->windowSize.height == displays[currentDisplay].modes[i].height &&
					_info->refleshRate == displays[currentDisplay].modes[i].refleshRate) {
					currentDisplayMode = i;
					break;
				}
			}
			if (i == displays[currentDisplay].modesLen) {
				_info->windowSize.width = displays[currentDisplay].modes[displays[currentDisplay].currentMode].width;
				_info->windowSize.height = displays[currentDisplay].modes[displays[currentDisplay].currentMode].height;
				currentDisplayMode = displays[currentDisplay].currentMode;
			}

			_System::_Windows::hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_TOPMOST, _T("Xfit"), _info->title, WS_POPUP, displays[currentDisplay].displayPos.x, displays[currentDisplay].displayPos.y,
				_info->windowSize.width, _info->windowSize.height, nullptr, nullptr, _System::_Windows::hInstance, nullptr);
			if (_System::_Windows::hWnd == nullptr);
		} else {
			DWORD style = WS_CAPTION | WS_SYSMENU | (_System::_Windows::resizeWindow ? WS_THICKFRAME : 0) | (_System::_Windows::maximized ? WS_MAXIMIZEBOX : 0)
				| (_System::_Windows::minimized ? WS_MINIMIZEBOX : 0);

			RECT rect = { 0,0, _info->windowSize.width, _info->windowSize.height };
			AdjustWindowRectEx(&rect, style, FALSE, 0);
			_System::_Windows::hWnd = CreateWindowEx(0, _T("Xfit"), _info->title, style, _info->windowPos.x, _info->windowPos.y,
				rect.right-rect.left, rect.bottom-rect.top, nullptr, nullptr, _System::_Windows::hInstance, nullptr);
		}
		_Renderer::windowSize = _info->windowSize;
		_Renderer::vSync = _info->vSync;

		if (factory2) {
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.SampleDesc.Count = _info->msaaCount;
			if (_info->msaaCount > 1)swapChainDesc.SampleDesc.Quality = _info->msaaQuality;
			else {
				swapChainDesc.SampleDesc.Quality = 0;
				_info->msaaQuality = 0;
			}

			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Stereo = false;
			swapChainDesc.Flags = 0;
			swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;

			swapChainDesc.Width = _Renderer::windowSize.width;
			swapChainDesc.Height = _Renderer::windowSize.height;

			hr = factory2->CreateSwapChainForHwnd(_DirectX11::device1, _System::_Windows::hWnd, &swapChainDesc, nullptr ,nullptr, &swapChain1);

			swapChain1->QueryInterface(&swapChain);
			swapChain1->QueryInterface(&swapChain2);
			if (swapChain2)swapChain1->QueryInterface(&swapChain3);
			factory2->QueryInterface(&factory1);
		} else {
			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.SampleDesc.Count = _info->msaaCount;
			if (_info->msaaCount > 1)swapChainDesc.SampleDesc.Quality = _info->msaaQuality;
			else {
				swapChainDesc.SampleDesc.Quality = 0;
				_info->msaaQuality = 0;
			}

			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
			swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.OutputWindow = _System::_Windows::hWnd;
			swapChainDesc.Windowed = TRUE;

			swapChainDesc.BufferDesc.Width = _Renderer::windowSize.width;
			swapChainDesc.BufferDesc.Height = _Renderer::windowSize.height;

			hr = factory1->CreateSwapChain(_DirectX11::device, &swapChainDesc, &swapChain);
		}
		hr = factory1->MakeWindowAssociation(_System::_Windows::hWnd, 0);
		
		_Renderer::msaaCount = _info->msaaCount;
		_Renderer::msaaQuality = _info->msaaQuality;
	}
	void Render() {
		if (swapChain1)swapChain1->Present(_Renderer::vSync ? 1 : 0, 0);
		else swapChain->Present(_Renderer::vSync ? 1 : 0, 0);
	}
	void SetFullScreenMode(unsigned _displayIndex, unsigned _displayModeIndex) {
		_Renderer::windowSize.width = displays[_displayIndex].modes[_displayModeIndex].width;
		_Renderer::windowSize.height = displays[_displayIndex].modes[_displayModeIndex].height;

		currentDisplay = _displayIndex;
		currentDisplayMode = _displayModeIndex;
	}
	int GetDisplayFromWindow() {
		const auto hMonitor = MonitorFromWindow(_Windows::hWnd, MONITOR_DEFAULTTONEAREST);
		for (unsigned i = 0; i < displaysLen; i++) {
			if (displays[i].hMonitor == hMonitor)return i;
		}
		return -1;
	}
	void SetWindowMode() {
		currentDisplay = -1;
		currentDisplayMode = -1;
	}
}