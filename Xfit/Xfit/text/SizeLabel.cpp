#include "SizeLabel.h"
#include "Font.h"

#include "../object/ScaleHeader.h"

void SizeLabel::CopyBaseSizes() {
	baseSizes = new unsigned[sizeLen];
	WriteBaseSizes();
}
void SizeLabel::WriteBaseSizes() {
	for (unsigned i = 0; i < sizeLen; i++) {
		baseSizes[i] = sizes[i].pixelSize;
	}
}
bool SizeLabel::IsChangeSize(float _ratio)const {
	return sizes[0].pixelSize != (unsigned)((float)baseSizes[0] * _ratio);
}
void SizeLabel::SizePrepareDraw(float _ratio) {
	for (unsigned i = 0; i < sizeLen; i++) sizes[i].pixelSize = (unsigned)((float)baseSizes[i] * _ratio);

	PrepareDraw();

	for (unsigned i = 0; i < sizeLen; i++) sizes[i].pixelSize = baseSizes[i];
}


void SizeLabel::MakeSizeLabelNoSet(FontSize* _fontSize, unsigned* _baseSize, SizeLabel* _sizeLabel, FontColor* _fontColor /*= nullptr*/) {
	_sizeLabel->fonts = fontContainer;

	if (_fontColor) {
		_sizeLabel->colors = _fontColor;
	} else {
		_sizeLabel->colors = fontColor;
	}
	_sizeLabel->sizes = _fontSize;
	_sizeLabel->sizes[0].len = 0;
	_sizeLabel->sizes[0].pixelSize = 0;

	_sizeLabel->baseSizes = _baseSize;
	_sizeLabel->sizeLen = 1;
}
void SizeLabel::MakeSizeLabel(FontSize* _fontSize, unsigned* _baseSize, SizeLabel* _sizeLabel, unsigned _fontPx, FontColor* _fontColor /*= nullptr*/, unsigned _color /*= 0*/) {
	_sizeLabel->fonts = fontContainer;

	if (_fontColor) {
		_sizeLabel->colors = _fontColor;
		_sizeLabel->colors[0].color = _color;
		_sizeLabel->colors[0].len = 0;
	} else {
		_sizeLabel->colors = fontColor;
	}

	_sizeLabel->sizes = _fontSize;
	_sizeLabel->sizes[0].len = 0;
	_sizeLabel->sizes[0].pixelSize = 0;

	_sizeLabel->baseSizes = _baseSize;
	_sizeLabel->sizeLen = 1;
	_baseSize[0] = _fontPx;
}
void SizeLabel::MakeSizeLabelTextNoDraw(const wchar_t* _text, FontSize* _fontSize, unsigned* _baseSize, SizeLabel* _sizeLabel, unsigned _fontPx, FontColor* _fontColor /*= nullptr*/, unsigned _color /*= 0*/) {
	MakeSizeLabel(_fontSize, _baseSize, _sizeLabel, _fontPx, _fontColor, _color);

	_sizeLabel->text = _text;
}
void SizeLabel::MakeSizeLabelText(const wchar_t* _text, FontSize* _fontSize, unsigned* _baseSize, SizeLabel* _sizeLabel, unsigned _fontPx, FontColor* _fontColor /*= nullptr*/, unsigned _color /*= 0*/) {
	MakeSizeLabel(_fontSize, _baseSize, _sizeLabel, _fontPx, _fontColor, _color);

	_sizeLabel->text = _text;
	_sizeLabel->SizePrepareDraw(WindowRatio());
}