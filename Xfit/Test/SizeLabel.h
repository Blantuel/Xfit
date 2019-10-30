#pragma once
#include <text/Label.h>


class SizeLabel : public Label {
public:
	unsigned* baseSizes;
	unsigned sizeLen;
	//메모리 할당한 후 복사
	void CopyBaseSizes();
	//이미 있는 메모리에 복사
	void WriteBaseSizes();

	void SizePrepareDraw(float _ratio);
};

