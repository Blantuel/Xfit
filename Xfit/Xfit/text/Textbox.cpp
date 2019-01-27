#include "Textbox.h"
#include "Font.h"
#include "../data/Memory.h"

void TextBox::PrepareDraw(const wchar_t* _text, const Font* font, unsigned _width, unsigned _height, unsigned _pixelSize, unsigned _color) {
	const FT_Face& face = font->face;

	size_t len = wcslen(_text);
	Font::CharImage* charImages = new Font::CharImage[len];
	unsigned char** buffers = nullptr;
	if ((Font::charImages.MaxSize() - Font::charImages.Size()) < len) {
		buffers = new unsigned char* [len];
	}
	size_t bufferIndex = 0;

	int top = 0;
	unsigned imageSize;
	unsigned lineHeight,totalHeight;

	for (size_t i = 0; i < len; i++) {
		size_t charImageIndex = SIZE_MAX;
		if (_text[i] == L'\n')continue;

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

			charImages[i].advanceX = glyph->advance.x >> 6;
			charImages[i].left = glyph->bitmap_left;
			charImages[i].top = glyph->bitmap_top;
			if (top < charImages[i].top)top = charImages[i].top;

			if (Font::charImages.Size() < Font::charImages.MaxSize()) Font::charImages.InsertLast(charImages[i]);
			else buffers[bufferIndex++] = charImages[i].bitmap;

		} else {
			charImages[i] = Font::charImages[charImageIndex];
			if (top < charImages[i].top)top = charImages[i].top;
		}
	}
	lineHeight = face->size->metrics.height >> 6;
	totalHeight = lineHeight;
	
	imageSize = _width * _height;
	unsigned* outBitmap = new unsigned[imageSize];
	Memory::Set(outBitmap, 0U, (size_t)imageSize);

	int advanceX = 0;
	for (size_t i = 0; i < len; i++) {
		if (totalHeight > _height)break;
		if (_text[i] == L'\n') {
			totalHeight += lineHeight;
			advanceX = 0;
			continue;
		}
		if ((advanceX+ charImages[i].advanceX) > _width) {
			totalHeight += lineHeight;
			advanceX = 0;
		}
		for (int y = 0; y < charImages[i].height; y++) {
			for (int x = 0; x < charImages[i].width; x++) {
				outBitmap[(int)_width * (top - charImages[i].top + y + totalHeight) + advanceX + charImages[i].left + x] = ((unsigned)charImages[i].bitmap[charImages[i].width * y + x] << 24) | _color;
			}
		}
		advanceX += charImages[i].advanceX;
	}

	const RectF rect = { -1.f,1.f,1.f,-1.f };
	Build(outBitmap, _width, _height, rect);

	if (buffers) {
		for (size_t i = 0; i < bufferIndex; i++) delete[]buffers[i];
		delete[]buffers;
	}
}
