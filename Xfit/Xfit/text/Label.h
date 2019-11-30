#pragma once

#include "../resource/Frame.h"

class Font;
struct FontSize;
struct FontColor;
struct FontContainer;
struct FontRender;

class Label : public Frame {
protected:
	unsigned maxScrollTextCount;
public:
	const wchar_t* text;
	FontContainer* fonts;
	FontColor* colors;
	FontSize* sizes;
	FontRender* renders;
	unsigned textWidth;
	unsigned scrollTextCount;
	int* charWidths;

	Label();
	void PrepareDraw();

	unsigned GetMaxScrollTextCount()const;
};