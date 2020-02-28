#pragma once

#include "../resource/Frame.h"

class Font;
struct FontSize;
struct FontColor;
struct FontContainer;




class TextBox : public Frame {
public:
	enum class Align {
		Left, Center, Right
	};
	enum class VerticalAlign {
		High, Middle, Bottom
	};
	struct AlignPack {
		unsigned len;
		Align align;
	};
	struct LineSpacing {
		unsigned len;
		float lineSpacing;
	};
protected:
	void _PrepareDraw();
public:
	bool isScrollBox;
protected:
	unsigned scrollHeight;
public:
	unsigned scrollPx;
	VerticalAlign verticalAlign;
	unsigned width, height;
	wchar_t* text;
	FontContainer* fonts;
	FontColor* colors;
	FontSize* sizes;
	AlignPack* aligns;
	LineSpacing* lineSpacings;

	
	void PrepareDraw();
	void PrepareDrawEdit();
	unsigned GetScrollHeight()const;
};
