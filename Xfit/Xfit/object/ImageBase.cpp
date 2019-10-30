#include "ImageBase.h"
#include "../resource/Sampler.h"

#include "../_system/_GraphicsBase.h"

#include "../_system/_Windows.h"

#include "../resource/Frame.h"
#include "../resource/Vertex.h"
#include "../resource/Index.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"
#include "../_system/_DirectX11.h"

#ifdef _WIN32
using namespace _System::_DirectX11;
#elif __ANDROID__
using namespace _System::_OpenGL;
#endif


ImageBase::ImageBase():sampler(System::defaultSampler) { colorMat.Identity(); }
ImageBase::ImageBase(PointF _pos,PointF _scale,float _rotation,Blend* _blend,Sampler* _sampler):MatrixObject(_pos,_scale,_rotation,_blend),sampler(_sampler) { colorMat.Identity(); }
void ImageBase::DrawImage(const Vertex* _vertex,const Vertex* _uv, const Index* _index,const Frame* _frame) {
#ifdef _DEBUG
	if(!_vertex)throw ImageBaseError(ImageBaseError::Code::NullVertex);
	if(!_uv)throw ImageBaseError(ImageBaseError::Code::NullUV);
	if(!_frame)throw ImageBaseError(ImageBaseError::Code::NullFrame);
	if(!_vertex->IsBuild())throw ImageBaseError(ImageBaseError::Code::NotVertexBuild);
	if(!_uv->IsBuild())throw ImageBaseError(ImageBaseError::Code::NotUVBuild);
	if(!_frame->IsBuild())throw ImageBaseError(ImageBaseError::Code::NotFrameBuild);
	if(_vertex->GetNum()!=_uv->GetNum())throw ImageBaseError(ImageBaseError::Code::VertexUVMismatch);
#endif
#ifdef _WIN32
	_System::_DirectX11::context->OMSetDepthStencilState(depthStencilState2D, 0xffffffff);

	_System::_DirectX11::context->VSSetShader(imgVert2DShader, nullptr, 0);//기본값 nullptr, nullptr, 0
	_System::_DirectX11::context->GSSetShader(nullptr, nullptr, 0);//사용하지 않는 셰이더는 비활성화 해야 됩니다.


	if (sampler) {
		imgPx2DClassInstances[imgPx2DisampleOffset] = imgPx2DsamplerSample;
		_System::_DirectX11::context->PSSetShader(imgPx2DShader, imgPx2DClassInstances, imgPx2DInterfaceNum);
		_System::_DirectX11::context->PSSetSamplers(0, 1, &sampler->sampler);
	} else {
		imgPx2DClassInstances[imgPx2DisampleOffset] = imgPx2DloadSample;
		_System::_DirectX11::context->PSSetShader(imgPx2DShader, imgPx2DClassInstances, imgPx2DInterfaceNum);
		_System::_DirectX11::context->PSSetSamplers(0, 1, &System::defaultSampler->sampler);
	}
	


	const ImgVertConstantStruct2D vsMat = { mat, Matrix::GetScale(2.f / (float)System::GetWindowWidth(), 2.f / (float)System::GetWindowHeight()) };
	const ImgPxConstantStruct2D px = { colorMat };

	if (context1) {
		context1->UpdateSubresource1(imgVertConstantBuffer2D, 0, nullptr, &vsMat, 0, 0, D3D11_COPY_DISCARD);
		context1->VSSetConstantBuffers1(0, 1, &imgVertConstantBuffer2D,nullptr,nullptr);//기본값 nullptr, nullptr

		context1->UpdateSubresource1(imgPxConstantBuffer2D, 0, nullptr, &px, 0, 0, D3D11_COPY_DISCARD);
		context1->PSSetConstantBuffers1(0, 1, &imgPxConstantBuffer2D,nullptr,nullptr);
	} else {
		_System::_DirectX11::context->UpdateSubresource(imgVertConstantBuffer2D, 0, nullptr, &vsMat, 0, 0);
		_System::_DirectX11::context->VSSetConstantBuffers(0, 1, &imgVertConstantBuffer2D);

		_System::_DirectX11::context->UpdateSubresource(imgPxConstantBuffer2D, 0, nullptr, &px, 0, 0);
		_System::_DirectX11::context->PSSetConstantBuffers(0, 1, &imgPxConstantBuffer2D);
	}

	ID3D11Buffer* buffers[] = { _vertex->vertex,_uv->vertex };
	UINT stride[] = { sizeof(PointF),sizeof(PointF) };
	UINT offset[] = { 0,0 };

	_System::_DirectX11::context->RSSetState(rasterizerState);

	_System::_DirectX11::context->IASetInputLayout(imgVert2DLayout);

	_System::_DirectX11::context->IASetVertexBuffers(0, 2, buffers, stride, offset);


	_System::_DirectX11::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_System::_DirectX11::context->PSSetShaderResources(0, 1, &_frame->srv);

	_System::_DirectX11::context->IASetIndexBuffer(_index->index, DXGI_FORMAT_R32_UINT, 0);

	_System::_DirectX11::context->DrawIndexed(_index->GetNum(),0,0);
#elif __ANDROID__
	if(glUseProgramStages) {
		glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgVertProg);

		glProgramUniformMatrix4fv(imgVertProg, imgVert::matUniform, 1, GL_FALSE, mat.e);

		glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgFragProg);

		glProgramUniformMatrix4fv(imgFragProg, imgFrag::colorMatUniform, 1,GL_FALSE,colorMat.e);
	} else {
		glUseProgram(imgProg);
		glUniformMatrix4fv(img::matUniform, 1, GL_FALSE, mat.e);
		glUniformMatrix4fv(img::colorMatUniform, 1,GL_FALSE,colorMat.e);
	}
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, _vertex->vertex);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _uv->vertex);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _frame->texture);

	glBindSampler(0, sampler->sampler);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableVertexAttribArray(1);
#endif
}