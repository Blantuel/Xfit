#pragma once

#include "../resource/Frame.h"

class Font;
struct FontSize;
struct FontColor;
struct FontContainer;
struct FontRender;

class Label : public Frame {
public:
	const wchar_t* text;
	FontContainer* fonts;
	FontColor* colors;
	FontSize* sizes;
	FontRender* renders;

	Label();
	void PrepareDraw();
};