#include "ShapeInstance.h"
#include "../resource/ShapeVertex.h"

#include "../_system/_DirectX11.h"

using namespace _System::_DirectX11;

void ShapeInstance::BuildInstance() {
	num = nodes.Size();
	maxNum = nodes.MaxSize();

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = sizeof(ShapeInstanceNode) * maxNum;

	if (num == 0) {
		device->CreateBuffer(&bufferDesc, nullptr, &instanceBuffer);
	} else if(num==maxNum) {
		D3D11_SUBRESOURCE_DATA subSourceData;
		subSourceData.pSysMem = nodes.GetData();
		subSourceData.SysMemPitch = 0;
		subSourceData.SysMemSlicePitch = 0;

		device->CreateBuffer(&bufferDesc, &subSourceData, &instanceBuffer);
	} else {
		device->CreateBuffer(&bufferDesc, nullptr, &instanceBuffer);
		UpdateInstance();
	}
}
void ShapeInstance::UpdateInstance(unsigned _start/* = 0*/, unsigned _num/* = UINT_MAX*/) {
	num = nodes.Size();
	if (_num > 0) {
		D3D11_BOX box;
		box.left = _start * sizeof(ShapeInstanceNode);
		box.right = _num == UINT_MAX ? num * sizeof(ShapeInstanceNode) : (_start + _num) * sizeof(ShapeInstanceNode);
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;

		if (context1) {
			context1->UpdateSubresource1(instanceBuffer, 0, &box, &nodes[_start], 0, 0,
				(_num == UINT_MAX || _num == num) && _start == 0 ? D3D11_COPY_DISCARD : 0);
		} else {
			context->UpdateSubresource(instanceBuffer, 0, &box, &nodes[_start], 0, 0);
		}
	}
}
unsigned ShapeInstance::GetNum()const {
	return num;
}
unsigned ShapeInstance::GetMaxNum()const {
	return maxNum;
}
void ShapeInstance::Delete() {
	instanceBuffer->Release();
	instanceBuffer = nullptr;
	num = 0;
	maxNum = 0;
}

void ShapeInstance::Draw() {
	if (!visible)return;
	Object::Draw();

	_System::_DirectX11::context->OMSetDepthStencilState(depthStencilState2D, 0xffffffff);

	_System::_DirectX11::context->VSSetShader(shapeInsVert2DShader, nullptr, 0);
	_System::_DirectX11::context->GSSetShader(shapeInsGeo2DShader, nullptr, 0);

	_System::_DirectX11::context->PSSetShader(shapePx2DShader, nullptr, 0);

	ShapeInsVertConstantStruct2D shapeInsVert2DContant = { mat, vertex->GetCenterPos() };
	ShapeVertConstantStruct2D shapeInsGeo2DContant = { Matrix::GetScale(2.f / (float)System::GetWindowWidth(), 2.f / (float)System::GetWindowHeight()) };

	if (context1) {
		context1->UpdateSubresource1(shapeInsVertConstantBuffer2D, 0, nullptr, &shapeInsVert2DContant, 0, 0, D3D11_COPY_DISCARD);
		context1->VSSetConstantBuffers1(0, 1, &shapeInsVertConstantBuffer2D, nullptr, nullptr);//±âº»°ª nullptr, nullptr

		context1->UpdateSubresource1(shapeVertConstantBuffer2D, 0, nullptr, &shapeInsGeo2DContant, 0, 0, D3D11_COPY_DISCARD);
		context1->GSSetConstantBuffers1(0, 1, &shapeVertConstantBuffer2D, nullptr, nullptr);
	} else {
		_System::_DirectX11::context->UpdateSubresource(shapeInsVertConstantBuffer2D, 0, nullptr, &shapeInsVert2DContant, 0, 0);
		_System::_DirectX11::context->VSSetConstantBuffers(0, 1, &shapeInsVertConstantBuffer2D);

		_System::_DirectX11::context->UpdateSubresource(shapeVertConstantBuffer2D, 0, nullptr, &shapeInsGeo2DContant, 0, 0);
		_System::_DirectX11::context->GSSetConstantBuffers(0, 1, &shapeVertConstantBuffer2D);
	}

	ID3D11Buffer* buffers[] = { vertex->vertex, instanceBuffer };
	UINT stride[] = { sizeof(PointF), sizeof(ShapeInstanceNode) };
	UINT offset[] = { 0,0 };

	_System::_DirectX11::context->RSSetState(rasterizerState);

	_System::_DirectX11::context->IASetInputLayout(shapeInsVert2DLayout);

	_System::_DirectX11::context->IASetVertexBuffers(0, 2, buffers, stride, offset);


	_System::_DirectX11::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	_System::_DirectX11::context->DrawInstanced(vertex->GetNum() + 1, num, 0, 0);
}

ShapeInstance::ShapeInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, ShapeVertex* _vertex):
	MatrixObject(_pos,_scale,_rotation,_blend),num(0),maxNum(0),instanceBuffer(nullptr),vertex(_vertex) {}
ShapeInstance::ShapeInstance():num(0), maxNum(0), instanceBuffer(nullptr), vertex(nullptr) {}
ShapeInstance::~ShapeInstance() {
	if (instanceBuffer)instanceBuffer->Release();
}