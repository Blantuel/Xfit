#include "Label.h"
#include "Font.h"
#include "../data/Memory.h"

void Label::PrepareDraw(const wchar_t* _text, const Font* font, unsigned _pixelSize, unsigned _color, unsigned* _outWidth/* = nullptr*/, unsigned* _outHeight/* = nullptr*/) {
	const FT_Face& face = font->face;

	size_t len = wcslen(_text);
	Font::CharImage* charImages = new Font::CharImage[len];
	unsigned char** buffers = nullptr;
	if ((Font::charImages.MaxSize() - Font::charImages.Size()) < len) {
		buffers = new unsigned char*[len];
	}
	size_t bufferIndex = 0;

	int top = 0;
	unsigned width = 0, height = 0;
	unsigned imageSize;

	for (size_t i = 0;i<len; i++) {
		if (_text[i] == L'\n')continue;
		size_t charImageIndex = SIZE_MAX;

		for (size_t j = 0; j < Font::charImages.Size(); j++) {
			if ((Font::charImages[j].text == _text[i]) && (Font::charImages[j].pixelSize == _pixelSize)) {
				charImageIndex = j;
				break;
			}
		}
		if (charImageIndex == SIZE_MAX) {
			auto index = FT_Get_Char_Index(face, _text[i]);
			FT_Set_Pixel_Sizes(face, _pixelSize, 0);

			FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP);
			const FT_GlyphSlot& glyph = face->glyph;

			FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);

			charImages[i].width = glyph->bitmap.width;
			charImages[i].height = glyph->bitmap.rows;

			charImages[i].text = _text[i];
			imageSize = charImages[i].width * charImages[i].height;
			charImages[i].bitmap = new unsigned char[imageSize];

			Memory::Copy(charImages[i].bitmap, imageSize, glyph->bitmap.buffer, imageSize);

			charImages[i].advanceX = glyph->advance.x>>6;
			charImages[i].left = glyph->bitmap_left;
			charImages[i].top = glyph->bitmap_top;
			if (top < charImages[i].top)top = charImages[i].top;
			width += charImages[i].advanceX;

			if (Font::charImages.Size() < Font::charImages.MaxSize()) Font::charImages.InsertLast(charImages[i]);
			else buffers[bufferIndex++] = charImages[i].bitmap;
			
		} else {
			charImages[i] = Font::charImages[charImageIndex];
			if (top < charImages[i].top)top = charImages[i].top;
			width += charImages[i].advanceX;
		}
	}
	height = face->size->metrics.height>>6;
	if (_outWidth) * _outWidth = width;
	if (_outHeight) * _outHeight = height;
	imageSize = width * height;
	unsigned* outBitmap = new unsigned[imageSize];
	Memory::Set(outBitmap, 0U, (size_t)imageSize);

	int advanceX = 0;
	for (size_t i = 0; i < len; i++) {
		if (_text[i] == L'\n')continue;
		for (int y = 0; y < charImages[i].height; y++) {
			for (int x = 0; x < charImages[i].width; x++) {
				outBitmap[(int)width*(top - charImages[i].top +y)+advanceX + charImages[i].left + x] = ((unsigned)charImages[i].bitmap[charImages[i].width * y + x] << 24) | _color;
			}
		}
		advanceX += charImages[i].advanceX;
	}
	
	const RectF rect = { -1.f,1.f,1.f,-1.f };
	Build(outBitmap, width, height, rect);

	delete[]outBitmap;
	if (buffers) {
		for (size_t i = 0; i < bufferIndex; i++) delete[]buffers[i];
		delete[]buffers;
	}
}