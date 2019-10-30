#include "ImageMultiInstance.h"
#include "../math/Point3Dw.h"
#include "../data/Array.h"
#include "../resource/Sampler.h"

#include "../resource/FrameArray.h"
#include "../resource/Vertex2D.h"

#include "../_system/_GraphicsBase.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"

using namespace _System::_OpenGL;


ImageMultiInstance::ImageMultiInstance():ins(0),sampler(System::defaultSampler), uv(System::defaultUV), vertex(nullptr), frame(nullptr), nodes(nullptr) {
	colorMat.Identity();
}
ImageMultiInstance::~ImageMultiInstance() {
	if (ins)glDeleteBuffers(1, &ins);
}
void ImageMultiInstance::Draw() {
	if (visible) {
#ifdef _DEBUG
		if (!ins);
#endif
		Object::Draw();

		if (glUseProgramStages) {
			glUseProgramStages(progPipeline, GL_VERTEX_SHADER_BIT, imgMultiInsVertProg);
			glUseProgramStages(progPipeline, GL_FRAGMENT_SHADER_BIT, imgMultiInsFragProg);

			glActiveShaderProgram(progPipeline, imgMultiInsFragProg);
			glUniformMatrix4fv(imgMultiInsFrag::colorMatUniform, 1, GL_FALSE, colorMat.e);
		} else {
			glUseProgram(imgMultiInsProg);
			glUniformMatrix4fv(imgMultiIns::colorMatUniform, 1, GL_FALSE, colorMat.e);
		}

		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);

		glBindBuffer(GL_ARRAY_BUFFER, vertex->vertex);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, uv->vertex);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindBuffer(GL_ARRAY_BUFFER, ins);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), 0);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)offsetof(Matrix, e[4]));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)offsetof(Matrix, e[8]));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(MultiInstanceNode), (void*)offsetof(Matrix, e[12]));
		glVertexAttribIPointer(6, 4, GL_UNSIGNED_INT, sizeof(MultiInstanceNode), (void*)sizeof(Matrix));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, frame->texture);

		glBindSampler(0, sampler->sampler);

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, (GLsizei)insLen);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);
	}
}

void ImageMultiInstance::BuildInstance() {
#ifdef _DEBUG
	if (!nodes);
	if (ins);
#endif
	glGenBuffers(1, &ins);
	glBindBuffer(GL_ARRAY_BUFFER, ins);

	insLen = nodes->Size();
	insMaxLen = nodes->MaxSize();
	if (insLen == insMaxLen) {
#ifdef _WIN32
		if (glBufferStorage) {
			glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(MultiInstanceNode), nodes->GetData(), GL_DYNAMIC_STORAGE_BIT);
		} else {
			glBufferData(GL_ARRAY_BUFFER, insMaxLen * sizeof(MultiInstanceNode), nodes->GetData(), GL_DYNAMIC_DRAW);
		}
#elif __ANDROID__
		glBufferData(GL_ARRAY_BUFFER, insMaxLen * sizeof(MultiInstanceNode), nodes->GetData(), GL_DYNAMIC_DRAW);
#endif
	} else {
#ifdef _WIN32
		if (glBufferStorage) {
			glBufferStorage(GL_ARRAY_BUFFER, insMaxLen * sizeof(MultiInstanceNode), nullptr, GL_DYNAMIC_STORAGE_BIT);
		} else {
			glBufferData(GL_ARRAY_BUFFER, insMaxLen * sizeof(MultiInstanceNode), nullptr, GL_DYNAMIC_DRAW);
		}
#elif __ANDROID__
		glBufferData(GL_ARRAY_BUFFER, insMaxLen * sizeof(MultiInstanceNode), nullptr, GL_DYNAMIC_DRAW);
#endif
		//glBufferSubData(GL_ARRAY_BUFFER, 0, insLen * sizeof(MultiInstanceNode), nodes->GetData());
	}
}
void ImageMultiInstance::UpdateInstance() {
#ifdef _DEBUG
	if (!nodes);
	if (!ins);
#endif
	insLen = nodes->Size();
	glBindBuffer(GL_ARRAY_BUFFER, ins);
	glBufferSubData(GL_ARRAY_BUFFER, 0, insLen * sizeof(MultiInstanceNode), nodes->GetData());
}
void ImageMultiInstance::Delete() {
#ifdef _DEBUG
	if (!ins);
#endif
	glDeleteBuffers(1, &ins);
	ins = 0;
}
unsigned ImageMultiInstance::GetNum()const {
#ifdef _DEBUG
	if (!ins);
#endif
	return insLen;
}
unsigned ImageMultiInstance::GetMaxNum()const {
#ifdef _DEBUG
	if (!ins);
#endif
	return insMaxLen;
}
bool ImageMultiInstance::IsBuild()const {
	return (bool)ins;
}
