#pragma once

#include "../object/Image.h"
#include "../math/Rect.h"

class Font;

class Label : public Image {
public:
	/*enum class Align {
		Left, Center, Right
	};
	enum VerticalAlign {
		Top, Middle, Bottom
	};
	VerticalAlign verticalAlign;
	RectF textBox;
	Align align;*/
	void PrepareDraw(const wchar_t* _text,const Font* font,unsigned _pixelSize,unsigned _color, unsigned* _outWidth = nullptr,unsigned* _outHeight = nullptr);
};