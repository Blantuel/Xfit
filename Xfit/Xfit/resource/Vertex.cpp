#include "Vertex.h"

#include "../_system/_OpenGL.h"
#include "../_system/_DirectX11.h"

#ifdef _WIN32
using namespace _System::_DirectX11;
#elif __ANDROID__
using namespace _System::_OpenGL;
#endif

void Vertex::MakeImageVertex2D(PointF _size,PointF _pos, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/) {
#ifdef _DEBUG
	if(!vertices.GetData())throw VertexError(VertexError::Code::NoData);
	if(vertices.MaxSize()<4)throw VertexError(VertexError::Code::LessSize);
	if(vertices.Size()>0)throw VertexError(VertexError::Code::NotEmptyData);
	if(_size.x<=0.f||_size.y<=0.f)throw VertexError(VertexError::Code::InvalidImageSize);
#endif
	const float halfWidth = _size.x/2.f;
	const float halfHeight = _size.y/2.f;
	vertices.EmplaceLast(-halfWidth+_pos.x,halfHeight+_pos.y);
	vertices.EmplaceLast(halfWidth+_pos.x,halfHeight+_pos.y);
	vertices.EmplaceLast(halfWidth+_pos.x,-halfHeight+_pos.y);
	vertices.EmplaceLast(-halfWidth+_pos.x,-halfHeight+_pos.y);
}
void Vertex::MakeImageVertex2D(PointF _size, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/) {
#ifdef _DEBUG
	if (!vertices.GetData())throw VertexError(VertexError::Code::NoData);
	if (vertices.MaxSize() < 4)throw VertexError(VertexError::Code::LessSize);
	if (vertices.Size() > 0)throw VertexError(VertexError::Code::NotEmptyData);
	if (_size.x <= 0.f || _size.y <= 0.f)throw VertexError(VertexError::Code::InvalidImageSize);
#endif
	const float halfWidth = _size.x/2.f;
	const float halfHeight = _size.y/2.f;

	switch (_centerPointPos) {
	case CenterPointPos::Center:
		vertices.EmplaceLast(-halfWidth, halfHeight);
		vertices.EmplaceLast(halfWidth, halfHeight);
		vertices.EmplaceLast(halfWidth, -halfHeight);
		vertices.EmplaceLast(-halfWidth, -halfHeight);
		break;
	case CenterPointPos::TopLeft:
		vertices.EmplaceLast(0.f, 0.f);
		vertices.EmplaceLast(_size.x, 0.f);
		vertices.EmplaceLast(_size.x, -_size.y);
		vertices.EmplaceLast(0.f, -_size.y);
		break;
	case CenterPointPos::TopRight:
		vertices.EmplaceLast(-_size.x, 0.f);
		vertices.EmplaceLast(0.f, 0.f);
		vertices.EmplaceLast(0.f, -_size.y);
		vertices.EmplaceLast(-_size.x, -_size.y);
		break;
	case CenterPointPos::Left:
		vertices.EmplaceLast(0.f, halfHeight);
		vertices.EmplaceLast(_size.x, halfHeight);
		vertices.EmplaceLast(_size.x, -halfHeight);
		vertices.EmplaceLast(0.f, -halfHeight);
		break;
	case CenterPointPos::Right:
		vertices.EmplaceLast(-_size.x, halfHeight);
		vertices.EmplaceLast(0.f, halfHeight);
		vertices.EmplaceLast(0.f, -halfHeight);
		vertices.EmplaceLast(-_size.x, -halfHeight);
		break;
	case CenterPointPos::BottomLeft:
		vertices.EmplaceLast(0.f, _size.y);
		vertices.EmplaceLast(_size.x, _size.y);
		vertices.EmplaceLast(_size.x, 0.f);
		vertices.EmplaceLast(0.f, 0.f);
		break;
	case CenterPointPos::BottomRight:
		vertices.EmplaceLast(-_size.x, _size.y);
		vertices.EmplaceLast(0.f, _size.y);
		vertices.EmplaceLast(0.f, 0.f);
		vertices.EmplaceLast(-_size.x, 0.f);
		break;
	case CenterPointPos::Top:
		vertices.EmplaceLast(-halfWidth, 0.f);
		vertices.EmplaceLast(halfWidth, 0.f);
		vertices.EmplaceLast(halfWidth, -_size.y);
		vertices.EmplaceLast(-halfWidth, -_size.y);
		break;
	case CenterPointPos::Bottom:
		vertices.EmplaceLast(-halfWidth, _size.y);
		vertices.EmplaceLast(halfWidth, _size.y);
		vertices.EmplaceLast(halfWidth, 0.f);
		vertices.EmplaceLast(-halfWidth, 0.f);
		break;
	}
}
void Vertex::MakePtImageVertex2D(PointU _size, CenterPointPos _centerPointPos /*= CenterPointPos::Center*/) {
#ifdef _DEBUG
	if (!vertices.GetData())throw VertexError(VertexError::Code::NoData);
	if (vertices.MaxSize() < 4)throw VertexError(VertexError::Code::LessSize);
	if (vertices.Size() > 0)throw VertexError(VertexError::Code::NotEmptyData);
	if (_size.x <= 0.f || _size.y <= 0.f)throw VertexError(VertexError::Code::InvalidImageSize);
#endif
	float halfWidth, halfWidth2, halfHeight, halfHeight2;
	if (_size.x % 2 == 1) {
		halfWidth = (float)(_size.x / 2); 
		halfWidth2 = halfWidth+1;
	} else {
		halfWidth = (float)(_size.x / 2);
		halfWidth2 = halfWidth;
	}
	if (_size.y % 2 == 1) {
		halfHeight = (float)(_size.y / 2);
		halfHeight2 = halfHeight + 1;
	} else {
		halfHeight = (float)(_size.y / 2);
		halfHeight2 = halfHeight;
	}

	switch (_centerPointPos) {
	case CenterPointPos::Center:
		vertices.EmplaceLast(-halfWidth, halfHeight);
		vertices.EmplaceLast(halfWidth2, halfHeight);
		vertices.EmplaceLast(halfWidth2, -halfHeight2);
		vertices.EmplaceLast(-halfWidth, -halfHeight2);
		break;
	case CenterPointPos::TopLeft:
		vertices.EmplaceLast(0.f, 0.f);
		vertices.EmplaceLast((float)_size.x, 0.f);
		vertices.EmplaceLast((float)_size.x, -(float)_size.y);
		vertices.EmplaceLast(0.f, -(float)_size.y);
		break;
	case CenterPointPos::TopRight:
		vertices.EmplaceLast(-(float)_size.x, 0.f);
		vertices.EmplaceLast(0.f, 0.f);
		vertices.EmplaceLast(0.f, -(float)_size.y);
		vertices.EmplaceLast(-(float)_size.x, -(float)_size.y);
		break;
	case CenterPointPos::Left:
		vertices.EmplaceLast(0.f, halfHeight);
		vertices.EmplaceLast((float)_size.x, halfHeight);
		vertices.EmplaceLast((float)_size.x, -halfHeight2);
		vertices.EmplaceLast(0.f, -halfHeight2);
		break;
	case CenterPointPos::Right:
		vertices.EmplaceLast(-(float)_size.x, halfHeight);
		vertices.EmplaceLast(0.f, halfHeight);
		vertices.EmplaceLast(0.f, -halfHeight2);
		vertices.EmplaceLast(-(float)_size.x, -halfHeight2);
		break;
	case CenterPointPos::BottomLeft:
		vertices.EmplaceLast(0.f, (float)_size.y);
		vertices.EmplaceLast((float)_size.x, (float)_size.y);
		vertices.EmplaceLast((float)_size.x, 0.f);
		vertices.EmplaceLast(0.f, 0.f);
		break;
	case CenterPointPos::BottomRight:
		vertices.EmplaceLast(-(float)_size.x, (float)_size.y);
		vertices.EmplaceLast(0.f, (float)_size.y);
		vertices.EmplaceLast(0.f, 0.f);
		vertices.EmplaceLast(-(float)_size.x, 0.f);
		break;
	case CenterPointPos::Top:
		vertices.EmplaceLast(-halfWidth, 0.f);
		vertices.EmplaceLast(halfWidth2, 0.f);
		vertices.EmplaceLast(halfWidth2, -(float)_size.y);
		vertices.EmplaceLast(-halfWidth, -(float)_size.y);
		break;
	case CenterPointPos::Bottom:
		vertices.EmplaceLast(-halfWidth, (float)_size.y);
		vertices.EmplaceLast(halfWidth2, (float)_size.y);
		vertices.EmplaceLast(halfWidth2, 0.f);
		vertices.EmplaceLast(-halfWidth, 0.f);
		break;
	}
}
#ifdef _WIN32
Vertex::Vertex() {
	vertex = nullptr;
}
#elif __ANDROID__
Vertex::Vertex() {
	vertex = 0;
}
#endif
Vertex::~Vertex() {
#ifdef _WIN32
	if (vertex)vertex->Release();
#elif __ANDROID__
	if(vertex)glDeleteBuffers(1,&vertex);
#endif
}
void Vertex::_Build(PointF* _vertices, unsigned _num, bool _editable) {
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = _num * sizeof(PointF);
	bufferDesc.StructureByteStride = 0;//Structured Buffer만 해당
	bufferDesc.Usage = _editable ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subSourceData;
	subSourceData.pSysMem = _vertices;
	subSourceData.SysMemPitch = 0;//2D, 3D 텍스쳐만 해당
	subSourceData.SysMemSlicePitch = 0;//3D 텍스쳐만 해당
	device->CreateBuffer(&bufferDesc, &subSourceData, &vertex);
}
void Vertex::_Edit(PointF* _vertices, unsigned _num) {
	if (context1) {
		context1->UpdateSubresource1(vertex, 0, nullptr, _vertices, 0, 0, D3D11_COPY_DISCARD);
	} else {
		context->UpdateSubresource(vertex, 0, nullptr, _vertices, 0, 0);
	}
}
void Vertex::Build(bool _editable/*=false*/) {
#ifdef _DEBUG
	if (IsBuild())throw VertexError(VertexError::Code::AlreadyBuild);
#endif
	num=vertices.Size();

#ifdef _WIN32
	_Build(vertices.GetData(), num, _editable);


#elif __ANDROID__
	glGenBuffers(1, &vertex);
	glBindBuffer(GL_ARRAY_BUFFER, vertex);

	if (glBufferStorage) {
		glBufferStorage(GL_ARRAY_BUFFER, num * sizeof(PointF), vertices.GetData(), 
			_editable?GL_DYNAMIC_STORAGE_BIT:0);
	} else {
		glBufferData(GL_ARRAY_BUFFER, num * sizeof(PointF), vertices.GetData(), 
			_editable?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);
	}
#endif
}
bool Vertex::IsBuild()const {return (bool)vertex;}
unsigned Vertex::GetNum()const {
#ifdef _DEBUG
	if (!IsBuild())throw VertexError(VertexError::Code::NotBuild);
#endif
	return num;
}

void Vertex::Edit() {
#ifdef _DEBUG
	if (!IsBuild())throw VertexError(VertexError::Code::NotBuild);
#endif
#ifdef _WIN32
	Vertex::_Edit(vertices.GetData(), num);
#elif __ANDROID__
	glBindBuffer(GL_ARRAY_BUFFER, vertex);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(PointF), vertices.GetData());
#endif
}
void Vertex::Delete() {
#ifdef _DEBUG
	if (!IsBuild())throw VertexError(VertexError::Code::NotBuild);
#endif
#ifdef _WIN32
	vertex->Release();
	vertex = nullptr;
#elif __ANDROID__
	glDeleteBuffers(1,&vertex);
	vertex=0;
#endif
}