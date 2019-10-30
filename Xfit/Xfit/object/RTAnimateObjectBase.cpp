#include "RTAnimateObjectBase.h"
#include "../resource/Sampler.h"
#include "../resource/Vertex2D.h"
#include "../_system/_OpenGL.h"
#include "../system/System.h"

using namespace _System::_OpenGL;

void RTAnimateObjectBase::DrawImage(const Vertex2D* _vertex,const Vertex2D* _uv) {
#ifdef _DEBUG
	if(!_vertex)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::NullVertex);
	if(!_uv)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::NullUV);
	if(!_vertex->IsBuild())throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::NotVertexBuild);
	if(!_uv->IsBuild())throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::NotUVBuild);
	if(_vertex->GetNum()!=_uv->GetNum())throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::VertexUVMismatch);
#endif

	if(glUseProgramStages) {
		glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgVertProg);
		glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgFragProg);

		glActiveShaderProgram(progPipeline, imgVertProg);
		glUniformMatrix4fv(imgVert::matUniform, 1, GL_FALSE, mat.e);

		glActiveShaderProgram(progPipeline, imgFragProg);
		glUniformMatrix4fv(imgFrag::colorMatUniform, 1,GL_FALSE,colorMat.e);
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
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindSampler(0, sampler->sampler);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableVertexAttribArray(1);
}
void RTAnimateObjectBase::Delete() {
#ifdef _DEBUG
	if(!texture)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::NotBuild);
#endif
	glDeleteTextures(1,&texture);
	texture=0;
}

RTAnimateObjectBase::RTAnimateObjectBase(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler) :MatrixObject(_pos, _scale, _rotation, _blend), sampler(_sampler), texture(0) { colorMat.Identity(); }
RTAnimateObjectBase::RTAnimateObjectBase():texture(0),sampler(System::defaultSampler) { colorMat.Identity(); }
RTAnimateObjectBase::~RTAnimateObjectBase() {
	if(texture)glDeleteTextures(1,&texture);
}