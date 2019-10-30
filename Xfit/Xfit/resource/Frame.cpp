#include "Frame.h"
#include "../_system/_DirectX11.h"
#include "../_system/_OpenGL.h"

#ifdef _WIN32
using namespace _System::_DirectX11;
#elif __ANDROID__
using namespace _System::_OpenGL;
#endif


unsigned Frame::GetWidth()const {
#ifdef _DEBUG
	if (!IsBuild())throw FrameError(FrameError::Code::NotBuild);
#endif
	return width;
}
unsigned Frame::GetHeight()const {
#ifdef _DEBUG
	if (!IsBuild())throw FrameError(FrameError::Code::NotBuild);
#endif
	return height;
}

bool Frame::IsBuild()const { return (bool)texture; }
Frame::Frame():width(0),height(0){
#ifdef _WIN32
	texture = nullptr;
#elif __ANDROID__
	texture = 0;
	fmt = 0;
#endif
}

Frame::~Frame() { 
#ifdef _WIN32
	if (texture) {
		srv->Release();
		texture->Release();
	}
#elif __ANDROID__
	if (texture)glDeleteTextures(1, &texture);
#endif
	
}

Frame::Frame(const void* _data, unsigned _width, unsigned _height, FrameFormat _format/*=FrameFormat::RGBA*/) {
#ifdef _WIN32
	texture = nullptr;
#elif __ANDROID__
	texture = 0;
#endif
	Build(_data, _width, _height, _format);
}
void Frame::Build(const void* _data, unsigned _width, unsigned _height, FrameFormat _format/*=FrameFormat::RGBA*/) {
#ifdef _DEBUG
	if (IsBuild())throw FrameError(FrameError::Code::AlreadyBuild);
#endif
	width = _width;
	height = _height;

#ifdef _WIN32
	if (_System::_DirectX11::device3) {
		D3D11_TEXTURE2D_DESC1 texture2DDesc;
		texture2DDesc.ArraySize = 1;
		texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2DDesc.Width = width;
		texture2DDesc.Height = height;
		texture2DDesc.MipLevels = 1;
		texture2DDesc.MiscFlags = 0;
		texture2DDesc.SampleDesc.Count = 1;
		texture2DDesc.SampleDesc.Quality = 0;
		texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2DDesc.CPUAccessFlags = 0;
		texture2DDesc.TextureLayout = D3D11_TEXTURE_LAYOUT_UNDEFINED;

		if (_data == nullptr) {
			_System::_DirectX11::device3->CreateTexture2D1(&texture2DDesc, nullptr, &texture1);
		} else {
			D3D11_SUBRESOURCE_DATA subSourceData;
			subSourceData.pSysMem = _data;
			subSourceData.SysMemPitch = _width * 4;//byte단위
			subSourceData.SysMemSlicePitch = 0;

			_System::_DirectX11::device3->CreateTexture2D1(&texture2DDesc, &subSourceData, &texture1);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC1 srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.PlaneSlice = 0;//The index (plane slice number) of the plane to use in the texture.

		_System::_DirectX11::device3->CreateShaderResourceView1(texture1, &srvDesc, &srv1);
	} else {
		D3D11_TEXTURE2D_DESC texture2DDesc;
		texture2DDesc.ArraySize = 1;
		texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2DDesc.Width = width;
		texture2DDesc.Height = height;
		texture2DDesc.MipLevels = 1;
		texture2DDesc.MiscFlags = 0;
		texture2DDesc.SampleDesc.Count = 1;
		texture2DDesc.SampleDesc.Quality = 0;
		texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2DDesc.CPUAccessFlags = 0;

		if (_data == nullptr) {
			_System::_DirectX11::device->CreateTexture2D(&texture2DDesc, nullptr, &texture);
		} else {
			D3D11_SUBRESOURCE_DATA subSourceData;
			subSourceData.pSysMem = _data;
			subSourceData.SysMemPitch = _width * 4;//byte단위
			subSourceData.SysMemSlicePitch = 0;
			
			_System::_DirectX11::device->CreateTexture2D(&texture2DDesc, &subSourceData, &texture);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		
		_System::_DirectX11::device->CreateShaderResourceView(texture, &srvDesc, &srv);
	}
#elif __ANDROID__
	glGenTextures(1, &texture);

	switch (_format) {
	case FrameFormat::RGBA:
		fmt = GL_RGBA;
		break;
	case FrameFormat::RGB:
		fmt = GL_RGB;
		break;
	case FrameFormat::BGRA:
		fmt = GL_BGRA;
		_format = FrameFormat::RGBA;
		break;
	case FrameFormat::BGR:
		fmt = GL_BGR;
		_format = FrameFormat::RGB;
		break;
	default:
		break;
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	if (glTexStorage2D) {
		glTexStorage2D(GL_TEXTURE_2D, 1, (GLenum)_format, width, height);
		if (_data)glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, fmt, GL_UNSIGNED_BYTE, _data);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)_format, width, height, 0, fmt, GL_UNSIGNED_BYTE, _data);
	}
#endif
}
void Frame::BuildCompress(const void* _data, unsigned _size, unsigned _width, unsigned _height, FrameCompressFormat _format) {
#ifdef _DEBUG
	if (IsBuild())throw FrameError(FrameError::Code::AlreadyBuild);
#endif
	width = _width;
	height = _height;

#ifdef _WIN32
	
#elif __ANDROID__
	glGenTextures(1, &texture);

	fmt = (GLenum)_format;

	glBindTexture(GL_TEXTURE_2D, texture);
	if (glTexStorage2D) {
		glTexStorage2D(GL_TEXTURE_2D, 1, fmt, width, height);
		if (_data)glCompressedTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, fmt, _size, _data);
	} else {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, fmt, width, height, 0, _size, _data);
	}
#endif
}
void Frame::Delete() {
#ifdef _DEBUG
	if (!IsBuild())throw FrameError(FrameError::Code::NotBuild);
#endif

#ifdef _WIN32
	if (texture) {
		srv->Release();
		texture->Release();
		texture = nullptr;
	}
#elif __ANDROID__
	glDeleteTextures(1, &texture);
	texture = 0;
#endif
}
void Frame::Edit(const void* _data, unsigned _width, unsigned _height, unsigned _offsetX/* = 0*/, unsigned _offsetY/* = 0*/) {
#ifdef _DEBUG
	if (!IsBuild())throw FrameError(FrameError::Code::NotBuild);
	if (width < _width)throw FrameError(FrameError::Code::TooBigWidth);
	if (height < _height)throw FrameError(FrameError::Code::TooBigHeight);
#endif

#ifdef _WIN32
	if (context1) {
		context1->UpdateSubresource1(texture, 0, nullptr, _data, _width * 4, 0, D3D11_COPY_DISCARD);
	} else {
		context->UpdateSubresource(texture, 0, nullptr, _data, _width * 4, 0);
	}
#elif __ANDROID__
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, _offsetX, _offsetY, _width, _height, fmt, GL_UNSIGNED_BYTE, _data);
#endif
}
void Frame::EditCompress(const void* _data, unsigned _size, unsigned _width, unsigned _height, unsigned _offsetX/* = 0*/, unsigned _offsetY/* = 0*/) {
#ifdef _DEBUG
	if (!IsBuild())throw FrameError(FrameError::Code::NotBuild);
	if (width < _width)throw FrameError(FrameError::Code::TooBigWidth);
	if (height < _height)throw FrameError(FrameError::Code::TooBigHeight);
#endif
#ifdef _WIN32

#elif __ANDROID__
	glBindTexture(GL_TEXTURE_2D, texture);
	glCompressedTexSubImage2D(GL_TEXTURE_2D, 0, _offsetX, _offsetY, _width, _height, fmt, _size, _data);
#endif
}
