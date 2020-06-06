#include "FrameArray.h"
#include "../_system/_OpenGL.h"
#include "../_system/_DirectX11.h"

#ifdef __ANDROID__

#include "../_system/_Android.h"

using namespace _System::_OpenGL;


unsigned FrameArray::GetWidth()const {
#ifdef _DEBUG
	if (!texture);
#endif
	return width;
}
unsigned FrameArray::GetHeight()const {
#ifdef _DEBUG
	if (!texture);
#endif
	return height;
}
unsigned FrameArray::GetFrameLen()const {
#ifdef _DEBUG
	if (!texture);
#endif
	return len;
}
FrameArray::FrameArray() {
	texture = 0;
}
bool FrameArray::IsCompressed()const {
#ifdef _DEBUG
	if (!IsBuild());
#endif
	return isCompressed;
}
bool FrameArray::IsBuild()const { return (bool)texture; }
void FrameArray::Delete() {
#ifdef _DEBUG
	if (!IsBuild());
#endif
	_System::_Android::Lock();
	glDeleteTextures(1, &texture);
	_System::_Android::Unlock();
	texture = 0;
}
FrameArray::~FrameArray() { if(texture)glDeleteTextures(1, &texture); }

FrameArray::FrameArray(const void* _data,unsigned _len, unsigned _width, unsigned _height, FrameFormat _format/* = FrameFormat::RGBA*/, unsigned _mipLevels /*= 1*/) {Build(_data, _len, _width, _height,_format);}
void FrameArray::Build(const void* _data,unsigned _len, unsigned _width, unsigned _height, FrameFormat _format/* = FrameFormat::RGBA*/, unsigned _mipLevels /*= 1*/) {
#ifdef _DEBUG
	if (texture);
#endif
	width = _width;
	height = _height;
	len=_len;
	_System::_Android::Lock();
	glGenTextures(1, &texture);

	switch (_format) {
	case FrameFormat::RGBA:
		fmt = GL_RGBA;
		break;
	case FrameFormat::RGB:
		fmt = GL_RGB;
		break;
	/*case FrameFormat::BGRA:
		fmt = GL_BGRA;
		_format = FrameFormat::RGBA;
		break;
	case FrameFormat::BGR:
		fmt = GL_BGR;
		_format = FrameFormat::RGB;
		break;*/
	default:
		break;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, len);
	if (_data)glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, len, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, len ,0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	_System::_Android::Unlock();
	isCompressed = false;
}
void FrameArray::Edit(const void* _data, unsigned _start, unsigned _len, unsigned _width, unsigned _height, unsigned _offsetX/* = 0*/, unsigned _offsetY/* = 0*/) {
#ifdef _DEBUG
	if (!texture);
#endif
	_System::_Android::Lock();
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,_offsetX,_offsetY,_start,_width,_height,_len,GL_RGBA,GL_UNSIGNED_BYTE,_data);
	_System::_Android::Unlock();
}
void FrameArray::BuildCompress(const void* _data, unsigned _size, unsigned _len, unsigned _width, unsigned _height, FrameCompressFormat _format) {
#ifdef _DEBUG
	if (IsBuild());
#endif
	width = _width;
	height = _height;
	len = _len;
	_System::_Android::Lock();
	glGenTextures(1, &texture);

	fmt = (GLenum)_format;

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	if (glTexStorage3D) {
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, fmt, width, height, len);
		if (_data)glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, len,fmt, _size, _data);
	} else {
		glCompressedTexImage3D(GL_TEXTURE_2D, 0, fmt, width, height, len, 0, _size, _data);
	}
	_System::_Android::Unlock();
	isCompressed = true;
}
void FrameArray::EditCompress(const void* _data, unsigned _size, unsigned _start, unsigned _len, unsigned _width, unsigned _height, unsigned _offsetX/* = 0*/, unsigned _offsetY /*= 0*/) {
	_System::_Android::Lock();
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, _offsetX, _offsetY, _start,_width, _height, _len, fmt, _size, _data);
	_System::_Android::Unlock();
}
#elif _WIN32

using namespace _System::_DirectX11;

unsigned FrameArray::GetWidth()const {
	return width;
}
unsigned FrameArray::GetHeight()const {
	return height;
}
unsigned FrameArray::GetFrameLen()const {
	return len;
}
bool FrameArray::IsCompressed()const {
	return isCompressed;
}
bool FrameArray::IsBuild()const {
	return (bool)texture || (bool)texture1;
}

FrameArray::FrameArray():width(0),height(0),len(0) {
	texture = nullptr;
	texture1 = nullptr;
	srv = nullptr;
	srv1 = nullptr;
}
FrameArray::~FrameArray() {
	if (texture1) {
		srv1->Release();
		texture1->Release();
	} else if (texture) {
		srv->Release();
		texture->Release();
	}
}
FrameArray::FrameArray(const void* _data, unsigned _len, unsigned _width, unsigned _height, FrameFormat _format /*= FrameFormat::RGBA*/, unsigned _mipLevels /*= 1*/) {
	texture = nullptr;
	texture1 = nullptr;
	srv = nullptr;
	srv1 = nullptr;

	Build(_data, _len, _width, _height, _format, _mipLevels);
}
void FrameArray::Build(const void* _data, unsigned _len, unsigned _width, unsigned _height, FrameFormat _format /*= FrameFormat::RGBA*/, unsigned _mipLevels /*= 1*/) {
#ifdef _DEBUG
	if (IsBuild())throw FrameArrayError(FrameArrayError::Code::AlreadyBuild);
#endif
	width = _width;
	height = _height;
	len = _len;

	if (_System::_DirectX11::device3) {
		D3D11_TEXTURE2D_DESC1 texture2DDesc;
		texture2DDesc.ArraySize = len;
		texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (_mipLevels != 1 ? D3D11_BIND_RENDER_TARGET : 0);
		texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2DDesc.Width = width;
		texture2DDesc.Height = height;
		texture2DDesc.MipLevels = _mipLevels;
		texture2DDesc.MiscFlags = _mipLevels != 1 ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
		texture2DDesc.SampleDesc.Count = 1;
		texture2DDesc.SampleDesc.Quality = 0;
		texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2DDesc.CPUAccessFlags = 0;
		texture2DDesc.TextureLayout = D3D11_TEXTURE_LAYOUT_UNDEFINED;

		if (_mipLevels == 1) {
			if (_data != nullptr) {
				D3D11_SUBRESOURCE_DATA* datas = new D3D11_SUBRESOURCE_DATA[len];
				for (unsigned i = 0; i < len; i++) {
					datas[i].pSysMem = &((unsigned char*)_data)[i * _width * _height * 4];
					datas[i].SysMemPitch = _width * 4;
					datas[i].SysMemSlicePitch = 0;
				}
				_System::_DirectX11::device3->CreateTexture2D1(&texture2DDesc, datas, &texture1);
				delete[]datas;
			} else _System::_DirectX11::device3->CreateTexture2D1(&texture2DDesc, nullptr, &texture1);
		} else {
			_System::_DirectX11::device3->CreateTexture2D1(&texture2DDesc, nullptr, &texture1);
			if (_data != nullptr) {
				if (context1T) {
					for (unsigned i = 0; i < len; i++) {
						context1T->UpdateSubresource1(texture1, D3D11CalcSubresource(0, i, _mipLevels), nullptr, &((unsigned char*)_data)[i * _width * _height * 4], _width * 4, 0, 0);
					}
				} else {
					for (unsigned i = 0; i < len; i++) {
						contextT->UpdateSubresource(texture1, D3D11CalcSubresource(0, i, _mipLevels), nullptr, &((unsigned char*)_data)[i * _width * _height * 4], _width * 4, 0);
					}
				}
			}
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC1 srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MipLevels = _mipLevels == 0 ? -1 : _mipLevels;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.PlaneSlice = 0;//The index (plane slice number) of the plane to use in the texture.

		srvDesc.Texture2DArray.ArraySize = len;
		srvDesc.Texture2DArray.FirstArraySlice = 0;

		_System::_DirectX11::device3->CreateShaderResourceView1(texture1, &srvDesc, &srv1);
	} else {
		D3D11_TEXTURE2D_DESC texture2DDesc;
		texture2DDesc.ArraySize = len;
		texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (_mipLevels != 1 ? D3D11_BIND_RENDER_TARGET : 0);
		texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2DDesc.Width = width;
		texture2DDesc.Height = height;
		texture2DDesc.MipLevels = _mipLevels;
		texture2DDesc.MiscFlags = _mipLevels != 1 ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
		texture2DDesc.SampleDesc.Count = 1;
		texture2DDesc.SampleDesc.Quality = 0;
		texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2DDesc.CPUAccessFlags = 0;

		if (_mipLevels == 1) {
			if (_data != nullptr) {
				D3D11_SUBRESOURCE_DATA* datas = new D3D11_SUBRESOURCE_DATA[len];
				for (unsigned i = 0; i < len; i++) {
					datas[i].pSysMem = &((unsigned char*)_data)[i * _width * _height * 4];
					datas[i].SysMemPitch = _width * 4;
					datas[i].SysMemSlicePitch = 0;
				}
				_System::_DirectX11::device3->CreateTexture2D(&texture2DDesc, datas, &texture);
				delete[]datas;
			} else _System::_DirectX11::device->CreateTexture2D(&texture2DDesc, nullptr, &texture);
		} else {
			_System::_DirectX11::device->CreateTexture2D(&texture2DDesc, nullptr, &texture);
			if (_data != nullptr) {
				if (context1T) {
					for (unsigned i = 0; i < len; i++) {
						context1T->UpdateSubresource1(texture, D3D11CalcSubresource(0, i, _mipLevels), nullptr, &((unsigned char*)_data)[i * _width * _height * 4], _width * 4, 0, 0);
					}
				} else {
					for (unsigned i = 0; i < len; i++) {
						contextT->UpdateSubresource(texture, D3D11CalcSubresource(0, i, _mipLevels), nullptr, &((unsigned char*)_data)[i * _width * _height * 4], _width * 4, 0);
					}
				}
			}
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MipLevels = _mipLevels == 0 ? -1 : _mipLevels;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.ArraySize = len;
		srvDesc.Texture2DArray.FirstArraySlice = 0;

		_System::_DirectX11::device->CreateShaderResourceView(texture, &srvDesc, &srv);
	}
	if (_mipLevels != 1) {
		if (context1T) {
			if (texture1)context1T->GenerateMips(srv1);
			else context1T->GenerateMips(srv);
		} else {
			contextT->GenerateMips(srv);
		}
	}
}
void FrameArray::Edit(const void* _data, unsigned _start, unsigned _len, unsigned _width, unsigned _height, unsigned _offsetX /*= 0*/, unsigned _offsetY /*= 0*/) {
	if (!IsBuild())throw FrameArrayError(FrameArrayError::Code::NotBuild);
	if (width < _width)throw FrameArrayError(FrameArrayError::Code::TooBigWidth);
	if (height < _height)throw FrameArrayError(FrameArrayError::Code::TooBigHeight);

	if (context1T) {
		if (texture1) {
			D3D11_TEXTURE2D_DESC1 desc;
			texture1->GetDesc1(&desc);
			for (unsigned i = _start; i < _len; i++) {
				context1T->UpdateSubresource1(texture1, D3D11CalcSubresource(0, i, desc.MipLevels), nullptr, &((unsigned char*)_data)[i * _width * _height * 4], _width * 4, 0, 0);
			}
		} else {
			D3D11_TEXTURE2D_DESC desc;
			texture->GetDesc(&desc);
			for (unsigned i = _start; i < _len; i++) {
				context1T->UpdateSubresource1(texture, D3D11CalcSubresource(0, i, desc.MipLevels), nullptr, &((unsigned char*)_data)[i * _width * _height * 4], _width * 4, 0, 0);
			}
		}
	} else {
		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);
		for (unsigned i = _start; i < _len; i++) {
			contextT->UpdateSubresource(texture, D3D11CalcSubresource(0, i, desc.MipLevels), nullptr, &((unsigned char*)_data)[i * _width * _height * 4], _width * 4, 0);
		}
	}
}
void FrameArray::BuildCompress(const void* _data, unsigned _size, unsigned _len, unsigned _width, unsigned _height, FrameCompressFormat _format) {

}
void FrameArray::EditCompress(const void* _data, unsigned _size, unsigned _start, unsigned _len, unsigned _width, unsigned _height, unsigned _offsetX /*= 0*/, unsigned _offsetY /*= 0*/) {

}

void FrameArray::Delete() {
#ifdef _DEBUG
	if (!IsBuild())throw FrameArrayError(FrameArrayError::Code::NotBuild);
#endif
	if (texture1) {
		srv1->Release();
		texture1->Release();
		texture1 = nullptr;
		srv1 = nullptr;
	} else if (texture) {
		srv->Release();
		texture->Release();
		texture = nullptr;
		srv = nullptr;
	}
}

#endif