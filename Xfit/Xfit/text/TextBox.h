#pragma once

#include "../object/Image.h"

class Font;


class TextBox : public Image {
public:
	void PrepareDraw(const wchar_t* _text, const Font* font, unsigned _width,unsigned _height, unsigned _pixelSize, unsigned _color);
};