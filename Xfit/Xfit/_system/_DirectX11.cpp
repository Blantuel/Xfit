#ifdef _WIN32

#include "_DirectX11.h"
#include "_DXGI.h"
#include "_Windows.h"
#include "_Renderer.h"

#include "../file/File.h"

#include "_XfitDataFiles.h"

using namespace XfitFileData;

#define COMPILE 1

#define COMPILE_SHADER_PATH "../Xfit/"

#define SHADER_PATH "../Xfit/_shader/out/"


static unsigned char* shaderData;

namespace _System::_DirectX11 {
	static void LoadVertexShader(const unsigned* _path, ID3D11VertexShader** _shader, D3D11_INPUT_ELEMENT_DESC* _inputElement, ID3D11InputLayout** _outLayout, unsigned _elementSize, 
		ID3D11ClassLinkage** _outClassLinkage = nullptr, ID3D11ShaderReflection** _outReflection = nullptr) {
		HRESULT hr;
		if (_outClassLinkage) {
			hr = device->CreateClassLinkage(_outClassLinkage);
			hr = device->CreateVertexShader(&shaderData[_path[0]], _path[1], *_outClassLinkage, _shader);
			hr = D3DReflect(&shaderData[_path[0]], _path[1], __uuidof(ID3D11ShaderReflection), (void**)_outReflection);
		} else {
			hr = device->CreateVertexShader(&shaderData[_path[0]], _path[1], nullptr, _shader);
		}
		if (FAILED(hr)) {
			wchar_t text[20];
			swprintf_s(text, 20, L"%x", hr);
			MessageBox(nullptr, text, L"오류", MB_OK);
		}

		hr = device1->CreateInputLayout(_inputElement, _elementSize, &shaderData[_path[0]], _path[1], _outLayout);
		if (FAILED(hr));
	}
	static void LoadPixelShader(const unsigned* _path, ID3D11PixelShader** _shader, ID3D11ClassLinkage** _outClassLinkage = nullptr, ID3D11ShaderReflection** _outReflection = nullptr) {

		HRESULT hr;
		if (_outClassLinkage) {
			hr = device->CreateClassLinkage(_outClassLinkage);
			hr = device->CreatePixelShader(&shaderData[_path[0]], _path[1], *_outClassLinkage, _shader);
			hr = D3DReflect(&shaderData[_path[0]], _path[1], __uuidof(ID3D11ShaderReflection), (void**)_outReflection);
		} else {
			hr = device->CreatePixelShader(&shaderData[_path[0]], _path[1], nullptr , _shader);
		}
		if (FAILED(hr)) {
			wchar_t text[20];
			swprintf_s(text, 20, L"%x", hr);
			MessageBox(nullptr, text, L"오류", MB_OK);
		}
		if (FAILED(hr));
	}
	static void LoadGeometryShader(const unsigned* _path, ID3D11GeometryShader** _shader, ID3D11ClassLinkage** _outClassLinkage = nullptr, ID3D11ShaderReflection** _outReflection = nullptr) {
		HRESULT hr;
		if (_outClassLinkage) {
			hr = device->CreateClassLinkage(_outClassLinkage);
			hr = device->CreateGeometryShader(&shaderData[_path[0]], _path[1], *_outClassLinkage, _shader);
			hr = D3DReflect(&shaderData[_path[0]], _path[1], __uuidof(ID3D11ShaderReflection), (void**)_outReflection);
		} else {
			hr = device->CreateGeometryShader(&shaderData[_path[0]], _path[1], nullptr, _shader);
		}
		if (FAILED(hr)) {
			wchar_t text[20];
			swprintf_s(text, 20, L"%x", hr);
			MessageBox(nullptr, text, L"오류", MB_OK);
		}
		if (FAILED(hr));
	}
	static void LoadCompileVertexShader(const char* _path, ID3D11VertexShader** _shader, D3D11_INPUT_ELEMENT_DESC* _inputElement, ID3D11InputLayout** _outLayout, unsigned _elementSize,
		ID3D11ClassLinkage** _outClassLinkage = nullptr, ID3D11ShaderReflection** _outReflection = nullptr) {
		File file(_path);
		const unsigned size = file.GetSize();
		unsigned char* data = new unsigned char[size];
		file.ReadBytes(size, data);

		HRESULT hr;

		ID3DBlob* compiledData, *error;
		hr = D3DCompile(data, size, nullptr, nullptr, nullptr, "main", "vs_5_0", 0,0,&compiledData,&error);
		 
		if (FAILED(hr)) {
			if (error) {
				MessageBoxA(nullptr, (const char*)error->GetBufferPointer(), "Vertex Shader Compile Error", MB_OK);
				error->Release();
			}
		}
		
		if (_outClassLinkage) {
			hr = device->CreateClassLinkage(_outClassLinkage);
			hr = device->CreateVertexShader(compiledData->GetBufferPointer(), compiledData->GetBufferSize(), *_outClassLinkage, _shader);
			hr = D3DReflect(compiledData->GetBufferPointer(), compiledData->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**)_outReflection);
		} else {
			hr = device->CreateVertexShader(compiledData->GetBufferPointer(), compiledData->GetBufferSize(), nullptr, _shader);
		}
		if (FAILED(hr)) {
			wchar_t text[20];
			swprintf_s(text, 20, L"%x", hr);
			MessageBox(nullptr, text, L"오류", MB_OK);
		}

		hr = device1->CreateInputLayout(_inputElement, _elementSize, compiledData->GetBufferPointer(), compiledData->GetBufferSize(), _outLayout);
		
		compiledData->Release();
		delete[]data;
		if (FAILED(hr));
	}
	static void LoadCompilePixelShader(const char* _path, ID3D11PixelShader** _shader, ID3D11ClassLinkage** _outClassLinkage = nullptr, ID3D11ShaderReflection** _outReflection = nullptr) {
		File file(_path);
		const unsigned size = file.GetSize();
		unsigned char* data = new unsigned char[size];
		file.ReadBytes(size, data);

		HRESULT hr;

		ID3DBlob* compiledData, * error;
		hr = D3DCompile(data, size, nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &compiledData, &error);

		if (FAILED(hr)) {
			if (error) {
				MessageBoxA(nullptr, (const char*)error->GetBufferPointer(), "Pixel Shader Compile Error", MB_OK);
				error->Release();
			}
		}

		if (_outClassLinkage) {
			hr = device->CreateClassLinkage(_outClassLinkage);
			hr = device->CreatePixelShader(compiledData->GetBufferPointer(), compiledData->GetBufferSize(), *_outClassLinkage, _shader);
			hr = D3DReflect(compiledData->GetBufferPointer(), compiledData->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**)_outReflection);
		} else {
			hr = device->CreatePixelShader(compiledData->GetBufferPointer(), compiledData->GetBufferSize(), nullptr, _shader);
		}
		if (FAILED(hr)) {
			wchar_t text[20];
			swprintf_s(text, 20, L"%x", hr);
			MessageBox(nullptr, text, L"오류", MB_OK);
		}
		compiledData->Release();

		delete[]data;
		if (FAILED(hr));
	}
	static void LoadCompileGeometryShader(const char* _path, ID3D11GeometryShader** _shader, ID3D11ClassLinkage** _outClassLinkage = nullptr, ID3D11ShaderReflection** _outReflection = nullptr) {
		File file(_path);
		const unsigned size = file.GetSize();
		unsigned char* data = new unsigned char[size];
		file.ReadBytes(size, data);
		
		HRESULT hr;

		ID3DBlob* compiledData, * error;
		hr = D3DCompile(data, size, nullptr, nullptr, nullptr, "main", "gs_5_0", 0, 0, &compiledData, &error);

		if (FAILED(hr)) {
			if (error) {
				MessageBoxA(nullptr, (const char*)error->GetBufferPointer(), "Geometry Shader Compile Error", MB_OK);
				error->Release();
			}
		}
		if (_outClassLinkage) {
			hr = device->CreateClassLinkage(_outClassLinkage);
			hr = device->CreateGeometryShader(compiledData->GetBufferPointer(), compiledData->GetBufferSize(), *_outClassLinkage, _shader);
			hr = D3DReflect(compiledData->GetBufferPointer(), compiledData->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**)_outReflection);
		} else {
			hr = device->CreateGeometryShader(compiledData->GetBufferPointer(), compiledData->GetBufferSize(), nullptr, _shader);
		}
		if (FAILED(hr)) {
			wchar_t text[20];
			swprintf_s(text, 20, L"%x", hr);
			MessageBox(nullptr, text, L"오류", MB_OK);
		}
		compiledData->Release();

		delete[]data;
		if (FAILED(hr));
	}

	void Clear(bool _clearDepth) {
		context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)_Renderer::windowSize.width;
		viewport.Height = (FLOAT)_Renderer::windowSize.height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		context->RSSetViewports(1, &viewport);

		context->ClearRenderTargetView(renderTargetView, _Renderer::backColor);
		if (_clearDepth)context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0xff);
	}
	void Release() {
		SafeRelease(&device);
		SafeRelease(&device1);
		SafeRelease(&device2);
		SafeRelease(&device3);

		SafeRelease(&context);
		SafeRelease(&context1);

		SafeRelease(&renderTargetView);
		SafeRelease(&renderTargetView1);

		SafeRelease(&backBuffer);
		SafeRelease(&backBuffer1);

		SafeRelease(&depthStencilView);

		SafeRelease(&rasterizerState);
		SafeRelease(&rasterizerState1);

		SafeRelease(&rasterizerStateCullingBack);
		SafeRelease(&rasterizerStateCullingBack1);

		SafeRelease(&rasterizerStateCullingFront);
		SafeRelease(&rasterizerStateCullingFront1);

		SafeRelease(&depthStencilState);
		SafeRelease(&depthStencilState2D);

		SafeRelease(&imgVert2DShader);
		SafeRelease(&imgVert2DLayout);
		SafeRelease(&imgPx2DShader);

		SafeRelease(&imgVertConstantBuffer2D);
		SafeRelease(&imgPxConstantBuffer2D);

		SafeRelease(&shapeVert2DShader);
		SafeRelease(&shapeVert2DLayout);
		SafeRelease(&shapeGeo2DShader);
		SafeRelease(&shapePx2DShader);

		SafeRelease(&shapeVertConstantBuffer2D);
		SafeRelease(&shapeGeoConstantBuffer2D);

		SafeRelease(&lineGeo2DShader);
		SafeRelease(&linePx2DShader);

		SafeRelease(&lineGeoConstantBuffer2D);
		SafeRelease(&linePxConstantBuffer2D);

		SafeRelease(&imgInsVert2DShader);
		SafeRelease(&imgInsVert2DLayout);
		SafeRelease(&imgInsPx2DShader);

		SafeRelease(&imgMultiInsVert2DShader);
		SafeRelease(&imgMultiInsVert2DLayout);
		SafeRelease(&imgMultiInsPx2DShader);

		SafeRelease(&shapeInsVert2DShader);
		SafeRelease(&shapeInsVert2DLayout);
		SafeRelease(&shapeInsGeo2DShader);

		SafeRelease(&shapeInsVertConstantBuffer2D);

		SafeRelease(&lineInsVert2DShader);
		SafeRelease(&lineInsVert2DLayout);
		SafeRelease(&lineInsGeo2DShader);

		SafeRelease(&lineInsGeoConstantBuffer2D);
	}
	void Create() {
		HRESULT hr;

#ifdef _DEBUG
		constexpr UINT flag = D3D11_CREATE_DEVICE_DEBUG;
#else
		constexpr UINT flag = 0;
#endif
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag, nullptr, 0, D3D11_SDK_VERSION, &device, nullptr, &context);
		
		if (FAILED(hr)) {
			wchar_t text[20];
			swprintf_s(text, 20, L"%x", hr);
			MessageBox(nullptr, text, L"오류", MB_OK);
		}

		device->QueryInterface(&device1);//실패시 device1이 nullptr이됨.
		if (device1)device->QueryInterface(&device2);
		if (device2)device->QueryInterface(&device3);

		context->QueryInterface(&context1);

		contextT = context;
		context1T = context1;
	}
	unsigned GetMaxMsaaQuality(unsigned _sampleCount) {
#ifdef _DEBUG
		if (device == nullptr);
#endif
		UINT quality;
		device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, _sampleCount, &quality);
		return quality - 1;
	}
	void Init(System::CreateInfo* _info) {
		HRESULT hr;

		hr = _DXGI::swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D1), (void**)&backBuffer1);
		if (FAILED(hr))hr = _DXGI::swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		if (FAILED(hr));

		if (device3 != nullptr) {
			if (backBuffer1 != nullptr) {
				hr = device3->CreateRenderTargetView1(backBuffer1, nullptr, &renderTargetView1);
				if (FAILED(hr));
			} else {
				hr = device3->CreateRenderTargetView1(backBuffer, nullptr, &renderTargetView1);
				if (FAILED(hr));
			}
			hr = renderTargetView1->QueryInterface(&renderTargetView);
			if (FAILED(hr));
		} else {
			if (backBuffer1 != nullptr) {
				hr = device->CreateRenderTargetView(backBuffer1, nullptr, &renderTargetView);
				if (FAILED(hr));
			} else {
				hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
				if (FAILED(hr));
			}
		}

		if (device3 != nullptr) {
			ID3D11Texture2D1* depthStencilTexture;

			D3D11_TEXTURE2D_DESC1 texture2DDesc;
			texture2DDesc.ArraySize = 1;
			texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			texture2DDesc.CPUAccessFlags = 0;
			texture2DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			texture2DDesc.Width = _Renderer::windowSize.width;
			texture2DDesc.Height = _Renderer::windowSize.height;
			texture2DDesc.MipLevels = 1;
			texture2DDesc.MiscFlags = 0;
			texture2DDesc.SampleDesc.Count = _Renderer::msaaCount;
			texture2DDesc.SampleDesc.Quality = _Renderer::msaaQuality;
			texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
			texture2DDesc.TextureLayout = D3D11_TEXTURE_LAYOUT_UNDEFINED;

			hr = device3->CreateTexture2D1(&texture2DDesc, nullptr, &depthStencilTexture);
			if (FAILED(hr));

			hr = device3->CreateDepthStencilView(depthStencilTexture, nullptr, &depthStencilView);
			if (FAILED(hr));

			depthStencilTexture->Release();
		} else {
			ID3D11Texture2D* depthStencilTexture;

			D3D11_TEXTURE2D_DESC texture2DDesc;
			texture2DDesc.ArraySize = 1;
			texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			texture2DDesc.CPUAccessFlags = 0;
			texture2DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			texture2DDesc.Width = _Renderer::windowSize.width;
			texture2DDesc.Height = _Renderer::windowSize.height;
			texture2DDesc.MipLevels = 1;
			texture2DDesc.MiscFlags = 0;
			texture2DDesc.SampleDesc.Count = _Renderer::msaaCount;
			texture2DDesc.SampleDesc.Quality = _Renderer::msaaQuality;
			texture2DDesc.Usage = D3D11_USAGE_DEFAULT;

			hr = device->CreateTexture2D(&texture2DDesc, nullptr, &depthStencilTexture);
			if (FAILED(hr));

			hr = device->CreateDepthStencilView(depthStencilTexture, nullptr, &depthStencilView);
			if (FAILED(hr));

			depthStencilTexture->Release();
		}

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		depthStencilDesc.StencilEnable = FALSE;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		hr = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
		if (FAILED(hr));

		depthStencilDesc.DepthEnable = FALSE;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;

		hr = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState2D);
		if (FAILED(hr));

		if (device1 != nullptr) {
			D3D11_RASTERIZER_DESC1 rasterizerDesc;
			rasterizerDesc.AntialiasedLineEnable = false;
			rasterizerDesc.DepthClipEnable = false;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthBias = 0;
			rasterizerDesc.DepthBiasClamp = 0.f;
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.FrontCounterClockwise = false;
			rasterizerDesc.MultisampleEnable = false;
			rasterizerDesc.ScissorEnable = false;
			rasterizerDesc.SlopeScaledDepthBias = false;
			rasterizerDesc.ForcedSampleCount = 0;//기본값 : 사용하지 않음

			hr = device1->CreateRasterizerState1(&rasterizerDesc, &rasterizerState1);
			if (FAILED(hr));
			rasterizerState1->QueryInterface(&rasterizerState);
			if (FAILED(hr));

			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			hr = device1->CreateRasterizerState1(&rasterizerDesc, &rasterizerStateCullingBack1);
			if (FAILED(hr));
			rasterizerStateCullingBack1->QueryInterface(&rasterizerStateCullingBack);
			if (FAILED(hr));

			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
			hr = device1->CreateRasterizerState1(&rasterizerDesc, &rasterizerStateCullingFront1);
			if (FAILED(hr));
			rasterizerStateCullingFront1->QueryInterface(&rasterizerStateCullingFront);
			if (FAILED(hr));
		} else {
			D3D11_RASTERIZER_DESC rasterizerDesc;
			rasterizerDesc.AntialiasedLineEnable = false;
			rasterizerDesc.DepthClipEnable = false;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthBias = 0;
			rasterizerDesc.DepthBiasClamp = 0.f;
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.FrontCounterClockwise = false;
			rasterizerDesc.MultisampleEnable = false;
			rasterizerDesc.ScissorEnable = false;
			rasterizerDesc.SlopeScaledDepthBias = false;

			hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
			if (FAILED(hr));

			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerStateCullingBack);
			if (FAILED(hr));

			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
			hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerStateCullingFront);
			if (FAILED(hr));
		}

		D3D11_BUFFER_DESC constantBufferDesc;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = 0;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		constantBufferDesc.ByteWidth = sizeof(ImgVertConstantStruct2D);
		hr = device->CreateBuffer(&constantBufferDesc, nullptr, &imgVertConstantBuffer2D);
		if (FAILED(hr));

		constantBufferDesc.ByteWidth = sizeof(ImgPxConstantStruct2D);
		hr = device->CreateBuffer(&constantBufferDesc, nullptr, &imgPxConstantBuffer2D);
		if (FAILED(hr));

		constantBufferDesc.ByteWidth = sizeof(ShapeVertConstantStruct2D);
		hr = device->CreateBuffer(&constantBufferDesc, nullptr, &shapeVertConstantBuffer2D);
		if (FAILED(hr));

		constantBufferDesc.ByteWidth = sizeof(ShapeGeoConstantStruct2D);
		hr = device->CreateBuffer(&constantBufferDesc, nullptr, &shapeGeoConstantBuffer2D);
		if (FAILED(hr));

		constantBufferDesc.ByteWidth = sizeof(LinePxConstantStruct2D);
		hr = device->CreateBuffer(&constantBufferDesc, nullptr, &linePxConstantBuffer2D);
		if (FAILED(hr));

		constantBufferDesc.ByteWidth = sizeof(LineGeoConstantStruct2D);
		hr = device->CreateBuffer(&constantBufferDesc, nullptr, &lineGeoConstantBuffer2D);
		if (FAILED(hr));


		constantBufferDesc.ByteWidth = sizeof(ShapeInsVertConstantStruct2D);
		hr = device->CreateBuffer(&constantBufferDesc, nullptr, &shapeInsVertConstantBuffer2D);
		if (FAILED(hr));

		constantBufferDesc.ByteWidth = sizeof(LineInsGeoConstantStruct2D);
		hr = device->CreateBuffer(&constantBufferDesc, nullptr, &lineInsGeoConstantBuffer2D);
		if (FAILED(hr));
		/*
		LPCSTR SemanticName;
		UINT SemanticIndex;
		DXGI_FORMAT Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
		*/

#ifndef COMPILE 
		File file("XfitData.xfitData");

		const unsigned size = file.GetSize();

		shaderData = new unsigned char[size];

		file.ReadBytes(size, shaderData);

		file.Close();
#endif

		D3D11_INPUT_ELEMENT_DESC imgElement[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

#ifdef COMPILE 
		LoadCompileVertexShader(COMPILE_SHADER_PATH"imgVert2D.hlsl", &imgVert2DShader, imgElement, &imgVert2DLayout, ARRAYSIZE(imgElement));
		LoadCompilePixelShader(COMPILE_SHADER_PATH"imgPx2D.hlsl", &imgPx2DShader);
#else
		LoadVertexShader(shaders::imgVert2D_cso, &imgVert2DShader, imgElement, &imgVert2DLayout, ARRAYSIZE(imgElement));
		LoadPixelShader(shaders::imgPx2D_cso, &imgPx2DShader);
#endif
		D3D11_INPUT_ELEMENT_DESC shapeElement[]{
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

#ifdef COMPILE 
		LoadCompileVertexShader(COMPILE_SHADER_PATH"shapeVert2D.hlsl", &shapeVert2DShader, shapeElement, &shapeVert2DLayout, ARRAYSIZE(shapeElement));
		LoadCompileGeometryShader(COMPILE_SHADER_PATH"shapeGeo2D.hlsl", &shapeGeo2DShader);
		LoadCompilePixelShader(COMPILE_SHADER_PATH"shapePx2D.hlsl", &shapePx2DShader);
#else
		LoadVertexShader(shaders::shapeVert2D_cso, &shapeVert2DShader, shapeElement, &shapeVert2DLayout, ARRAYSIZE(shapeElement));
		LoadGeometryShader(shaders::shapeGeo2D_cso, &shapeGeo2DShader);
		LoadPixelShader(shaders::shapePx2D_cso, &shapePx2DShader);
#endif

#ifdef COMPILE 
		//shapeVert2D
		LoadCompileGeometryShader(COMPILE_SHADER_PATH"lineGeo2D.hlsl", &lineGeo2DShader);
		LoadCompilePixelShader(COMPILE_SHADER_PATH"linePx2D.hlsl", &linePx2DShader);
#else
		//shapeVert2D
		LoadGeometryShader(shaders::lineGeo2D_cso, &lineGeo2DShader);
		LoadPixelShader(shaders::linePx2D_cso, &linePx2DShader);
#endif

		D3D11_INPUT_ELEMENT_DESC imgInsElement[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"COLORMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"COLORMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"COLORMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"COLORMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 112, D3D11_INPUT_PER_INSTANCE_DATA, 1}
		};
#ifdef COMPILE 
		LoadCompileVertexShader(COMPILE_SHADER_PATH"imgInsVert2D.hlsl", &imgInsVert2DShader, imgInsElement, &imgInsVert2DLayout, ARRAYSIZE(imgInsElement));
		LoadCompilePixelShader(COMPILE_SHADER_PATH"imgInsPx2D.hlsl", &imgInsPx2DShader);
#else
		LoadVertexShader(shaders::imgInsVert2D_cso, &imgInsVert2DShader, imgInsElement, &imgInsVert2DLayout, ARRAYSIZE(imgInsElement));
		LoadPixelShader(shaders::imgInsPx2D_cso, &imgInsPx2DShader);
#endif

		D3D11_INPUT_ELEMENT_DESC imgMultiInsElement[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"COLORMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"COLORMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"COLORMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"COLORMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 112, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"IMGINDEX", 0, DXGI_FORMAT_R32_SINT, 2, 128, D3D11_INPUT_PER_INSTANCE_DATA, 1}
		};

#ifdef COMPILE 
		LoadCompileVertexShader(COMPILE_SHADER_PATH"imgMultiInsVert2D.hlsl", &imgMultiInsVert2DShader, imgMultiInsElement, &imgMultiInsVert2DLayout, ARRAYSIZE(imgMultiInsElement));
		LoadCompilePixelShader(COMPILE_SHADER_PATH"imgMultiInsPx2D.hlsl", &imgMultiInsPx2DShader);
#else
		LoadVertexShader(shaders::imgMultiInsVert2D_cso, &imgMultiInsVert2DShader, imgMultiInsElement, &imgMultiInsVert2DLayout, ARRAYSIZE(imgMultiInsElement));
		LoadPixelShader(shaders::imgMultiInsPx2D_cso, &imgMultiInsPx2DShader);
#endif
		D3D11_INPUT_ELEMENT_DESC shapeInsElement[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"LINECOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"FILLCOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"LINEWIDTH", 0, DXGI_FORMAT_R32_FLOAT, 1, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1}
		};
#ifdef COMPILE 
		LoadCompileVertexShader(COMPILE_SHADER_PATH"shapeInsVert2D.hlsl", &shapeInsVert2DShader, shapeInsElement, &shapeInsVert2DLayout, ARRAYSIZE(shapeInsElement));
		LoadCompileGeometryShader(COMPILE_SHADER_PATH"shapeInsGeo2D.hlsl", &shapeInsGeo2DShader);
		//shapePx2DShader
#else
		LoadVertexShader(shaders::shapeInsVert2D_cso, &shapeInsVert2DShader, shapeInsElement, &shapeInsVert2DLayout, ARRAYSIZE(shapeInsElement));
		LoadGeometryShader(shaders::shapeInsGeo2D_cso, &shapeInsGeo2DShader);
		//shapePx2DShader
#endif

		D3D11_INPUT_ELEMENT_DESC lineInsElement[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"LINECOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"LINEWIDTH", 0, DXGI_FORMAT_R32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1}
		};

#ifdef COMPILE 
		LoadCompileVertexShader(COMPILE_SHADER_PATH"lineInsVert2D.hlsl", &lineInsVert2DShader, lineInsElement, &lineInsVert2DLayout, ARRAYSIZE(lineInsElement));
		LoadCompileGeometryShader(COMPILE_SHADER_PATH"lineInsGeo2D.hlsl", &lineInsGeo2DShader);
		//shapePx2DShader
#else
		LoadVertexShader(shaders::lineInsVert2D_cso, &lineInsVert2DShader, lineInsElement, &lineInsVert2DLayout, ARRAYSIZE(lineInsElement));
		LoadGeometryShader(shaders::lineInsGeo2D_cso, &lineInsGeo2DShader);
		//shapePx2DShader
#endif

		context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
#ifndef COMPILE 
		delete[]shaderData;
#endif
	}
	void Resize() {
		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)_Renderer::windowSize.width;
		viewport.Height = (FLOAT)_Renderer::windowSize.height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		context->RSSetViewports(1, &viewport);

		HRESULT hr = S_OK;
		context->OMSetRenderTargets(0, nullptr, nullptr);

		SafeRelease(&depthStencilView);
		SafeRelease(&renderTargetView);
		SafeRelease(&renderTargetView1);

		SafeRelease(&backBuffer1);
		SafeRelease(&backBuffer);
		
		hr = _DXGI::swapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		
		if (_Renderer::windowSize.width > 0) {
			if (backBuffer1) {
				_DXGI::swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D1), (void**)& backBuffer1);

				if (device3) {
					hr = device3->CreateRenderTargetView1(backBuffer1, nullptr, &renderTargetView1);
					renderTargetView1->QueryInterface(&renderTargetView);
				} else {
					hr = device->CreateRenderTargetView(backBuffer1, nullptr, &renderTargetView);
				}
			} else {
				_DXGI::swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)& backBuffer);
				if (device3) {
					hr = device3->CreateRenderTargetView1(backBuffer, nullptr, &renderTargetView1);
					renderTargetView1->QueryInterface(&renderTargetView);
				} else {
					hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
				}
			}

			D3D11_TEXTURE2D_DESC texture2DDesc;
			texture2DDesc.ArraySize = 1;
			texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			texture2DDesc.CPUAccessFlags = 0;
			texture2DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			texture2DDesc.Height = _Renderer::windowSize.height;
			texture2DDesc.Width = _Renderer::windowSize.width;
			texture2DDesc.MipLevels = 1;
			texture2DDesc.MiscFlags = 0;
			texture2DDesc.SampleDesc.Count = _Renderer::msaaCount;
			texture2DDesc.SampleDesc.Quality = _Renderer::msaaQuality;
			texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
			ID3D11Texture2D* g_depthStencil = nullptr;

			hr = device1->CreateTexture2D(&texture2DDesc, nullptr, &g_depthStencil);

			hr = device1->CreateDepthStencilView(g_depthStencil, 0, &depthStencilView);

			g_depthStencil->Release();
			context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
		}
	}
}

#endif