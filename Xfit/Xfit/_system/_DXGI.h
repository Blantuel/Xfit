#pragma once

#ifdef _WIN32

#include "../system/System.h"
#include "../data/Array.h"

namespace _System::_DXGI {
	inline IDXGISwapChain* swapChain = nullptr;
	inline IDXGISwapChain1* swapChain1 = nullptr;
	inline IDXGISwapChain2* swapChain2 = nullptr;
	inline IDXGISwapChain3* swapChain3 = nullptr;
	//inline IDXGISwapChain4* swapChain4=nullptr;

	inline IDXGIFactory1* factory1 = nullptr;
	inline IDXGIFactory2* factory2 = nullptr;
	//inline IDXGIFactory3* factory3=nullptr;
	//inline IDXGIFactory4* factory4=nullptr;
	//inline IDXGIFactory5* factory5=nullptr;
	//inline IDXGIFactory6* factory6=nullptr;

	inline int currentDisplay = -1;
	inline int currentDisplayMode = -1;

	struct DisplayMode {
		unsigned width;
		unsigned height;
		unsigned refleshRate;
	};
	struct Display {
		unsigned modesLen;
		unsigned currentMode;
		HMONITOR hMonitor;
		DisplayMode* modes;
		Point displayPos;
	};
	inline Display* displays;
	inline unsigned displaysLen;

	void Create();
	void Init(System::CreateInfo* _info);
	void Render();

	void SetFullScreenMode(unsigned _displayIndex, unsigned _displayModeIndex);
	int GetDisplayFromWindow();
	void SetWindowMode();
}

#endif