#pragma once

#include "TextBox.h"



class SizeTextBox : public TextBox {
public:
	unsigned* baseSizes;
	unsigned baseWidth, baseHeight;
	unsigned sizeLen;
	//메모리 할당한 후 복사
	void CopyBaseSizes();
	//이미 있는 메모리에 복사
	void WriteBaseSizes();

	void SizePrepareDraw(float _ratio);
	void SizePrepareDrawEdit(float _ratio);
};
