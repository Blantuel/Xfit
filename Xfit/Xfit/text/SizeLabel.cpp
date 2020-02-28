#include "SizeLabel.h"
#include "Font.h"

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