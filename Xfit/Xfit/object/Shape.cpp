#include "Shape.h"
#include "../resource/ShapeVertex.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"
#include "../_system/_DirectX11.h"

//using namespace _System::_OpenGL;
using namespace _System::_DirectX11;


Shape::Shape():vertex(nullptr),fillColor(1.f,1.f,1.f,1.f),lineColor(0.f,0.f,0.f,1.f),lineWidth(1.f) {}
Shape::Shape(PointF _pos, PointF _scale, float _rotation, Blend* _blend, ShapeVertex* _vertex, Point3DwF _fillColor /*= Point3DwF(1.f, 1.f, 1.f, 1.f)*/,
	Point3DwF _lineColor /*= Point3DwF(0.f, 0.f, 0.f, 1.f)*/, float _lineWidth /*= 1.f*/)
	: MatrixObject(_pos, _scale, _rotation, _blend), vertex(_vertex), fillColor(_fillColor), lineColor(_lineColor),lineWidth(_lineWidth) {}

void Shape::Draw() {
#ifdef _DEBUG
	if(!vertex)throw ShapeError(ShapeError::Code::NullVertex);
	if(!vertex->IsBuild())throw ShapeError(ShapeError::Code::NotVertexBuild);
#endif
	if (!visible)return;
	Object::Draw();

	_System::_DirectX11::context->OMSetDepthStencilState(depthStencilState2D, 0xffffffff);

	_System::_DirectX11::context->VSSetShader(shapeVert2DShader, nullptr, 0);
	_System::_DirectX11::context->GSSetShader(shapeGeo2DShader, nullptr, 0);

	_System::_DirectX11::context->PSSetShader(shapePx2DShader, nullptr, 0);

	ShapeGeoConstantStruct2D shapeGeoContant = { Matrix::GetScale(2.f / (float)System::GetWindowWidth(), 2.f / (float)System::GetWindowHeight()),
	fillColor, lineColor, lineWidth, vertex->GetCenterPos().SetTransform(mat) };

	if (context1) {
		context1->UpdateSubresource1(shapeVertConstantBuffer2D, 0, nullptr, &mat, 0, 0, D3D11_COPY_DISCARD);
		context1->VSSetConstantBuffers1(0, 1, &shapeVertConstantBuffer2D, nullptr, nullptr);//±âº»°ª nullptr, nullptr

		context1->UpdateSubresource1(shapeGeoConstantBuffer2D, 0, nullptr, &shapeGeoContant, 0, 0, D3D11_COPY_DISCARD);
		context1->GSSetConstantBuffers1(0, 1, &shapeGeoConstantBuffer2D, nullptr, nullptr);
	} else {
		_System::_DirectX11::context->UpdateSubresource(shapeVertConstantBuffer2D, 0, nullptr, &mat, 0, 0);
		_System::_DirectX11::context->VSSetConstantBuffers(0, 1, &shapeVertConstantBuffer2D);

		_System::_DirectX11::context->UpdateSubresource(shapeGeoConstantBuffer2D, 0, nullptr, &shapeGeoContant, 0, 0);
		_System::_DirectX11::context->GSSetConstantBuffers(0, 1, &shapeGeoConstantBuffer2D);
	}

	ID3D11Buffer* buffers[] = { vertex->vertex };
	UINT stride[] = { sizeof(PointF) };
	UINT offset[] = { 0 };

	_System::_DirectX11::context->RSSetState(rasterizerState);

	_System::_DirectX11::context->IASetInputLayout(shapeVert2DLayout);

	_System::_DirectX11::context->IASetVertexBuffers(0, 1, buffers, stride, offset);


	_System::_DirectX11::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	_System::_DirectX11::context->Draw(vertex->GetNum()+1,0);

	/*if(glUseProgramStages) {
		glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, shapeVertProg);

		glProgramUniformMatrix4fv(shapeVertProg, shapeVert::matUniform, 1, GL_FALSE, mat.e);

		glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, shapeFragProg);

		glProgramUniform4fv(shapeFragProg, shapeFrag::colorUniform, 1, (float*)&color);
	} else {
		glUseProgram(shapeProg);
		glUniformMatrix4fv(shape::matUniform, 1, GL_FALSE, mat.e);
		glUniform4fv(shape::colorUniform, 1, (float*)&color);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertex->vertex);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);*/
}