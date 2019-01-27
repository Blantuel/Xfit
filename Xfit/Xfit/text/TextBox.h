#pragma once

#include "../object/Image.h"

class Font;


class TextBox : public Image {
public:
	enum class Align {
		Left, Center, Right
	};
	enum VerticalAlign {
		Top, Middle, Bottom
	};
	void PrepareDraw(const wchar_t* _text, const Font* font, unsigned _width,unsigned _height, Align _align, VerticalAlign _verticalAlign, unsigned _pixelSize, unsigned _color);
};