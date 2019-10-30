#include "Blend.h"

#include "../_system/_DirectX11.h"

#ifdef _WIN32
Blend::Blend(Value _srcColor, Value _destColor, Value _srcAlpha, Value _destAlpha, Equation _colorEquation, Equation _alphaEquation,
	float _constR/* = 1.f*/, float _constG/* = 1.f*/, float _constB/* = 1.f*/, float _constA/* = 1.f*/):
	consts{ _constR,  _constG ,_constB, _constA } {
	HRESULT hr;

	if (_System::_DirectX11::device1) {
		D3D11_BLEND_DESC1 blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		//AlphaToCoverageEnable와 IndependentBlendEnable는 false

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = DirectX11Value[(int)_srcColor];
		blendDesc.RenderTarget[0].DestBlend = DirectX11Value[(int)_destColor];
		blendDesc.RenderTarget[0].BlendOp = DirectX11Equation[(int)_colorEquation];
		blendDesc.RenderTarget[0].SrcBlendAlpha = DirectX11Value[(int)_srcAlpha];
		blendDesc.RenderTarget[0].DestBlendAlpha = DirectX11Value[(int)_destAlpha];
		blendDesc.RenderTarget[0].BlendOpAlpha = DirectX11Equation[(int)_alphaEquation];
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[0].LogicOpEnable = false;
		blendDesc.RenderTarget[0].LogicOp = D3D11_LOGIC_OP_SET;

		hr = _System::_DirectX11::device1->CreateBlendState1(&blendDesc, &blendState1);
		if (FAILED(hr));

		hr = blendState1->QueryInterface(&blendState);
		if (FAILED(hr));
	} else {
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		//AlphaToCoverageEnable와 IndependentBlendEnable는 false

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = DirectX11Value[(int)_srcColor];
		blendDesc.RenderTarget[0].DestBlend = DirectX11Value[(int)_destColor];
		blendDesc.RenderTarget[0].BlendOp = DirectX11Equation[(int)_colorEquation];
		blendDesc.RenderTarget[0].SrcBlendAlpha = DirectX11Value[(int)_srcAlpha];
		blendDesc.RenderTarget[0].DestBlendAlpha = DirectX11Value[(int)_destAlpha];
		blendDesc.RenderTarget[0].BlendOpAlpha = DirectX11Equation[(int)_alphaEquation];
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = _System::_DirectX11::device->CreateBlendState(&blendDesc, &blendState);
		if (FAILED(hr));
	}
}
#elif __ANDROID__
Blend::Blend(Value _srcColor, Value _destColor, Value _srcAlpha, Value _destAlpha, Equation _colorEquation, Equation _alphaEquation,
	float _constR/* = 1.f*/, float _constG/* = 1.f*/, float _constB/* = 1.f*/, float _constA/* = 1.f*/) :
	srcColor(_srcColor), destColor(_destColor), srcAlpha(_srcAlpha),
	destAlpha(_destAlpha), colorEquation(_colorEquation), alphaEquation(_alphaEquation), consts{ _constR,  _constG ,_constB, _constA } {}
#endif

Blend::~Blend() {
	if (blendState1)blendState1->Release();
	blendState->Release();
}