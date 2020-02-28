#include "RTAnimateObject.h"
#include "../data/Array.h"
#include "../_system/_OpenGL.h"
#include "../resource/RTAnimateFrame.h"

#include "../data/Compress.h"

using namespace _System::_OpenGL;

RTAnimateObject::RTAnimateObject(PointF _pos,PointF _scale,float _rotation,Blend* _blend,Sampler* _sampler,Array<RTAnimateFrame>* _frames):
	RTAnimateObjectBase(_pos,_scale,_rotation,_blend,_sampler),frames(_frames) {}
RTAnimateObject::RTAnimateObject():frames(nullptr) {}
unsigned RTAnimateObject::GetTotalFrame() {
#ifdef _DEBUG
	if(!frames)throw AnimateBaseError(AnimateBaseError::Code::NullFrames);
#endif
	return frames->Size();
}
void RTAnimateObject::Draw() {
	if(visible) {
#ifdef _DEBUG
		if(!texture)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::NotBuild);
		if(!(*frames)[cFrame].data)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::NullData);
#endif

		glBindTexture(GL_TEXTURE_2D,texture);

		if(isCompressed) {
#ifdef _DEBUG
			if((*frames)[cFrame].size==0)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::ZeroSize);
#endif
			glCompressedTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,fmt,(*frames)[cFrame].size,(*frames)[cFrame].data);
		} else {
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,fmt,GL_UNSIGNED_BYTE,(*frames)[cFrame].data);
		}
		Object::Draw();

		DrawImage((*frames)[cFrame].vertex,(*frames)[cFrame].uv);
	}
}
bool RTAnimateObject::IsCompressed()const {
#ifdef _DEBUG
		if(!texture)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::NotBuild);
#endif

	return isCompressed;
}
void RTAnimateObject::Build(unsigned _width,unsigned _height,FrameFormat _format/* = FrameFormat::RGBA*/) {
#ifdef _DEBUG
	if(texture)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::AlreadyBuild);
	if(_width==0)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::ZeroWidth);
	if(_height==0)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::ZeroHeight);
#endif
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);

	width=_width;
	height=_height;

	switch(_format) {
	case FrameFormat::RGBA:
		fmt=GL_RGBA;
		break;
	case FrameFormat::RGB:
		fmt=GL_RGB;
		break;
	/*case FrameFormat::BGRA:
		fmt = GL_BGRA;
		_format = FrameFormat::RGBA;
		break;
	case FrameFormat::BGR:
		fmt = GL_BGR;
		_format = FrameFormat::RGB;
		break;*/
	default:throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::InvalidFormat);
	}

	if(glTexStorage2D)glTexStorage2D(GL_TEXTURE_2D,1,(GLenum)_format,width,height);
	else glTexImage2D(GL_TEXTURE_2D,0,(GLenum)_format,width,height,0,fmt,GL_UNSIGNED_BYTE,nullptr);
	isCompressed=false;
}
void RTAnimateObject::BuildCompress(unsigned _width,unsigned _height,FrameCompressFormat _format) {
#ifdef _DEBUG
	if(texture)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::AlreadyBuild);
	switch(_format) {
		case FrameCompressFormat::ETC2_RGBA:
		case FrameCompressFormat::ETC2_RGB:
		case FrameCompressFormat::ASTC_RGBA:
		case FrameCompressFormat::BPTC_RGBA:
		case FrameCompressFormat::DXT5_RGBA:
			break;
		default:throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::InvalidFormat);
	}
	if(_width==0)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::ZeroWidth);
	if(_height==0)throw RTAnimateObjectBaseError(RTAnimateObjectBaseError::Code::ZeroHeight);
#endif
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);

	width=_width;
	height=_height;

	fmt = (GLenum)_format;

	if(glTexStorage2D)glTexStorage2D(GL_TEXTURE_2D,1,(GLenum)_format,width,height);
	else glCompressedTexImage2D(GL_TEXTURE_2D,0,fmt,width,height,0,0,nullptr);
	isCompressed=true;
}
unsigned RTAnimateObject::GetWidth()const {return width;}
unsigned RTAnimateObject::GetHeight()const {return height;}