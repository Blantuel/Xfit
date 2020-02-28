#pragma once

#include "SizeAnimateTextBox.h"
#include "../system/System.h"
#include "../object/ScaleHeader.h"

SizeAnimateTextBox::SizeAnimateTextBox(float _animateSpeed):animateSpeed(_animateSpeed), textCount(0), time(0.f) {
	text = nullptr;
}
void SizeAnimateTextBox::AllocText() {
	if (text) delete[]text;
	const unsigned len = wcslen(baseText);
	text = new wchar_t[len + 1];
	text[0] = 0;
}
bool SizeAnimateTextBox::Update() {
	const unsigned len = wcslen(baseText);
	if (textCount < len) {
		if (time >= animateSpeed) {
			textCount++;

			for (unsigned i = 0; i < textCount; i++) {
				text[i] = baseText[i];
			}
			text[textCount] = 0;

			SizePrepareDrawEdit(WindowRatio());
			
			time -= animateSpeed;
			return true;
		}
		time += System::GetDeltaTime();
	} else time = 0.f;
	return false;
}
SizeAnimateTextBox::~SizeAnimateTextBox() {
	if(text)delete[]text;
}