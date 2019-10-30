#include "ImageInstance.h"
#include "../math/Point3Dw.h"
#include "../data/Array.h"
#include "../resource/Sampler.h"

#include "../resource/Frame.h"
#include "../resource/Vertex2D.h"

#include "../_system/_GraphicsBase.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"
#include "../system/System.h"

#include "../data/Compress.h"


using namespace _System::_OpenGL;


ImageInstance::ImageInstance():ins(0),sampler(System::defaultSampler),uv(System::defaultUV),vertex(nullptr),frame(nullptr),nodes(nullptr) {
	colorMat.Identity();
}
ImageInstance::~ImageInstance() {
	if (ins)glDeleteBuffers(1, &ins);
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
		if(glUseProgramStages) {
			glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgInsVertProg);
			glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgFragProg);

			glActiveShaderProgram(progPipeline, imgFragProg);
			glUniformMatrix4fv(imgFrag::colorMatUniform, 1,GL_FALSE,colorMat.e);
		}else{
			glUseProgram(imgInsProg);
			glUniformMatrix4fv(imgIns::colorMatUniform, 1,GL_FALSE,colorMat.e);
		}

		glBindBuffer(GL_ARRAY_BUFFER, vertex->vertex);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, uv->vertex);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, ins);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), 0);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)offsetof(Matrix,e[4]));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)offsetof(Matrix, e[8]));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)offsetof(Matrix, e[12]));
		
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);

		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frame->texture);

		glBindSampler(0, sampler->sampler);

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4,(GLsizei)insLen);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
	}
}

void ImageInstance::BuildInstance() {
#ifdef _DEBUG
	if(!nodes)throw ImageInstanceError(ImageInstanceError::Code::NullNodes);
	if(ins)throw ImageInstanceError(ImageInstanceError::Code::AlreadyBuild);
#endif
	glGenBuffers(1, &ins);
	glBindBuffer(GL_ARRAY_BUFFER, ins);

	insLen = nodes->Size();
	insMaxLen = nodes->MaxSize();
	if (insLen == insMaxLen) {
		if (glBufferStorage) {
			glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(Matrix), nodes->GetData(), GL_DYNAMIC_STORAGE_BIT);
		} else {
			glBufferData(GL_ARRAY_BUFFER, insMaxLen * sizeof(Matrix), nodes->GetData(), GL_DYNAMIC_DRAW);
		}
	} else {
		if (glBufferStorage) {
			glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(Matrix), nullptr, GL_DYNAMIC_STORAGE_BIT);
		} else {
			glBufferData(GL_ARRAY_BUFFER, insMaxLen * sizeof(Matrix), nullptr, GL_DYNAMIC_DRAW);
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, insLen * sizeof(Matrix), nodes->GetData());
	}
}
void ImageInstance::UpdateInstance() {
#ifdef _DEBUG
	if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
	if(!nodes)throw ImageInstanceError(ImageInstanceError::Code::NullNodes);
	if (nodes->Size() > insMaxLen)throw ImageInstanceError(ImageInstanceError::Code::TooBigNode);
#endif
	insLen = nodes->Size();
	glBindBuffer(GL_ARRAY_BUFFER, ins);
	glBufferSubData(GL_ARRAY_BUFFER, 0, insLen * sizeof(Matrix), nodes->GetData());
}
void ImageInstance::Delete() {
#ifdef _DEBUG
	if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
	glDeleteBuffers(1, &ins);
	ins=0;
}
unsigned ImageInstance::GetNum()const {
#ifdef _DEBUG
	if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
	return insLen;
}
unsigned ImageInstance::GetMaxNum()const {
#ifdef _DEBUG
	if(!ins)throw ImageInstanceError(ImageInstanceError::Code::NotBuild);
#endif
	return insMaxLen;
}
bool ImageInstance::IsBuild()const {
	return (bool)ins;
}