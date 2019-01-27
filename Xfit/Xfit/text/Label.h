#pragma once

#include "../object/Image.h"

class Font;

class Label : public Image {
public:
	void PrepareDraw(const wchar_t* _text,const Font* font,unsigned _pixelSize,unsigned _color, unsigned* _outWidth = nullptr,unsigned* _outHeight = nullptr);
};