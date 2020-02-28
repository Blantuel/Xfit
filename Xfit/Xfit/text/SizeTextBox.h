#pragma once

#include "TextBox.h"



class SizeTextBox : public TextBox {
public:
	unsigned* baseSizes;
	unsigned baseWidth, baseHeight;
	unsigned sizeLen;
	//�޸� �Ҵ��� �� ����
	void CopyBaseSizes();
	//�̹� �ִ� �޸𸮿� ����
	void WriteBaseSizes();

	void SizePrepareDraw(float _ratio);
	void SizePrepareDrawEdit(float _ratio);
};
