#include "VisualTextBox.h"
#include <text/Font.h>
#include "main.h"

#include <system/Input.h>

VisualTextBox::VisualTextBox(unsigned _width, unsigned _height):textImage(PointF(0.f,0.f),PointF(_width, _height),
	0.f,System::defaultBlend,nullptr,&textBox,System::defaultVertex2D,System::defaultUV,System::defaultIndex), prevCharsLen(0) {
	textBox.aligns = new TextBox::AlignPack[1];
	textBox.aligns[0].align = TextBox::Align::Left;
	textBox.aligns[0].len = 0;
	textBox.colors = new FontColor[1];
	textBox.colors[0].color = 0;
	textBox.colors[0].len = 0;
	textBox.fonts = new FontContainer[1];
	textBox.fonts[0].font = font;
	textBox.fonts[0].len = 0;
	textBox.lineSpacings = new TextBox::LineSpacing[1];
	textBox.lineSpacings[0].lineSpacing = 1.f;
	textBox.lineSpacings[0].len = 0;
	textBox.sizes = new FontSize[1];
	textBox.sizes[0].pixelSize = 20;
	textBox.sizes[0].len = 0;
	textBox.isScrollBox = true;
	textBox.scrollPx = 1000000;


	textBox.width = _width;
	textBox.height = _height;
	textBox.text = text.c_str();
	textBox.PrepareDraw();
}
bool VisualTextBox::Update() {
	
	if (Input::GetEnterCharState() == EnterCharState::Making) {
		unsigned len = Input::GetChars().size();

		text.resize(text.size() - prevCharsLen);
		text += Input::GetChars();

		textBox.text = text.c_str();
		textBox.PrepareDrawEdit();
		prevCharsLen = Input::GetPrevCharsLen();
	} else if (Input::GetEnterCharState() == EnterCharState::Finish) {
		unsigned len = Input::GetChars().size();
		if (Input::GetChars()[0] == L'\b') {
			if(text.size()>0)text.pop_back();
		} else {
			text.resize(text.size() - prevCharsLen);
			text += Input::GetChars();
		}
		textBox.text = text.c_str();
		textBox.PrepareDrawEdit();
		prevCharsLen = 0;
	}
	
	
	return true;
}
void VisualTextBox::Draw() {
	textImage.Draw();
}