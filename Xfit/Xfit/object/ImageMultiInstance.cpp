#include "ImageMultiInstance.h"
#include "../math/Point3Dw.h"
#include "../resource/Sampler.h"

#include "../resource/FrameArray.h"
#include "../resource/Vertex.h"
#include "../resource/Index.h"

#include "../_system/_GraphicsBase.h"

#include "../_system/_DirectX11.h"
#include "ScaleHeader.h"

#include "../_system/_OpenGL.h"


#ifdef _WIN32

ImageMultiInstance::ImageMultiInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, FrameArray* _frame, Vertex* _vertex, Vertex* _uv, Index* _index) :
	instanceBuffer(nullptr), sampler(_sampler), uv(_uv), vertex(_vertex), frame(_frame), index(_index), SizeMatrixObject(_pos, _scale,_rotation, _blend), num(0), maxNum(0) {
}

using namespace _System::_DirectX11;

ImageMultiInstance::~ImageMultiInstance() {
	if (instanceBuffer)instanceBuffer->Release();
}
void ImageMultiInstance::Draw() {
	if (visible) {
#ifdef _DEBUG
		if (!instanceBuffer)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
		if (!vertex)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullVertex);
		if (!uv)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullUV);
		if (!frame)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullFrame);
		if (!sampler)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullSampler);
		if (!vertex->IsBuild())throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotVertexBuild);
		if (!uv->IsBuild())throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotUVBuild);
		if (!frame->IsBuild())throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotFrameBuild);
		if (vertex->GetNum() != uv->GetNum())throw ImageMultiInstanceError(ImageMultiInstanceError::Code::VertexUVMismatch);
#endif
		Object::Draw();
		contextT->OMSetDepthStencilState(depthStencilState2D, 0xffffffff);

		contextT->VSSetShader(imgMultiInsVert2DShader, nullptr, 0);
		contextT->GSSetShader(nullptr, nullptr, 0);//사용하지 않는 셰이더는 비활성화 해야 됩니다.

		contextT->PSSetShader(imgMultiInsPx2DShader, nullptr, 0);

		ImgVertConstantStruct2D imgMultiInsVert2DContant = { mat, Matrix::GetScale(2.f / (float)System::GetWindowWidth(), 2.f / (float)System::GetWindowHeight()) };

		if (context1) {
			context1T->UpdateSubresource1(imgVertConstantBuffer2D, 0, nullptr, &imgMultiInsVert2DContant, 0, 0, D3D11_COPY_DISCARD);
			context1T->VSSetConstantBuffers1(0, 1, &imgVertConstantBuffer2D, nullptr, nullptr);

			ID3D11Buffer* nullBuffer = nullptr;
			context1T->PSSetConstantBuffers1(0, 1, &nullBuffer, nullptr, nullptr);
		} else {
			contextT->UpdateSubresource(imgVertConstantBuffer2D, 0, nullptr, &imgMultiInsVert2DContant, 0, 0);
			contextT->VSSetConstantBuffers(0, 1, &imgVertConstantBuffer2D);
		}

		ID3D11Buffer* buffers[] = { vertex->vertex, uv->vertex, instanceBuffer };
		UINT stride[] = { sizeof(PointF), sizeof(PointF), sizeof(MultiInstanceNode) };
		UINT offset[] = { 0,0,0 };


		contextT->PSSetSamplers(0, 1, &sampler->sampler);

		contextT->RSSetState(rasterizerState);

		contextT->IASetInputLayout(imgMultiInsVert2DLayout);

		contextT->IASetVertexBuffers(0, 3, buffers, stride, offset);


		contextT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (frame->srv1)contextT->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&frame->srv1);
		else contextT->PSSetShaderResources(0, 1, &frame->srv);


		contextT->IASetIndexBuffer(index->index, DXGI_FORMAT_R32_UINT, 0);

		contextT->DrawIndexedInstanced(index->GetNum(), num, 0, 0, 0);
	}
}

void ImageMultiInstance::BuildInstance() {
#ifdef _DEBUG
	if (nodes.Size() == 0)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullNodes);
	if (instanceBuffer)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::AlreadyBuild);
#endif
	num = nodes.Size();
	maxNum = nodes.MaxSize();

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = sizeof(MultiInstanceNode) * maxNum;

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
void ImageMultiInstance::UpdateInstance(unsigned _start /*= 0*/, unsigned _num /*= UINT_MAX*/) {
#ifdef _DEBUG
	if (!instanceBuffer)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
	if (nodes.Size() == 0)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullNodes);
	if (nodes.Size() > maxNum)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::TooBigNode);
#endif
	num = nodes.Size();
	if (_num > 0) {
		D3D11_BOX box;
		box.left = _start * sizeof(MultiInstanceNode);
		box.right = _num == UINT_MAX ? num * sizeof(MultiInstanceNode) : (_start + _num) * sizeof(MultiInstanceNode);
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
void ImageMultiInstance::Delete() {
#ifdef _DEBUG
	if (!instanceBuffer)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
#endif
	instanceBuffer->Release();
	instanceBuffer = nullptr;
}
unsigned ImageMultiInstance::GetNum()const {
#ifdef _DEBUG
	if (!instanceBuffer)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
#endif
	return num;
}
unsigned ImageMultiInstance::GetMaxNum()const {
#ifdef _DEBUG
	if (!instanceBuffer)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
#endif
	return maxNum;
}
bool ImageMultiInstance::IsBuild()const {
	return (bool)instanceBuffer;
}

#elif __ANDROID__

#include "../_system/_Android.h"

using namespace _System::_OpenGL;

ImageMultiInstance::ImageMultiInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, FrameArray* _frame, Vertex* _vertex, Vertex* _uv, Index* _index) :
ins(0), sampler(_sampler), uv(_uv), vertex(_vertex), frame(_frame),index(_index), SizeMatrixObject(_pos, _scale,_rotation, _blend), num(0), maxNum(0) {
    Size();
}

ImageMultiInstance::~ImageMultiInstance() {
    if (ins) {
        _System::_Android::Lock();
        glDeleteBuffers(1, &ins);
        _System::_Android::Unlock();
    }
}
void ImageMultiInstance::Draw() {
	if (visible) {
#ifdef _DEBUG
		if (!ins)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
		if (!vertex)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullVertex);
		if (!uv)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullUV);
		if (!frame)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullFrame);
		if (!sampler)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullSampler);
		if (!vertex->IsBuild())throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotVertexBuild);
		if (!uv->IsBuild())throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotUVBuild);
		if (!frame->IsBuild())throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotFrameBuild);
		if (vertex->GetNum() != uv->GetNum())throw ImageMultiInstanceError(ImageMultiInstanceError::Code::VertexUVMismatch);
#endif
        Object::Draw();
//		if(glUseProgramStages) {
//			glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgInsVertProg);
//			glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgFragProg);
//		}else{
        glUseProgram(imgMultiInsProg);

        glUniformMatrix4fv(imgMultiIns::matUniform, 1, GL_FALSE, mat.e);
        glUniformMatrix4fv(imgMultiIns::viewMatUniform, 1, GL_FALSE, Matrix::GetScale(2.f / (float)System::GetWindowWidth(), 2.f / (float)System::GetWindowHeight()).e);
        //}

        glBindBuffer(GL_ARRAY_BUFFER, vertex->vertex);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, uv->vertex);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, ins);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), 0);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)16);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)24);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)48);

        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)64);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)80);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)96);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)112);
        glVertexAttribIPointer(10, 1, GL_INT, sizeof(MultiInstanceNode), (void*)128);

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);
        glVertexAttribDivisor(10, 1);

        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);
        glEnableVertexAttribArray(7);
        glEnableVertexAttribArray(8);
        glEnableVertexAttribArray(9);
        glEnableVertexAttribArray(10);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, frame->texture);

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
        glDisableVertexAttribArray(10);
	}
}

void ImageMultiInstance::BuildInstance() {
#ifdef _DEBUG
	if (nodes.Size() == 0)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullNodes);
	if (ins)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::AlreadyBuild);
#endif
    num = nodes.Size();
    maxNum = nodes.MaxSize();

    _System::_Android::Lock();
    glGenBuffers(1, &ins);
    glBindBuffer(GL_ARRAY_BUFFER, ins);

    if (num == maxNum) {
        //glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(ImageInstanceNode), nodes->GetData(), GL_DYNAMIC_STORAGE_BIT);
        glBufferData(GL_ARRAY_BUFFER, maxNum * sizeof(MultiInstanceNode), nodes.GetData(), GL_DYNAMIC_DRAW);
    } else {
        //glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(ImageInstanceNode), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glBufferData(GL_ARRAY_BUFFER, maxNum * sizeof(MultiInstanceNode), nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(MultiInstanceNode), nodes.GetData());
    }
    _System::_Android::Unlock();
}
void ImageMultiInstance::UpdateInstance(unsigned _start /*= 0*/, unsigned _num /*= UINT_MAX*/) {
#ifdef _DEBUG
	if (!ins)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
	if (nodes.Size() == 0)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NullNodes);
	if (nodes.Size() > maxNum)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::TooBigNode);
#endif
    num = nodes.Size();
    _System::_Android::Lock();
    glBindBuffer(GL_ARRAY_BUFFER, ins);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(MultiInstanceNode), nodes.GetData());
    _System::_Android::Unlock();
}
void ImageMultiInstance::Delete() {
#ifdef _DEBUG
	if (!ins)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
#endif
    _System::_Android::Lock();
    glDeleteBuffers(1, &ins);
    _System::_Android::Unlock();
    ins = 0;
}
unsigned ImageMultiInstance::GetNum()const {
#ifdef _DEBUG
	if (!ins)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
#endif
	return num;
}
unsigned ImageMultiInstance::GetMaxNum()const {
#ifdef _DEBUG
	if (!ins)throw ImageMultiInstanceError(ImageMultiInstanceError::Code::NotBuild);
#endif
	return maxNum;
}
bool ImageMultiInstance::IsBuild()const {
	return (bool)ins;
}
#endif