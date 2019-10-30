#pragma once

#include "ImageType.h"
#include "../system/Error.h"

namespace ImageUtil {
	class CutError : public Error {
	public:
		enum class Code {
			OutWidth,
			OutHeight,
			ZeroSrcWidth,
			ZeroSrcHeight,
			ZeroDestWidth,
			ZeroDestHeight,
			NullSrc,
			NullDest,
			InvalidType
		};
	protected:
		const Code code;
	public:
		Code GetCode()const{return code;}
		CutError(CutError::Code _code) :code(_code) {}
	};
	void Cut(const void* _src,ImageType _type,unsigned _srcWidth,unsigned _srcHeight,void* _dest,unsigned _destX,unsigned _destY,unsigned _destWidth,unsigned _destHeight);
}