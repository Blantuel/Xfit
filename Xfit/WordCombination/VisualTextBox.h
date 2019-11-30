#pragma once

#include <object/Shape.h>
#include <text/TextBox.h>
#include <object/Image.h>

class VisualTextBox {
	unsigned prevCharsLen;
	
	Image textImage;
public:
	wstring text;
	TextBox textBox;
	VisualTextBox(unsigned _width,unsigned _height);
	bool Update();
	void Draw();
};