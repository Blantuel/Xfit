#pragma once

#include "../system/Error.h"
#include "../data/Array.h"



class Font {
	friend class Label;
	friend class TextBox;

	struct CharImage {
		wchar_t text;
		int advanceX;
		int left, top;
		unsigned pixelSize;
		int width, height, pitch;
		unsigned char* bitmap;
		const Font* font;
	};
	static FT_Library library;
	static Array<CharImage*> charImages;
	static size_t charImageBlockLen;
	static std::mutex mutex;
	thread_local static unsigned* widths;
	thread_local static unsigned* heights;
	thread_local static int* paddings;
	thread_local static CharImage** charImages_t;
	thread_local static unsigned *image;
	thread_local static unsigned imageSize;
	thread_local static unsigned blockImageSize;

	FT_Face face;

	//static void RenderText(const unsigned char* _bitmap, int _xx, int _yy, unsigned _width, unsigned _height,
		//unsigned _pitch, FontRenderType _renderType);
public:
	Font(void* _data, unsigned _size, unsigned _index = 0);

	static void Init(size_t _charImageMaxLen, size_t _charImageBlockLen);
	static void ThreadInit(size_t _maxLine, size_t _maxLen, size_t _maxImageSize, size_t _blockImageSize);
	static void ThreadRelease();
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
struct FontSize {
	unsigned len;
	unsigned pixelSize;
};
struct FontColor {
	unsigned len;
	unsigned color;
};
struct FontContainer {
	unsigned len;
	Font* font;
};