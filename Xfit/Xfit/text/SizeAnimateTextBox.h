#pragma once

#include "SizeTextBox.h"

class SizeAnimateTextBox : public SizeTextBox {
	float time;
public:
	const wchar_t* baseText;
	unsigned textCount;
	float animateSpeed;

	SizeAnimateTextBox(float _animateSpeed);
	void AllocText();

	bool Update();
	~SizeAnimateTextBox();
};