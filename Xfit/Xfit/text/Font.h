#pragma once

#include "../system/Error.h"
#include "../data/Array.h"
#include <ft2build.h>
#include FT_FREETYPE_H


class FontError : public Error {
public:
	FontError( int _code) : Error( _code) {}
};
class Font {
	friend class Label;

	struct CharImage {
		wchar_t text;
		unsigned char* bitmap;
		int advanceX;
		int left, top;
		unsigned pixelSize, width, height;
	};

	static inline thread_local Array<CharImage> charImages;
	static inline FT_Library library = nullptr;

	FT_Face face;
public:
	Font(void* _data, unsigned _size, unsigned _index = 0);

	static void Init();
	static void CreateCharImages(size_t _charImageMaxLen);
	static void FreeCharImages();
	static void Release();
	bool IsBold()const;
	bool IsItalic()const;
	bool IsKerning()const;
	bool IsHinting()const;
	unsigned GetIndex()const;

	~Font();
	const char* GetFamilyName()const;
	const char* GetStyleName()const;
};
