#include "SizeLabel.h"
#include <text/Font.h>

void SizeLabel::CopyBaseSizes() {
	baseSizes = new unsigned[sizeLen];
	WriteBaseSizes();
}
void SizeLabel::WriteBaseSizes() {
	for (unsigned i = 0; i < sizeLen; i++) {
		baseSizes[i] = sizes[i].pixelSize;
	}
}
void SizeLabel::SizePrepareDraw(float _ratio) {
	for (unsigned i = 0; i < sizeLen; i++) sizes[i].pixelSize = (unsigned)((float)baseSizes[i] * _ratio);

	PrepareDraw();

	for (unsigned i = 0; i < sizeLen; i++) sizes[i].pixelSize =baseSizes[i];
}