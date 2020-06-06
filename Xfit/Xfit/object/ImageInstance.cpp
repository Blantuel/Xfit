#include "ImageInstance.h"
#include "../math/Point3Dw.h"
#include "../resource/Sampler.h"

#include "../resource/Frame.h"
#include "../resource/Vertex.h"

#include "../_system/_GraphicsBase.h"

#include "../_system/_DirectX11.h"
#include "../system/System.h"

#include "../data/Compress.h"

#include "ScaleHeader.h"

#include "../resource/Index.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"

#ifdef _WIN32

using namespace _System::_DirectX11;

ImageInstance::ImageInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, Frame* _frame, Vertex* _vertex, Vertex* _uv, Index* _index):
	instanceBuffer(nullptr),sampler(_sampler),uv(_uv),vertex(_vertex),frame(_frame), basePos(_pos), baseScale(_scale), index(_index), SizeMatrixObject(_pos, _scale,
		_rotation, _blend), num(0), maxNum(0) {
}
ImageInstance::~ImageInstance() {
	if (instanceBuffer)instanceBuffer->Release();
}
void ImageInstance::Draw() {
	if(visible) {
#ifdef _DEBUG
		if (!instanceBuffer)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
		if(!vertex)throw ImageInstanceError(ImageInstanceError::Code::NullVertex);
		if(!uv)throw ImageInstanceError(ImageInstanceError::Code::NullUV);
		if(!frame)throw ImageInstanceError(ImageInstanceError::Code::NullFrame);
		if(!sampler)throw ImageInstanceError(ImageInstanceError::Code::NullSampler);
		if(!vertex->IsBuild())throw ImageInstanceError(ImageInstanceError::Code::NotVertexBuild);
		if(!uv->IsBuild())throw ImageInstanceError(ImageInstanceError::Code::NotUVBuild);
		if(!frame->IsBuild())throw ImageInstanceError(ImageInstanceError::Code::NotFrameBuild);
		if(vertex->GetNum() != uv->GetNum())throw ImageInstanceError(ImageInstanceError::Code::VertexUVMismatch);
#endif
		Object::Draw();
		contextT->OMSetDepthStencilState(depthStencilState2D, 0xffffffff);

		contextT->VSSetShader(imgInsVert2DShader, nullptr, 0);
		contextT->GSSetShader(nullptr, nullptr, 0);//사용하지 않는 셰이더는 비활성화 해야 됩니다.

		contextT->PSSetShader(imgInsPx2DShader, nullptr, 0);

		ImgVertConstantStruct2D imgInsVert2DContant = { mat, Matrix::GetScale(2.f / (float)System::GetWindowWidth(), 2.f / (float)System::GetWindowHeight()) };

		if (context1) {
			context1T->UpdateSubresource1(imgVertConstantBuffer2D, 0, nullptr, &imgInsVert2DContant, 0, 0, D3D11_COPY_DISCARD);
			context1T->VSSetConstantBuffers1(0, 1, &imgVertConstantBuffer2D, nullptr, nullptr);

			ID3D11Buffer* nullBuffer = nullptr;
			context1T->PSSetConstantBuffers1(0, 1, &nullBuffer, nullptr, nullptr);
		} else {
			contextT->UpdateSubresource(imgVertConstantBuffer2D, 0, nullptr, &imgInsVert2DContant, 0, 0);
			contextT->VSSetConstantBuffers(0, 1, &imgVertConstantBuffer2D);
		}

		ID3D11Buffer* buffers[] = { vertex->vertex, uv->vertex, instanceBuffer };
		UINT stride[] = { sizeof(PointF), sizeof(PointF), sizeof(ImageInstanceNode) };
		UINT offset[] = { 0,0,0 };


		contextT->PSSetSamplers(0, 1, &sampler->sampler);

		contextT->RSSetState(rasterizerState);

		contextT->IASetInputLayout(imgInsVert2DLayout);

		contextT->IASetVertexBuffers(0, 3, buffers, stride, offset);


		contextT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (frame->srv1)contextT->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&frame->srv1);
		else contextT->PSSetShaderResources(0, 1, &frame->srv);


		contextT->IASetIndexBuffer(index->index, DXGI_FORMAT_R32_UINT, 0);

		contextT->DrawIndexedInstanced(index->GetNum(), num, 0, 0, 0);
	}
}

void ImageInstance::BuildInstance() {
#ifdef _DEBUG
	if(nodes.Size() == 0)throw ImageInstanceError(ImageInstanceError::Code::NullNodes);
	if(instanceBuffer)throw ImageInstanceError(ImageInstanceError::Code::AlreadyBuild);
#endif
	num = nodes.Size();
	maxNum = nodes.MaxSize();

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = sizeof(ImageInstanceNode) * maxNum;

	if (num == maxNum) {
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
void ImageInstance::UpdateInstance(unsigned _start /*= 0*/, unsigned _num /*= UINT_MAX*/) {
#ifdef _DEBUG
	if(!instanceBuffer)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
	if(nodes.Size() == 0)throw ImageInstanceError(ImageInstanceError::Code::NullNodes);
	if (nodes.Size() > maxNum)throw ImageInstanceError(ImageInstanceError::Code::TooBigNode);
#endif
	num = nodes.Size();
	if (_num > 0) {
		D3D11_BOX box;
		box.left = _start * sizeof(ImageInstanceNode);
		box.right = _num == UINT_MAX ? num * sizeof(ImageInstanceNode) : (_start + _num) * sizeof(ImageInstanceNode);
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;

		if (context1T) {
			context1T->UpdateSubresource1(instanceBuffer, 0, &box, &nodes[_start], 0, 0,
				(_num == UINT_MAX || _num == num) && _start == 0 ? D3D11_COPY_DISCARD : 0);
		} else {
			contextT->UpdateSubresource(instanceBuffer, 0, &box, &nodes[_start], 0, 0);
		}
	}
}
void ImageInstance::Delete() {
#ifdef _DEBUG
	if(!instanceBuffer)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
	instanceBuffer->Release();
	instanceBuffer = nullptr;
}
unsigned ImageInstance::GetNum()const {
#ifdef _DEBUG
	if(!instanceBuffer)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
	return num;
}
unsigned ImageInstance::GetMaxNum()const {
#ifdef _DEBUG
	if(!instanceBuffer)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
	return maxNum;
}
bool ImageInstance::IsBuild()const {
	return (bool)instanceBuffer;
}

#elif __ANDROID__

#include "../_system/_Android.h"

using namespace _System::_OpenGL;

ImageInstance::ImageInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, Frame* _frame, Vertex* _vertex, Vertex* _uv, Index* _index):ins(0),
sampler(_sampler),uv(_uv),vertex(_vertex),frame(_frame),index(_index), SizeMatrixObject(_pos, _scale, _rotation, _blend), num(0), maxNum(0)  {
}

ImageInstance::~ImageInstance() {
	if (ins) {
        _System::_Android::Lock();
	    glDeleteBuffers(1, &ins);
        _System::_Android::Unlock();
	}
}
void ImageInstance::Draw() {
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

		glBindBuffer(GL_ARRAY_BUFFER, uv->vertex);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, ins);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ImageInstanceNode), 0);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ImageInstanceNode), (void*)16);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(ImageInstanceNode), (void*)24);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(ImageInstanceNode), (void*)48);

		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(ImageInstanceNode), (void*)64);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(ImageInstanceNode), (void*)80);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(ImageInstanceNode), (void*)96);
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(ImageInstanceNode), (void*)112);

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frame->texture);

		glBindSampler(0, sampler->sampler);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index->index);

		glDrawElementsInstanced(GL_TRIANGLES, index->GetNum(), GL_UNSIGNED_INT, nullptr ,(GLsizei)num);

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

void ImageInstance::BuildInstance() {
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
		//glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(ImageInstanceNode), nodes->GetData(), GL_DYNAMIC_STORAGE_BIT);
		glBufferData(GL_ARRAY_BUFFER, maxNum * sizeof(ImageInstanceNode), nodes.GetData(), GL_DYNAMIC_DRAW);
	} else {
		//glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(ImageInstanceNode), nullptr, GL_DYNAMIC_STORAGE_BIT);
		glBufferData(GL_ARRAY_BUFFER, maxNum * sizeof(ImageInstanceNode), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(ImageInstanceNode), nodes.GetData());
	}
    _System::_Android::Unlock();
}
void ImageInstance::UpdateInstance(unsigned _start /*= 0*/, unsigned _num /*= UINT_MAX*/) {
#ifdef _DEBUG
	if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
	if(!nodes)throw ImageInstanceError(ImageInstanceError::Code::NullNodes);
	if (nodes->Size() > insMaxLen)throw ImageInstanceError(ImageInstanceError::Code::TooBigNode);
#endif
	num = nodes.Size();
    _System::_Android::Lock();
	glBindBuffer(GL_ARRAY_BUFFER, ins);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(ImageInstanceNode), nodes.GetData());
    _System::_Android::Unlock();
}
void ImageInstance::Delete() {
#ifdef _DEBUG
	if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
    _System::_Android::Lock();
	glDeleteBuffers(1, &ins);
    _System::_Android::Unlock();
	ins=0;
}
unsigned ImageInstance::GetNum()const {
#ifdef _DEBUG
	if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
	return num;
}
unsigned ImageInstance::GetMaxNum()const {
#ifdef _DEBUG
	if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
	return maxNum;
}
bool ImageInstance::IsBuild()const {
	return (bool)ins;
}

#endif