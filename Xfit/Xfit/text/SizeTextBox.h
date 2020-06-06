#pragma once

#include "TextBox.h"

#include "../object/ScaleHeader.h"

class SizeTextBox : public TextBox {
public:
	unsigned* baseSizes;
	unsigned baseWidth, baseHeight;
	unsigned sizeLen;
	//메모리 할당한 후 복사
	void CopyBaseSizes();
	//이미 있는 메모리에 복사
	void WriteBaseSizes();

	bool IsChangeSize(float _ratio)const;

	void SizePrepareDraw(float _ratio);

	static void TEXTBOX_SIZE(SizeTextBox* _textBox) { if (_textBox->IsChangeSize(WindowRatio())) { _textBox->SizePrepareDraw(WindowRatio()); } }
	static void TEXTBOX_SIZE2(SizeTextBox* _textBox, float _scale) {if (_textBox->IsChangeSize(WindowRatio() * _scale)) { _textBox->SizePrepareDraw(WindowRatio() * _scale); }}


	static void MakeBasicSizeTextBox(unsigned _baseTextWidth, unsigned _baseTextHeight, FontSize* _fontSize, unsigned* _baseSize, SizeTextBox* _sizeTextBox, unsigned _fontPx, FontColor* _fontColor = nullptr, unsigned _color = 0, TextBox::Align _align = TextBox::Align::Left, TextBox::VerticalAlign _verticalAlign = TextBox::VerticalAlign::High);

	static void MakeBasicSizeTextBoxText(const wchar_t* _text, unsigned _baseTextWidth, unsigned _baseTextHeight, FontSize* _fontSize, unsigned* _baseSize, SizeTextBox* _sizeTextBox, unsigned _fontPx, FontColor* _fontColor = nullptr, unsigned _color = 0, TextBox::Align _align = TextBox::Align::Left, TextBox::VerticalAlign _verticalAlign = TextBox::VerticalAlign::High);

};
