#include "SizeTextBox.h"
#include "Font.h"

#include "../object/ScaleHeader.h"

void SizeTextBox::CopyBaseSizes() {
	baseSizes = new unsigned[sizeLen];
	WriteBaseSizes();
}
void SizeTextBox::WriteBaseSizes() {
	for (unsigned i = 0; i < sizeLen; i++) {
		baseSizes[i] = sizes[i].pixelSize;
	}
}
bool SizeTextBox::IsChangeSize(float _ratio)const {
	return sizes[0].pixelSize != (unsigned)((float)baseSizes[0] * _ratio);
}
void SizeTextBox::SizePrepareDraw(float _ratio) {
	if(IsBuild())Delete();
	for (unsigned i = 0; i < sizeLen; i++) sizes[i].pixelSize = (unsigned)((float)baseSizes[i] * _ratio);
	width = (unsigned)((float)baseWidth * _ratio);
	height = (unsigned)((float)baseHeight * _ratio);

	PrepareDraw();

	for (unsigned i = 0; i < sizeLen; i++) sizes[i].pixelSize = baseSizes[i];
	width = baseWidth;
	height = baseHeight;
}

void SizeTextBox::MakeBasicSizeTextBoxText(const wchar_t* _text, unsigned _baseTextWidth, unsigned _baseTextHeight, FontSize* _fontSize, unsigned* _baseSize, SizeTextBox* _sizeTextBox, unsigned _fontPx, FontColor* _fontColor/* = nullptr*/,
	unsigned _color/* = 0*/, TextBox::Align _align /*= TextBox::Align::Left*/, TextBox::VerticalAlign _verticalAlign /*= TextBox::VerticalAlign::High*/) {
	MakeBasicSizeTextBox(_baseTextWidth, _baseTextHeight, _fontSize, _baseSize, _sizeTextBox, _fontPx, _fontColor, _color, _align, _verticalAlign);

#ifdef _WIN32
	wcscpy_s(_sizeTextBox->text, wcslen(_text) + 1, _text);
#else
	wcscpy(_sizeTextBox->text, _text);
#endif
	_sizeTextBox->SizePrepareDraw(WindowRatio());
}


void SizeTextBox::MakeBasicSizeTextBox(unsigned _baseTextWidth, unsigned _baseTextHeight, FontSize* _fontSize, unsigned* _baseSize, SizeTextBox* _sizeTextBox, unsigned _fontPx, FontColor* _fontColor/* = nullptr*/, unsigned _color /*= 0*/,
	TextBox::Align _align /*= TextBox::Align::Left*/, TextBox::VerticalAlign _verticalAlign /*= TextBox::VerticalAlign::High*/) {
	_sizeTextBox->fonts = fontContainer;

	if (_fontColor) {
		_sizeTextBox->colors = _fontColor;
		_sizeTextBox->colors[0].color = _color;
		_sizeTextBox->colors[0].len = 0;
	} else {
		_sizeTextBox->colors = fontColor;
	}

	_sizeTextBox->sizes = _fontSize;
	_sizeTextBox->sizes[0].len = 0;
	_sizeTextBox->sizes[0].pixelSize = 0;



	_sizeTextBox->baseSizes = _baseSize;
	_sizeTextBox->sizeLen = 1;
	_baseSize[0] = _fontPx;

	switch (_align) {
	case TextBox::Align::Left:
		_sizeTextBox->aligns = fontLeftAlignPack;
		break;
	case TextBox::Align::Center:
		_sizeTextBox->aligns = fontCenterAlignPack;
		break;
	case TextBox::Align::Right:
		_sizeTextBox->aligns = fontRightAlignPack;
		break;
	default:
		throw 0;
	}

	_sizeTextBox->verticalAlign = _verticalAlign;
	_sizeTextBox->lineSpacings = fontLineSpacing;

	_sizeTextBox->baseWidth = _baseTextWidth;
	_sizeTextBox->baseHeight = _baseTextHeight;
}