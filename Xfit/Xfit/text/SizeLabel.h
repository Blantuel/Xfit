#pragma once
#include "Label.h"

#include "../object/ScaleHeader.h"

class SizeLabel : public Label {
public:
	unsigned* baseSizes;
	unsigned sizeLen;
	//메모리 할당한 후 복사
	virtual void CopyBaseSizes();
	//이미 있는 메모리에 복사
	virtual void WriteBaseSizes();

	virtual bool IsChangeSize(float _ratio)const;
	virtual void SizePrepareDraw(float _ratio);


	static void LABEL_SIZE(SizeLabel* _label) { if (_label->IsChangeSize(WindowRatio())) { _label->SizePrepareDraw(WindowRatio()); } }
	static void LABEL_SIZE2(SizeLabel* _label, float _scale) {if(_label->IsChangeSize(WindowRatio() * _scale)){_label->SizePrepareDraw(WindowRatio() * _scale);}}


	static void MakeSizeLabelNoSet(FontSize* _fontSize, unsigned* _baseSize, SizeLabel* _sizeLabel, FontColor* _fontColor = nullptr);
	static void MakeSizeLabel(FontSize* _fontSize, unsigned* _baseSize, SizeLabel* _sizeLabel, unsigned _fontPx, FontColor* _fontColor = nullptr, unsigned _color = 0);
	static void MakeSizeLabelText(const wchar_t* _text, FontSize* _fontSize, unsigned* _baseSize, SizeLabel* _sizeLabel, unsigned _fontPx, FontColor* _fontColor = nullptr, unsigned _color = 0);
	static void MakeSizeLabelTextNoDraw(const wchar_t* _text, FontSize* _fontSize, unsigned* _baseSize, SizeLabel* _sizeLabel, unsigned _fontPx, FontColor* _fontColor = nullptr, unsigned _color = 0);
};
