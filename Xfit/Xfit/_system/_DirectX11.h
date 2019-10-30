#pragma once

#ifdef _WIN32

#include "../system/System.h"
#include "../math/Point3Dw.h"


namespace _System::_DirectX11 {
#pragma pack(push,1)
	struct ImgVertConstantStruct2D {
		Matrix matrix;
		Matrix viewMatrix;
	};
	struct ImgPxConstantStruct2D {
		Matrix colorMatrix;
		float load;
		float sampler;
		float reversed[2];
	};

	struct ShapeVertConstantStruct2D {
		Matrix matrix;
	};
	struct ShapeGeoConstantStruct2D {
		Matrix viewMatrix;
		Point3DwF fillColor;
		Point3DwF lineColor;
		float lineWidth;
		PointF centerPos;
		float reversed;
	};
	struct LineGeoConstantStruct2D {
		Matrix viewMatrix;
		float lineWidth;
		float reversed[3];
	};
	struct LinePxConstantStruct2D {
		Point3DwF color;
	};
	struct ShapeInsVertConstantStruct2D {
		Matrix matrix;
		PointF centerPos;
		float reversed[2];
	};
	struct LineInsGeoConstantStruct2D {
		Matrix matrix;
	};
#pragma pack(pop)

	void Release();

	void Clear(bool _clearDepth);

	void Create();
	void Init(System::CreateInfo* _info);
	void Resize();

	unsigned GetMaxMsaaQuality(unsigned _sampleCount);

	inline ID3D11Device* device = nullptr;
	inline ID3D11Device1* device1 = nullptr;
	inline ID3D11Device2* device2 = nullptr;
	inline ID3D11Device3* device3 = nullptr;
	//inline ID3D11Device4* device4=nullptr;
	//inline ID3D11Device5* device5=nullptr;


	inline ID3D11DeviceContext* context = nullptr;
	inline ID3D11DeviceContext1* context1 = nullptr;
	//inline ID3D11DeviceContext2* context2=nullptr;
	//inline ID3D11DeviceContext3* context3=nullptr;
	//inline ID3D11DeviceContext4* context4=nullptr;

	inline ID3D11RenderTargetView* renderTargetView = nullptr;
	inline ID3D11RenderTargetView1* renderTargetView1 = nullptr;

	inline ID3D11Texture2D* backBuffer = nullptr;
	inline ID3D11Texture2D1* backBuffer1 = nullptr;

	inline ID3D11DepthStencilView* depthStencilView = nullptr;
	
	inline ID3D11RasterizerState* rasterizerState = nullptr;
	inline ID3D11RasterizerState1* rasterizerState1 = nullptr;

	inline ID3D11RasterizerState* rasterizerStateCullingBack = nullptr;
	inline ID3D11RasterizerState1* rasterizerStateCullingBack1 = nullptr;

	inline ID3D11RasterizerState* rasterizerStateCullingFront = nullptr;
	inline ID3D11RasterizerState1* rasterizerStateCullingFront1 = nullptr;

	inline ID3D11DepthStencilState* depthStencilState = nullptr;
	inline ID3D11DepthStencilState* depthStencilState2D = nullptr;




	inline ID3D11VertexShader* imgVert2DShader = nullptr;
	inline ID3D11InputLayout* imgVert2DLayout = nullptr;
	inline ID3D11PixelShader* imgPx2DShader = nullptr;

	inline ID3D11Buffer* imgVertConstantBuffer2D = nullptr;
	inline ID3D11Buffer* imgPxConstantBuffer2D = nullptr;

	inline ID3D11VertexShader* shapeVert2DShader = nullptr;
	inline ID3D11InputLayout* shapeVert2DLayout = nullptr;
	inline ID3D11GeometryShader* shapeGeo2DShader = nullptr;
	inline ID3D11PixelShader* shapePx2DShader = nullptr;

	inline ID3D11Buffer* shapeVertConstantBuffer2D = nullptr;
	inline ID3D11Buffer* shapeGeoConstantBuffer2D = nullptr;
	

	//shapeVert2DShader
	//shapeVert2DLayout
	inline ID3D11GeometryShader* lineGeo2DShader = nullptr;
	inline ID3D11PixelShader* linePx2DShader = nullptr;

	//shapeVertConstantBuffer2D
	inline ID3D11Buffer* lineGeoConstantBuffer2D = nullptr;
	inline ID3D11Buffer* linePxConstantBuffer2D = nullptr;

	inline ID3D11VertexShader* imgInsVert2DShader = nullptr;
	inline ID3D11InputLayout* imgInsVert2DLayout = nullptr;
	//imgPx2DShader

	inline ID3D11VertexShader* imgMultiInsVert2DShader = nullptr;
	inline ID3D11InputLayout* imgMultiInsVert2DLayout = nullptr;
	inline ID3D11PixelShader* imgMultiInsPx2DShader = nullptr;

	inline ID3D11VertexShader* shapeInsVert2DShader = nullptr;
	inline ID3D11InputLayout* shapeInsVert2DLayout = nullptr;
	inline ID3D11GeometryShader* shapeInsGeo2DShader = nullptr;
	//shapePx2DShader

	inline ID3D11Buffer* shapeInsVertConstantBuffer2D = nullptr;
	//shapeVertConstantBuffer2D

	inline ID3D11VertexShader* lineInsVert2DShader = nullptr;
	inline ID3D11InputLayout* lineInsVert2DLayout = nullptr;
	inline ID3D11GeometryShader* lineInsGeo2DShader = nullptr;
	//shapePx2DShader

	//shapeVertConstantBuffer2D
	inline ID3D11Buffer* lineInsGeoConstantBuffer2D = nullptr;

	inline ID3D11ClassLinkage* imgPx2DClassLinkage;
	inline ID3D11ShaderReflection* imgPx2DReflection;
	inline unsigned imgPx2DInterfaceNum;
	inline ID3D11ClassInstance** imgPx2DClassInstances;
	inline ID3D11ClassInstance* imgPx2DloadSample;
	inline ID3D11ClassInstance* imgPx2DsamplerSample;
	
	inline unsigned imgPx2DisampleOffset;

}

#endif