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

	inline IDXGIDevice* device = nullptr;
	//inline IDXGIDevice1* device1 = nullptr;
	//inline IDXGIDevice2* device2 = nullptr;
	//inline IDXGIDevice3* device3 = nullptr;
	//inline IDXGIDevice4* device4 = nullptr;


	inline IDXGIAdapter* adapter = nullptr;
	//inline IDXGIAdapter1* adapter1 = nullptr;
	//inline IDXGIAdapter2* adapter2 = nullptr;
	//inline IDXGIAdapter3* adapter3 = nullptr;


	//inline IDXGIOutput2* output2 = nullptr;
	//inline IDXGIOutput3* output3 = nullptr;
	//inline IDXGIOutput4* output4 = nullptr;
	//inline IDXGIOutput5* output5 = nullptr;

	inline int currentOutput = -1;
	inline int currentDisplayMode = -1;

	struct Output {
		unsigned num;
		unsigned current;
		IDXGIOutput* output;
		IDXGIOutput1* output1;
	
		DXGI_MODE_DESC* displayModes;
		DXGI_MODE_DESC1* displayModes1;
		RECT rect;
	};
	inline Array<Output> outputs(10);

	void Create();
	void Init(System::CreateInfo* _info);
	void Render();

	void SetFullScreenMode(unsigned _displayIndex, unsigned _displayModeIndex);
	void SetBorderlessScreenMode(unsigned _displayIndex);
	int GetDisplayFromWindow();
	void SetWindowMode();

	void Release();
}

#endif