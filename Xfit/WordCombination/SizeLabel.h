#pragma once
#include <text/Label.h>


class SizeLabel : public Label {
public:
	unsigned* baseSizes;
	unsigned sizeLen;
	//�޸� �Ҵ��� �� ����
	void CopyBaseSizes();
	//�̹� �ִ� �޸𸮿� ����
	void WriteBaseSizes();

	void SizePrepareDraw(float _ratio);
};

