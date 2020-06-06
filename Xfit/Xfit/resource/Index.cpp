#include "Index.h"

#include "../_system/_OpenGL.h"
#include "../_system/_DirectX11.h"
#include "../_system/_Android.h"

#ifdef _WIN32
using namespace _System::_DirectX11;
#elif __ANDROID__
using namespace _System::_OpenGL;
#endif

#ifdef _WIN32
Index::Index() {
	index = nullptr;
}
#elif __ANDROID__
Index::Index() {
	index = 0;
}
#endif
Index::~Index() {
#ifdef _WIN32
	if (index)index->Release();
#elif __ANDROID__
	_System::_Android::Lock();
	if (index)glDeleteBuffers(1, &index);
	_System::_Android::Unlock();
#endif
}
void Index::Build(bool _editable/*=false*/) {
#ifdef _DEBUG
	//if (IsBuild())throw IndexError(IndexError::Code::AlreadyBuild);
#endif
	num = indices.Size();

#ifdef _WIN32

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = num * sizeof(unsigned);
	bufferDesc.StructureByteStride = 0;//Structured Buffer만 해당
	bufferDesc.Usage = _editable ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subSourceData;
	subSourceData.pSysMem = indices.GetData();
	subSourceData.SysMemPitch = 0;//2D, 3D 텍스쳐만 해당
	subSourceData.SysMemSlicePitch = 0;//3D 텍스쳐만 해당
	device->CreateBuffer(&bufferDesc, &subSourceData, &index);


#elif __ANDROID__
	_System::_Android::Lock();
	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num * sizeof(unsigned), indices.GetData(),
			_editable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	_System::_Android::Unlock();
#endif
}
bool Index::IsBuild()const { return (bool)index; }
unsigned Index::GetNum()const {
#ifdef _DEBUG
	//if (!IsBuild())throw VertexError(VertexError::Code::NotBuild);
#endif
	return num;
}

void Index::Edit() {
#ifdef _DEBUG
	//if (!IsBuild())throw VertexError(VertexError::Code::NotBuild);
#endif
#ifdef _WIN32
	if (context1T) {
		context1T->UpdateSubresource1(index, 0, nullptr, indices.GetData(), 0, 0, D3D11_COPY_DISCARD);
	} else {
		contextT->UpdateSubresource(index, 0, nullptr, indices.GetData(), 0, 0);
	}
#elif __ANDROID__
	_System::_Android::Lock();
	glBindBuffer(GL_ARRAY_BUFFER, index);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(unsigned), indices.GetData());
	_System::_Android::Unlock();
#endif
}
void Index::Delete() {
#ifdef _DEBUG
	//if (!IsBuild())throw VertexError(VertexError::Code::NotBuild);
#endif
#ifdef _WIN32
	index->Release();
	index = nullptr;
#elif __ANDROID__
	_System::_Android::Lock();
	glDeleteBuffers(1, &index);
	_System::_Android::Unlock();
	index = 0;
#endif
}