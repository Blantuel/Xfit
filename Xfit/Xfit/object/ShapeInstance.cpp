#include "ShapeInstance.h"
#include "../resource/ShapeVertex.h"
#include "ScaleHeader.h"

#ifdef _WIN32

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
		context1->VSSetConstantBuffers1(0, 1, &shapeInsVertConstantBuffer2D, nullptr, nullptr);//�⺻�� nullptr, nullptr

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
	SizeMatrixObject(_pos,_scale,_rotation,_blend),num(0),maxNum(0),instanceBuffer(nullptr),vertex(_vertex) {}

ShapeInstance::~ShapeInstance() {
	if (instanceBuffer)instanceBuffer->Release();
}

bool ShapeInstance::IsBuild()const {
    return (bool)instanceBuffer;
}

#elif __ANDROID__

/*#include "../_system/_Android.h"
#include "../_system/_OpenGL.h"

using namespace _System::_OpenGL;

ShapeInstance::ShapeInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, ShapeVertex* _vertex):ins(0),
vertex(_vertex), basePos(_pos), baseScale(_scale), MatrixObject(_pos, _scale, _rotation, _blend), num(0), maxNum(0)  {
    Size();
}

void ShapeInstance::Size() {
    pos = basePos * WindowRatio();

    scale = baseScale * WindowRatio();
    UpdateMatrix();
}

ShapeInstance::~ShapeInstance() {
    if (ins) {
        _System::_Android::Lock();
        glDeleteBuffers(1, &ins);
        _System::_Android::Unlock();
    }
}
void ShapeInstance::Draw() {
    if(visible) {
#ifdef _DEBUG
        if (!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
		if(!vertex)throw ImageInstanceError(ImageInstanceError::Code::NullVertex);
		if(!uv)throw ImageInstanceError(ImageInstanceError::Code::NullUV);
		if(!frame)throw ImageInstanceError(ImageInstanceError::Code::NullFrame);
		if(!sampler)throw ImageInstanceError(ImageInstanceError::Code::NullSampler);
		if(!vertex->IsBuild())throw ImageInstanceError(ImageInstanceError::Code::NotVertexBuild);
		if(!uv->IsBuild())throw ImageInstanceError(ImageInstanceError::Code::NotUVBuild);
		if(!frame->IsBuild())throw ImageInstanceError(ImageInstanceError::Code::NotFrameBuild);
		if(vertex->GetNum()!=uv->GetNum())throw ImageInstanceError(ImageInstanceError::Code::VertexUVMismatch);
#endif
        Object::Draw();
//		if(glUseProgramStages) {
//			glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgInsVertProg);
//			glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgFragProg);
//		}else{
        glUseProgram(imgInsProg);

        glUniformMatrix4fv(imgIns::matUniform, 1, GL_FALSE, mat.e);
        glUniformMatrix4fv(imgIns::viewMatUniform, 1, GL_FALSE, Matrix::GetScale(2.f / (float)System::GetWindowWidth(), 2.f / (float)System::GetWindowHeight()).e);
        //}

        glBindBuffer(GL_ARRAY_BUFFER, vertex->vertex);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, ins);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ShapeInstanceNode), 0);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ShapeInstanceNode), (void*)16);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(ShapeInstanceNode), (void*)24);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(ShapeInstanceNode), (void*)48);

        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(ShapeInstanceNode), (void*)64);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(ShapeInstanceNode), (void*)80);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(ShapeInstanceNode), (void*)96);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(ShapeInstanceNode), (void*)112);

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);

        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);
        glEnableVertexAttribArray(7);
        glEnableVertexAttribArray(8);
        glEnableVertexAttribArray(9);

        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, vertex->GetNum(), (GLsizei)num);

        glDrawArraysInstanced(GL_LINE_STRIP, 0, vertex->GetNum(), (GLsizei)num);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
        glDisableVertexAttribArray(5);
        glDisableVertexAttribArray(6);
        glDisableVertexAttribArray(7);
        glDisableVertexAttribArray(8);
        glDisableVertexAttribArray(9);
    }
}

void ShapeInstance::BuildInstance() {
#ifdef _DEBUG
    if(!nodes)throw ImageInstanceError(ImageInstanceError::Code::NullNodes);
	if(ins)throw ImageInstanceError(ImageInstanceError::Code::AlreadyBuild);
#endif
    num = nodes.Size();
    maxNum = nodes.MaxSize();

    _System::_Android::Lock();
    glGenBuffers(1, &ins);
    glBindBuffer(GL_ARRAY_BUFFER, ins);
    if (num == maxNum) {
        //glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(ShapeInstanceNode), nodes->GetData(), GL_DYNAMIC_STORAGE_BIT);
        glBufferData(GL_ARRAY_BUFFER, maxNum * sizeof(ShapeInstanceNode), nodes.GetData(), GL_DYNAMIC_DRAW);
    } else {
        //glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(ShapeInstanceNode), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glBufferData(GL_ARRAY_BUFFER, maxNum * sizeof(ShapeInstanceNode), nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(ShapeInstanceNode), nodes.GetData());
    }
    _System::_Android::Unlock();
}
void ShapeInstance::UpdateInstance(unsigned _start *//*= 0*//*, unsigned _num *//*= UINT_MAX*//*) {
#ifdef _DEBUG
    if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
	if(!nodes)throw ImageInstanceError(ImageInstanceError::Code::NullNodes);
	if (nodes->Size() > insMaxLen)throw ImageInstanceError(ImageInstanceError::Code::TooBigNode);
#endif
    num = nodes.Size();
    _System::_Android::Lock();
    glBindBuffer(GL_ARRAY_BUFFER, ins);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(ShapeInstanceNode), nodes.GetData());
    _System::_Android::Unlock();
}
void ShapeInstance::Delete() {
#ifdef _DEBUG
    if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
    _System::_Android::Lock();
    glDeleteBuffers(1, &ins);
    _System::_Android::Unlock();
    ins=0;
}
unsigned ShapeInstance::GetNum()const {
#ifdef _DEBUG
    if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
    return num;
}
unsigned ShapeInstance::GetMaxNum()const {
#ifdef _DEBUG
    if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
    return maxNum;
}
bool ShapeInstance::IsBuild()const {
    return (bool)ins;
}

void ShapeInstance::SetPos(PointF _pos) {
    basePos = _pos;
    Size();
}
void ShapeInstance::SetX(float _x) {
    basePos.x = _x;
    Size();
}
void ShapeInstance::SetY(float _y) {
    basePos.y = _y;
    Size();
}
void ShapeInstance::SetScale(PointF _scale) {
    baseScale = _scale;
    Size();
}
void ShapeInstance::SetScaleX(float _scaleX) {
    baseScale.x = _scaleX;
    Size();
}
void ShapeInstance::SetScaleY(float _scaleY) {
    baseScale.y = _scaleY;
    Size();
}
void ShapeInstance::SetRotation(float _rotation) {
    rotation = _rotation;
    Size();
}
void ShapeInstance::SetTransform(PointF _pos, PointF _scale, float _rotation) {
    basePos = _pos;
    baseScale = _scale;
    rotation = _rotation;
    Size();
}*/

#endif