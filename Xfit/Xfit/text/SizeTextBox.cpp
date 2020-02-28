#include "SizeTextBox.h"
#include "Font.h"

void SizeTextBox::CopyBaseSizes() {
	baseSizes = new unsigned[sizeLen];
	WriteBaseSizes();
}
void SizeTextBox::WriteBaseSizes() {
	for (unsigned i = 0; i < sizeLen; i++) {
		baseSizes[i] = sizes[i].pixelSize;
	}
}
void SizeTextBox::SizePrepareDraw(float _ratio) {
	for (unsigned i = 0; i < sizeLen; i++) sizes[i].pixelSize = (unsigned)((float)baseSizes[i] * _ratio);
	width = (unsigned)((float)baseWidth * _ratio);
	height = (unsigned)((float)baseHeight * _ratio);

	PrepareDraw();

	for (unsigned i = 0; i < sizeLen; i++) sizes[i].pixelSize = baseSizes[i];
	width = baseWidth;
	height = baseHeight;
}
void SizeTextBox::SizePrepareDrawEdit(float _ratio) {
	Delete();
	SizePrepareDraw(_ratio);
}