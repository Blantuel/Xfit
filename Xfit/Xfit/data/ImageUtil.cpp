#include "ImageUtil.h"

namespace ImageUtil {
	void Cut(const void* _src, ImageType _type, unsigned _srcWidth, unsigned _srcHeight, void* _dest, unsigned _destX, unsigned _destY, unsigned _destWidth, unsigned _destHeight) {
#ifdef _DEBUG
		if(!_src)throw CutError(CutError::Code::NullSrc);
		if(!_dest)throw CutError(CutError::Code::NullDest);
		if(_srcWidth==0)throw CutError(CutError::Code::ZeroSrcWidth);
		if(_srcHeight==0)throw CutError(CutError::Code::ZeroSrcHeight);
		if(_destWidth==0)throw CutError(CutError::Code::ZeroDestWidth);
		if(_destHeight==0)throw CutError(CutError::Code::ZeroDestHeight);
		if (_srcWidth < (_destX + _destWidth))throw CutError(CutError::Code::OutWidth);
		if (_srcHeight < (_destY + _destHeight))throw CutError(CutError::Code::OutHeight);
#endif
		if ((int)_type>=0 && (int)_type < 2) {//24bit
			const unsigned char* src = (const unsigned char*)_src;
			unsigned char* dest = (unsigned char*)_dest;
			for (unsigned h = 0; h < _destHeight; h++) {
				for (unsigned w = 0; w < _destWidth; w++) {
					const unsigned destIndex = (h * _destWidth + w)*3;
					const unsigned srcIndex = ((h + _destY)*_srcWidth + _destX + w) * 3;
					dest[destIndex] = src[srcIndex];
					dest[destIndex+1] = src[srcIndex+1];
					dest[destIndex+2] = src[srcIndex+2];
				}
			}
		} else if ((int)_type>=2 && (int)_type < 10) {//32bit
			const unsigned* src = (const unsigned*)_src;
			unsigned* dest = (unsigned*)_dest;
			for (unsigned h = 0; h < _destHeight; h++) {
				for (unsigned w = 0; w < _destWidth; w++) {
					const unsigned destIndex = h * _destWidth + w;
					const unsigned srcIndex = (h + _destY)*_srcWidth + _destX + w;
					dest[destIndex] = src[srcIndex];
				}
			}
		} else {
			throw CutError(CutError::Code::InvalidType);
		}

	}
}