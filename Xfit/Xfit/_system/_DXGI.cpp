#include "_DXGI.h"
#include "_Renderer.h"
#include "../data/Memory.h"

#include "_DirectX11.h"
#include "_Windows.h"

namespace _System::_DXGI {
	static void FindOutputMode() {
		for (unsigned a = 0; a < outputs.Size(); a++) {
			DXGI_OUTPUT_DESC outputDesc;
			outputs[a].output->GetDesc(&outputDesc);

			MONITORINFOEX monitorInfo;
			monitorInfo.cbSize = sizeof(MONITORINFOEX);//GetMonitorInfo를 호출하기 전 cbSize 맴버를 설정
			GetMonitorInfo(outputDesc.Monitor, &monitorInfo);
			DEVMODE devMode;
			devMode.dmSize = sizeof(DEVMODE);
			devMode.dmDriverExtra = 0;//EnumDisplaySettings를 호출하기 전 두 맴버를 설정
			EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);

			DXGI_MODE_DESC currentDisplayModeDesc;
			currentDisplayModeDesc.Width = devMode.dmPelsWidth;
			currentDisplayModeDesc.Height = devMode.dmPelsHeight;
			currentDisplayModeDesc.RefreshRate.Numerator = devMode.dmDisplayFrequency;
			currentDisplayModeDesc.RefreshRate.Denominator = 1;
			currentDisplayModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			currentDisplayModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			currentDisplayModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			DXGI_MODE_DESC outCurrentDisplayModeDesc;

			//currentDisplayModeDesc와 제일 가까운 모드를 찾는다.
			outputs[a].output->FindClosestMatchingMode(&currentDisplayModeDesc, &outCurrentDisplayModeDesc, nullptr);

			int i = outputs[a].num - 1;
			for (; i >= 0; i--) {
				if (Memory::Equal(&outCurrentDisplayModeDesc, &outputs[a].displayModes[i], 1)) {
					outputs[a].current = i;
					break;
				}
			}
			if (i == -1);
		}

	}
	static void FindOutputMode1() {
		for (unsigned a = 0; a < outputs.Size(); a++) {
			DXGI_OUTPUT_DESC outputDesc;
			outputs[a].output1->GetDesc(&outputDesc);

			MONITORINFOEX monitorInfo;
			monitorInfo.cbSize = sizeof(MONITORINFOEX);//GetMonitorInfo를 호출하기 전 cbSize 맴버를 설정
			GetMonitorInfo(outputDesc.Monitor, &monitorInfo);
			DEVMODE devMode;
			devMode.dmSize = sizeof(DEVMODE);
			devMode.dmDriverExtra = 0;//EnumDisplaySettings를 호출하기 전 두 맴버를 설정
			EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);

			DXGI_MODE_DESC1 currentDisplayModeDesc;
			currentDisplayModeDesc.Width = devMode.dmPelsWidth;
			currentDisplayModeDesc.Height = devMode.dmPelsHeight;
			currentDisplayModeDesc.RefreshRate.Numerator = devMode.dmDisplayFrequency;
			currentDisplayModeDesc.RefreshRate.Denominator = 1;
			currentDisplayModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			currentDisplayModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			currentDisplayModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			currentDisplayModeDesc.Stereo = FALSE;
			DXGI_MODE_DESC1 outCurrentDisplayModeDesc;

			//currentDisplayModeDesc와 제일 가까운 모드를 찾는다.
			outputs[a].output1->FindClosestMatchingMode1(&currentDisplayModeDesc, &outCurrentDisplayModeDesc, nullptr);

			int i = outputs[a].num - 1;
			for (; i >= 0; i--) {
				if (Memory::Equal(&outCurrentDisplayModeDesc, &outputs[a].displayModes1[i], 1)) {
					outputs[a].current = i;
					break;
				}
			}
			if (i == -1);
		}
	}

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

	/*	unsigned maxDisplaysLen = 10;
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
		}*/
		_DirectX11::device->QueryInterface(__uuidof(IDXGIDevice), (void**)&device);
		

		device->GetAdapter(&adapter);
		if (adapter == nullptr);

		//adapter->EnumOutputs(0, &output);//index가 장치 수를 벗어나면 DXGI_ERROR_NOT_FOUND반환
		//if (output == nullptr);

		outputs.Resize(outputs.Size() + 1);
		while (adapter->EnumOutputs(outputs.Size() - 1, &outputs[outputs.Size() - 1].output) != DXGI_ERROR_NOT_FOUND) {
			if (outputs.Size() == outputs.MaxSize()) {
				outputs.ReAlloc(outputs.Size() + 10);
			}
			outputs[outputs.Size() - 1].output->QueryInterface(&outputs[outputs.Size() - 1].output1);
			outputs.Resize(outputs.Size() + 1);
		}
		outputs.Resize(outputs.Size() - 1);


		if (outputs[0].output1) {
			for (auto& i : outputs) {
				DXGI_OUTPUT_DESC desc;
				i.output1->GetDesc(&desc);
				i.rect = desc.DesktopCoordinates;

				hr = i.output1->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING, &i.num, nullptr);//먼저 마지막 인수를 nullptr로 해서 모드 갯수를 가져옴.
				if (FAILED(hr));

				i.displayModes = nullptr;
				i.displayModes1 = new DXGI_MODE_DESC1[i.num];

				hr = i.output1->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING, &i.num, i.displayModes1);
				if (FAILED(hr));
			}
			FindOutputMode1();
		} else {
			for (auto& i : outputs) {
				DXGI_OUTPUT_DESC desc;
				i.output->GetDesc(&desc);
				i.rect = desc.DesktopCoordinates;

				hr = i.output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING, &i.num, nullptr);//먼저 마지막 인수를 nullptr로 해서 모드 갯수를 가져옴.
				if (FAILED(hr));

				i.displayModes = new DXGI_MODE_DESC[i.num];
				i.displayModes1 = nullptr;

				hr = i.output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING, &i.num, i.displayModes);
				if (FAILED(hr));
			}
			FindOutputMode();
		}

	}
	void Init(System::CreateInfo* _info) {
		HRESULT hr;

		/*int i;
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
		}*/
		int i;
		if (_info->screenMode == System::ScreenMode::Fullscreen || _info->screenMode == System::ScreenMode::BorderlessScreen) {
			if (_info->screenIndex > outputs.Size() - 1)currentOutput = 0;
			else currentOutput = _info->screenIndex;

			if (outputs[currentOutput].output1) {
				i = outputs[currentOutput].num - 1;
				for (; i >= 0; i--) {
					if (_info->windowSize == PointU(outputs[currentOutput].displayModes1[i].Width, outputs[currentOutput].displayModes1[i].Height) 
						&& _info->refleshRateTop == outputs[currentOutput].displayModes1[i].RefreshRate.Numerator && _info->refleshRateBottom == outputs[currentOutput].displayModes1[i].RefreshRate.Denominator) {
						currentDisplayMode = i;
						break;
					}
				}
				if (i == -1) {
					currentDisplayMode = outputs[currentOutput].current;
					_info->windowSize = PointU(outputs[currentOutput].displayModes1[outputs[currentOutput].current].Width, outputs[currentOutput].displayModes1[outputs[currentOutput].current].Height);
					_info->refleshRateTop = outputs[currentOutput].displayModes1[outputs[currentOutput].current].RefreshRate.Numerator;
					_info->refleshRateBottom = outputs[currentOutput].displayModes1[outputs[currentOutput].current].RefreshRate.Denominator;
				}
			} else {
				i = outputs[currentOutput].num - 1;
				for (; i >= 0; i--) {
					if (_info->windowSize == PointU(outputs[currentOutput].displayModes[i].Width, outputs[currentOutput].displayModes[i].Height)
						&& _info->refleshRateTop == outputs[currentOutput].displayModes[i].RefreshRate.Numerator && _info->refleshRateBottom == outputs[currentOutput].displayModes[i].RefreshRate.Denominator) {
						currentDisplayMode = i;
						break;
					}
				}
				if (i == -1) {
					currentDisplayMode = outputs[currentOutput].current;
					_info->windowSize = PointU(outputs[currentOutput].displayModes[outputs[currentOutput].current].Width, outputs[currentOutput].displayModes[outputs[currentOutput].current].Height);
					_info->refleshRateTop = outputs[currentOutput].displayModes[outputs[currentOutput].current].RefreshRate.Numerator;
					_info->refleshRateBottom = outputs[currentOutput].displayModes[outputs[currentOutput].current].RefreshRate.Denominator;
				}
			}
			DWORD style = WS_CAPTION | WS_SYSMENU | (_System::_Windows::resizeWindow ? WS_THICKFRAME : 0) | (_System::_Windows::maximized ? WS_MAXIMIZEBOX : 0)
				| (_System::_Windows::minimized ? WS_MINIMIZEBOX : 0);

			_System::_Windows::hWnd = CreateWindowEx(WS_EX_APPWINDOW, _T("Xfit"), _info->title, WS_POPUP | WS_VISIBLE, 0, 0,
				_info->windowSize.width, _info->windowSize.height, nullptr, nullptr, _System::_Windows::hInstance, nullptr);
			if (_System::_Windows::hWnd == nullptr);
		} else {
			currentOutput = -1;
			currentDisplayMode = -1;

			if (outputs[0].output1) {
				i = outputs[0].num - 1;
				for (; i >= 0; i--) {
					if (_info->refleshRateTop == outputs[0].displayModes1[i].RefreshRate.Numerator && _info->refleshRateBottom == outputs[0].displayModes1[i].RefreshRate.Denominator) {
						break;
					}
				}
				if (i == -1) {
					_info->refleshRateTop = outputs[0].displayModes1[outputs[0].current].RefreshRate.Numerator;
					_info->refleshRateBottom = outputs[0].displayModes1[outputs[0].current].RefreshRate.Denominator;
				}
			} else {
				i = outputs[0].num - 1;
				for (; i >= 0; i--) {
					if (_info->refleshRateTop == outputs[0].displayModes[i].RefreshRate.Numerator && _info->refleshRateBottom == outputs[0].displayModes[i].RefreshRate.Denominator) {
						break;
					}
				}
				if (i == -1) {
					_info->refleshRateTop = outputs[0].displayModes[outputs[0].current].RefreshRate.Numerator;
					_info->refleshRateBottom = outputs[0].displayModes[outputs[0].current].RefreshRate.Denominator;
				}
			}

			DWORD style = WS_CAPTION | WS_SYSMENU | (_System::_Windows::resizeWindow ? WS_THICKFRAME : 0) | (_System::_Windows::maximized ? WS_MAXIMIZEBOX : 0)
				| (_System::_Windows::minimized ? WS_MINIMIZEBOX : 0);

			RECT rect = { 0,0, _info->windowSize.width, _info->windowSize.height };
			AdjustWindowRectEx(&rect, style, FALSE, 0);
			_System::_Windows::hWnd = CreateWindowEx(0, _T("Xfit"), _info->title, style, _info->windowPos.x, _info->windowPos.y,
				rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, _System::_Windows::hInstance, nullptr);
		}
		_Renderer::windowSize = _info->windowSize;
		_Renderer::vSync = _info->vSync;
		_Renderer::refleshRateTop = _info->refleshRateTop;
		_Renderer::refleshRateBottom = _info->refleshRateBottom;

		if (factory2) {
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
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

			if (_info->screenMode == System::ScreenMode::Fullscreen) {
				if (outputs[currentOutput].output1) {
					swapChainDesc.Format = outputs[currentOutput].displayModes1[currentDisplayMode].Format;
					fullscreenDesc.RefreshRate = outputs[currentOutput].displayModes1[currentDisplayMode].RefreshRate;
					fullscreenDesc.Scaling = outputs[currentOutput].displayModes1[currentDisplayMode].Scaling;
					fullscreenDesc.ScanlineOrdering = outputs[currentOutput].displayModes1[currentDisplayMode].ScanlineOrdering;
					fullscreenDesc.Windowed = false;

					hr = factory2->CreateSwapChainForHwnd(_DirectX11::device1, _System::_Windows::hWnd, &swapChainDesc, &fullscreenDesc, outputs[currentOutput].output1, &swapChain1);
				} else {
					swapChainDesc.Format = outputs[currentOutput].displayModes[currentDisplayMode].Format;
					fullscreenDesc.RefreshRate = outputs[currentOutput].displayModes[currentDisplayMode].RefreshRate;
					fullscreenDesc.Scaling = outputs[currentOutput].displayModes[currentDisplayMode].Scaling;
					fullscreenDesc.ScanlineOrdering = outputs[currentOutput].displayModes[currentDisplayMode].ScanlineOrdering;
					fullscreenDesc.Windowed = false;

					hr = factory2->CreateSwapChainForHwnd(_DirectX11::device1, _System::_Windows::hWnd, &swapChainDesc, &fullscreenDesc, outputs[currentOutput].output, &swapChain1);
				}
			} else {
				swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				hr = factory2->CreateSwapChainForHwnd(_DirectX11::device1, _System::_Windows::hWnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
			}
			
			swapChain1->QueryInterface(&swapChain);
			swapChain1->QueryInterface(&swapChain2);
			if (swapChain2)swapChain1->QueryInterface(&swapChain3);
			factory2->QueryInterface(&factory1);
		} else {
			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			
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
			swapChainDesc.OutputWindow = _System::_Windows::hWnd;

			swapChainDesc.BufferDesc.Width = _Renderer::windowSize.width;
			swapChainDesc.BufferDesc.Height = _Renderer::windowSize.height;

			if (_info->screenMode == System::ScreenMode::Fullscreen) {
				swapChainDesc.Windowed = false;

				swapChainDesc.BufferDesc.RefreshRate.Numerator = _info->refleshRateTop;
				swapChainDesc.BufferDesc.RefreshRate.Denominator = _info->refleshRateBottom;

				if (outputs[currentOutput].output1) {
					swapChainDesc.BufferDesc.Format = outputs[currentOutput].displayModes1[currentDisplayMode].Format;
					swapChainDesc.BufferDesc.Scaling = outputs[currentOutput].displayModes1[currentDisplayMode].Scaling;
					swapChainDesc.BufferDesc.ScanlineOrdering = outputs[currentOutput].displayModes1[currentDisplayMode].ScanlineOrdering;
				} else {
					swapChainDesc.BufferDesc.Format = outputs[currentOutput].displayModes[currentDisplayMode].Format;
					swapChainDesc.BufferDesc.Scaling = outputs[currentOutput].displayModes[currentDisplayMode].Scaling;
					swapChainDesc.BufferDesc.ScanlineOrdering = outputs[currentOutput].displayModes[currentDisplayMode].ScanlineOrdering;
				}
			} else {
				swapChainDesc.Windowed = true;

				swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				swapChainDesc.BufferDesc.RefreshRate.Numerator = _info->refleshRateTop;
				swapChainDesc.BufferDesc.RefreshRate.Denominator = _info->refleshRateBottom;
				swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
				swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			}

			hr = factory1->CreateSwapChain(_DirectX11::device, &swapChainDesc, &swapChain);
		}
		hr = factory1->MakeWindowAssociation(_System::_Windows::hWnd, 0);
		/*if (_info->screenMode == System::ScreenMode::Fullscreen) {
			if (outputs[currentOutput].output1) {
				swapChain->SetFullscreenState(TRUE, outputs[currentOutput].output1);
			} else {
				swapChain->SetFullscreenState(TRUE, outputs[currentOutput].output);
			}
		}*/


		_Renderer::msaaCount = _info->msaaCount;
		_Renderer::msaaQuality = _info->msaaQuality;
	}
	void Render() {
		if (swapChain1)swapChain1->Present(_Renderer::vSync ? 1 : 0, 0);
		else swapChain->Present(_Renderer::vSync ? 1 : 0, 0);
	}
	void SetFullScreenMode(unsigned _displayIndex, unsigned _displayModeIndex) {
		if (outputs[_displayIndex].output1) {
			_Renderer::windowSize.width = outputs[_displayIndex].displayModes1[_displayModeIndex].Width;
			_Renderer::windowSize.height = outputs[_displayIndex].displayModes1[_displayModeIndex].Height;
			_Renderer::refleshRateTop = outputs[_displayIndex].displayModes1[_displayModeIndex].RefreshRate.Numerator;
			_Renderer::refleshRateBottom = outputs[_displayIndex].displayModes1[_displayModeIndex].RefreshRate.Denominator;
		} else {
			_Renderer::windowSize.width = outputs[_displayIndex].displayModes[_displayModeIndex].Width;
			_Renderer::windowSize.height = outputs[_displayIndex].displayModes[_displayModeIndex].Height;
			_Renderer::refleshRateTop = outputs[_displayIndex].displayModes[_displayModeIndex].RefreshRate.Numerator;
			_Renderer::refleshRateBottom = outputs[_displayIndex].displayModes[_displayModeIndex].RefreshRate.Denominator;
		}
		
		currentOutput = _displayIndex;
		currentDisplayMode = _displayModeIndex;
	}

	void SetBorderlessScreenMode(unsigned _displayIndex) {
		if (outputs[_displayIndex].output1) {
			_Renderer::windowSize.width = outputs[_displayIndex].displayModes1[outputs[_displayIndex].current].Width;
			_Renderer::windowSize.height = outputs[_displayIndex].displayModes1[outputs[_displayIndex].current].Height;
		} else {
			_Renderer::windowSize.width = outputs[_displayIndex].displayModes[outputs[_displayIndex].current].Width;
			_Renderer::windowSize.height = outputs[_displayIndex].displayModes[outputs[_displayIndex].current].Height;
		}
	}

	int GetDisplayFromWindow() {
		const auto hMonitor = MonitorFromWindow(_Windows::hWnd, MONITOR_DEFAULTTONEAREST);

		if (outputs[0].output1) {
			for (unsigned i = 0; i < outputs.Size(); i++) {
				DXGI_OUTPUT_DESC outputDesc;
				outputs[i].output1->GetDesc(&outputDesc);
				if (outputDesc.Monitor == hMonitor)return i;
			}
		} else {
			for (unsigned i = 0; i < outputs.Size(); i++) {
				DXGI_OUTPUT_DESC outputDesc;
				outputs[i].output->GetDesc(&outputDesc);
				if (outputDesc.Monitor == hMonitor)return i;
			}
		}
		return -1;
	}
	void SetWindowMode() {
		currentOutput = -1;
		currentDisplayMode = -1;
	}
	void Release() {
		SafeRelease(&device);
		SafeRelease(&adapter);

		SafeRelease(&factory1);
		SafeRelease(&factory2);

		SafeRelease(&swapChain);
		SafeRelease(&swapChain1);
		SafeRelease(&swapChain2);
		SafeRelease(&swapChain3);

		for (auto& i : outputs) {
			SafeRelease(&i.output);
			SafeRelease(&i.output1);

			if(i.displayModes)delete[]i.displayModes;
			if(i.displayModes1)delete[]i.displayModes1;
		}
	}
}