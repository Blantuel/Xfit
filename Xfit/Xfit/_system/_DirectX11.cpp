#ifdef _WIN32

#include "_DirectX11.h"
#include "_DXGI.h"
#include "_Windows.h"
#include "_Renderer.h"

#include "..//file/File.h"


#define SHADER_PATH "../Xfit/_shader/out/"

#define RELEASE_SHADER_PATH "shaders/"

namespace _System::_DirectX11 {
	static void LoadVertexShaderDebug(const char* _path, ID3D11VertexShader** _shader, D3D11_INPUT_ELEMENT_DESC* _inputElement, ID3D11InputLayout** _outLayout, unsigned _elementSize, 
		ID3D11ClassLinkage** _outClassLinkage = nullptr, ID3D11ShaderReflection** _outReflection = nullptr) {
		File file(_path);
		const unsigned size = file.GetSize();
		unsigned char* data = new unsigned char[size];
		file.ReadBytes(size, data);

		HRESULT hr;
		if (_outClassLinkage) {
			hr = device->CreateClassLinkage(_outClassLinkage);
			hr = device->CreateVertexShader(data, size, *_outClassLinkage, _shader);
			hr = D3DReflect(data, size, __uuidof(ID3D11ShaderReflection), (void**)_outReflection);
		} else {
			hr = device->CreateVertexShader(data, size, nullptr, _shader);
		}

		hr = device1->CreateInputLayout(_inputElement, _elementSize, data, size, _outLayout);
		delete[]data;
		if (FAILED(hr));
	}
	static void LoadPixelShaderDebug(const char* _path, ID3D11PixelShader** _shader, ID3D11ClassLinkage** _outClassLinkage = nullptr, ID3D11ShaderReflection** _outReflection = nullptr) {
		File file(_path);
		const unsigned size = file.GetSize();
		unsigned char* data = new unsigned char[size];
		file.ReadBytes(size, data);

		HRESULT hr;
		if (_outClassLinkage) {
			hr = device->CreateClassLinkage(_outClassLinkage);
			hr = device->CreatePixelShader(data, size, *_outClassLinkage, _shader);
			hr = D3DReflect(data, size, __uuidof(ID3D11ShaderReflection), (void**)_outReflection);
		} else {
			hr = device->CreatePixelShader(data, size, nullptr , _shader);
		}

		delete[]data;
		if (FAILED(hr));
	}
	static void LoadGeometryShaderDebug(const char* _path, ID3D11GeometryShader** _shader, ID3D11ClassLinkage** _outClassLinkage = nullptr, ID3D11ShaderReflection** _outReflection = nullptr) {
		File file(_path);
		const unsigned size = file.GetSize();
		unsigned char* data = new unsigned char[size];
		file.ReadBytes(size, data);

		HRESULT hr;
		if (_outClassLinkage) {
			hr = device->CreateClassLinkage(_outClassLinkage);
			hr = device->CreateGeometryShader(data, size, *_outClassLinkage, _shader);
			hr = D3DReflect(data, size, __uuidof(ID3D11ShaderReflection), (void**)_outReflection);
		} else {
			hr = device->CreateGeometryShader(data, size, nullptr, _shader);
		}

		delete[]data;
		if (FAILED(hr));
	}
	void Clear(bool _clearDepth) {
		context->ClearRenderTargetView(renderTargetView, _Renderer::backColor);
		if (_clearDepth)context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0xff);
	}
	void Release() {

	}
	void Create() {
		HRESULT hr;

#ifdef _DEBUG
		constexpr UINT flag = D3D11_CREATE_DEVICE_DEBUG;
#else
		constexpr UINT flag = 0;
#endif
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0 };//DirectX 버전이 11.0이상인지 확인

		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag, featureLevels, 2, D3D11_SDK_VERSION, &device, nullptr, &context);

		if (FAILED(hr));//DirectX 버전이 11.0미만이면 오류보고

		device->QueryInterface(&device1);//실패시 device1이 nullptr이됨.
		if (device1)device->QueryInterface(&device2);
		if (device2)device->QueryInterface(&device3);

		context->QueryInterface(&context1);
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


		context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
		/*
		LPCSTR SemanticName;
		UINT SemanticIndex;
		DXGI_FORMAT Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
		*/

		D3D11_INPUT_ELEMENT_DESC imgElement[] = { 
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
#ifdef _DEBUG
		LoadVertexShaderDebug(SHADER_PATH"imgVert2D.cso", &imgVert2DShader, imgElement, &imgVert2DLayout, ARRAYSIZE(imgElement));
		LoadPixelShaderDebug(SHADER_PATH"imgPx2D.cso",&imgPx2DShader,&imgPx2DClassLinkage,&imgPx2DReflection);
#else
		LoadVertexShaderDebug(RELEASE_SHADER_PATH"imgVert2D.cso", &imgVert2DShader, imgElement, &imgVert2DLayout, ARRAYSIZE(imgElement));
		LoadPixelShaderDebug(RELEASE_SHADER_PATH"imgPx2D.cso", &imgPx2DShader, &imgPx2DClassLinkage, &imgPx2DReflection);
#endif
		imgPx2DInterfaceNum = imgPx2DReflection->GetNumInterfaceSlots();;
		imgPx2DClassInstances = new ID3D11ClassInstance * [imgPx2DInterfaceNum];
		Memory::Set(imgPx2DClassInstances, (ID3D11ClassInstance *)0, imgPx2DInterfaceNum);

		ID3D11ShaderReflectionVariable* imgPx2DisampleVar = imgPx2DReflection->GetVariableByName("isample");
		imgPx2DisampleOffset = imgPx2DisampleVar->GetInterfaceSlot(0);

		imgPx2DClassLinkage->GetClassInstance("loadSample", 0, &imgPx2DloadSample);
		imgPx2DClassLinkage->GetClassInstance("samplerSample", 0, &imgPx2DsamplerSample);


		D3D11_INPUT_ELEMENT_DESC shapeElement[]{
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
#ifdef _DEBUG
		LoadVertexShaderDebug(SHADER_PATH"shapeVert2D.cso", &shapeVert2DShader, shapeElement, &shapeVert2DLayout, ARRAYSIZE(shapeElement));
		LoadGeometryShaderDebug(SHADER_PATH"shapeGeo2D.cso", &shapeGeo2DShader);
		LoadPixelShaderDebug(SHADER_PATH"shapePx2D.cso", &shapePx2DShader);
#else
		LoadVertexShaderDebug(RELEASE_SHADER_PATH"shapeVert2D.cso", &shapeVert2DShader, shapeElement, &shapeVert2DLayout, ARRAYSIZE(shapeElement));
		LoadGeometryShaderDebug(RELEASE_SHADER_PATH"shapeGeo2D.cso", &shapeGeo2DShader);
		LoadPixelShaderDebug(RELEASE_SHADER_PATH"shapePx2D.cso", &shapePx2DShader);
#endif

#ifdef _DEBUG
		//shapeVert2D
		LoadGeometryShaderDebug(SHADER_PATH"lineGeo2D.cso", &lineGeo2DShader);
		LoadPixelShaderDebug(SHADER_PATH"linePx2D.cso", &linePx2DShader);
#else
#endif

		D3D11_INPUT_ELEMENT_DESC imgInsElement[] = {
			{"Pos", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"InsMat", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"InsMat", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"InsMat", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"InsMat", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1}
		};
#ifdef _DEBUG
		//LoadVertexShaderDebug(SHADER_PATH"imgInsVert2D.cso", &imgInsVert2DShader, imgInsElement, &imgInsVert2DLayout, ARRAYSIZE(imgInsElement));
		//imgPx2DShader
#else
#endif

		D3D11_INPUT_ELEMENT_DESC imgMultiInsElement[] = {
			{"Pos", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"InsMat", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"InsMat", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"InsMat", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"InsMat", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"ImgId", 0, DXGI_FORMAT_R32G32B32A32_UINT, 2, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1}//첫번째 요소(x)만 사용
		};
#ifdef _DEBUG
		//LoadVertexShaderDebug(SHADER_PATH"imgMultiInsVert2D.cso", &imgMultiInsVert2DShader, imgMultiInsElement, &imgMultiInsVert2DLayout, ARRAYSIZE(imgMultiInsElement));
		//LoadPixelShaderDebug(SHADER_PATH"imgMultiInsPx2D.cso", &imgMultiInsPx2DShader);
#else
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
#ifdef _DEBUG
		LoadVertexShaderDebug(SHADER_PATH"shapeInsVert2D.cso", &shapeInsVert2DShader, shapeInsElement, &shapeInsVert2DLayout, ARRAYSIZE(shapeInsElement));
		LoadGeometryShaderDebug(SHADER_PATH"shapeInsGeo2D.cso", &shapeInsGeo2DShader);
		//shapePx2DShader
#else
		LoadVertexShaderDebug(RELEASE_SHADER_PATH"shapeInsVert2D.cso", &shapeInsVert2DShader, shapeInsElement, &shapeInsVert2DLayout, ARRAYSIZE(shapeInsElement));
		LoadGeometryShaderDebug(RELEASE_SHADER_PATH"shapeInsGeo2D.cso", &shapeInsGeo2DShader);
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
#ifdef _DEBUG
		LoadVertexShaderDebug(SHADER_PATH"lineInsVert2D.cso", &lineInsVert2DShader, lineInsElement, &lineInsVert2DLayout, ARRAYSIZE(lineInsElement));
		LoadGeometryShaderDebug(SHADER_PATH"lineInsGeo2D.cso", &lineInsGeo2DShader);
		//shapePx2DShader
#else
		LoadVertexShaderDebug(RELEASE_SHADER_PATH"lineInsVert2D.cso", &lineInsVert2DShader, lineInsElement, &lineInsVert2DLayout, ARRAYSIZE(lineInsElement));
		LoadGeometryShaderDebug(RELEASE_SHADER_PATH"lineInsGeo2D.cso", &lineInsGeo2DShader);
#endif

		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)_Renderer::windowSize.width;
		viewport.Height = (FLOAT)_Renderer::windowSize.height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		context->RSSetViewports(1, &viewport);
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
			context1->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
		}
	}
}

#endif