#pragma once

#include "../resource/Frame.h"

class Font;
struct FontSize;
struct FontColor;
struct FontContainer;

class Label : public Frame {
protected:
	unsigned maxScrollTextCount;
public:
	const wchar_t* text;
	FontContainer* fonts;
	FontColor* colors;
	FontSize* sizes;
	unsigned textWidth;
	unsigned scrollTextCount;
	int* charWidths;

	Label();
	virtual void PrepareDraw();

	unsigned GetMaxScrollTextCount()const;
};