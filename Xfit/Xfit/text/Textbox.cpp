#include "Textbox.h"
#include "Font.h"
#include "../data/Memory.h"

unsigned TextBox::GetScrollHeight()const {
	return scrollHeight;
}
void TextBox::_PrepareDraw() {
	unsigned len = (unsigned)wcslen(text);

	int top = INT_MIN;
	unsigned imageSize;
	int lineHeight=0, totalHeight=0;
	unsigned lineIndex = 0;
	unsigned heightIndex = 0;

	unsigned currentSize = 0;
	unsigned sizeIndex = 0;
	unsigned sizeLenIndex = 0;
	unsigned sizeLen;

	unsigned fontIndex = 0;
	unsigned fontLenIndex = 0;
	unsigned fontLen;

	unsigned colorIndex = 0;
	unsigned colorLenIndex = 0;
	unsigned colorLen;

	unsigned lineSpacingIndex = 0;
	unsigned lineSpacingLenIndex = 0;
	unsigned lineSpacingLen;

	unsigned alignIndex = 0;
	unsigned alignLenIndex = 0;
	unsigned alignLen;

	unsigned widthsIndex = 0;
	unsigned totalWidth = 0;

	unsigned paddingIndex=0;

	if (sizes[0].len == 0)sizeLen = len;
	else sizeLen = sizes[0].len;

	if (fonts[0].len == 0)fontLen = len;
	else fontLen = fonts[0].len;

	if (colors[0].len == 0)colorLen = len;
	else colorLen = colors[0].len;

	if (lineSpacings[0].len == 0)lineSpacingLen = UINT_MAX;
	else lineSpacingLen = lineSpacings[0].len;

	if (aligns[0].len == 0)alignLen = UINT_MAX;
	else alignLen = aligns[0].len;

	Font::paddings[paddingIndex] = 0;

	Font::mutex.lock();
	for (unsigned i = 0; i < len; i++, fontLenIndex++, sizeLenIndex++) {
		if (fontLenIndex >= fontLen) {
			fontIndex++;
			if (fonts[fontIndex].len == 0)fontLen = len - i;
			else fontLen = fonts[fontIndex].len;

			fontLenIndex = 0;
			currentSize = 0;
		}
		if (sizeLenIndex >= sizeLen) {
			sizeIndex++;
			if (sizes[sizeIndex].len == 0)sizeLen = len - i;
			else sizeLen = sizes[sizeIndex].len;

			sizeLenIndex = 0;
		}
		if (lineSpacingLenIndex >= lineSpacingLen) {
			lineSpacingIndex++;
			if (lineSpacings[lineSpacingIndex].len == 0)lineSpacingLen = len - i;
			else lineSpacingLen = lineSpacings[lineSpacingIndex].len;

			lineSpacingLenIndex = 0;
		}
		unsigned charImageIndex = UINT_MAX;

		const FT_Face face = fonts[fontIndex].font->face;
		if (currentSize != sizes[sizeIndex].pixelSize) {
			currentSize = sizes[sizeIndex].pixelSize;
			FT_Set_Pixel_Sizes(face, currentSize, 0);
		}
		const int heightT = (int)((float)(face->size->metrics.height >> 6)*lineSpacings[lineSpacingIndex].lineSpacing);
		if (lineHeight < heightT)lineHeight = heightT;

		if (text[i] == L'\n') {
			totalHeight += lineHeight;
			Font::heights[heightIndex++] = lineHeight;
			lineHeight = heightT;
			Font::widths[widthsIndex++] = totalWidth;
			totalWidth = 0;
			lineSpacingLenIndex++;
			paddingIndex++;
			Font::paddings[paddingIndex] = 0;
			continue;
		} else if (text[i] == L'\r') {
			if (i < len - 1 && text[i + 1] == L'\n') {
				i++;//줄바꿈이 \r\n일 경우 한글자 더 건너뜀.
			}
			totalHeight += lineHeight;
			Font::heights[heightIndex++] = lineHeight;
			lineHeight = heightT;
			Font::widths[widthsIndex++] = totalWidth;
			totalWidth = 0;
			lineSpacingLenIndex++;
			paddingIndex++;
			Font::paddings[paddingIndex] = 0;
			continue;
		}

		for (unsigned j = 0; j < Font::charImages.Size(); j++) {
			if ((Font::charImages[j]->text == text[i]) && (Font::charImages[j]->pixelSize == currentSize) && (Font::charImages[j]->font == fonts[fontIndex].font)) {
				charImageIndex = j;
				break;
			}
		}
		
		if (charImageIndex == UINT_MAX) {
			auto index = FT_Get_Char_Index(face, text[i]);

			FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP);
			const FT_GlyphSlot glyph = face->glyph;

			FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);

			if (Font::charImages.Size() >= Font::charImages.MaxSize()) Font::charImages.ReAlloc(Font::charImages.MaxSize() + Font::charImageBlockLen);
			Font::charImages.Resize(Font::charImages.Size() + 1);
			Font::charImages[Font::charImages.Size() - 1] = new Font::CharImage;

			Font::charImages_t[i] = Font::charImages[Font::charImages.Size() - 1];

			Font::charImages_t[i]->width = glyph->bitmap.width;
			Font::charImages_t[i]->height = glyph->bitmap.rows;

			Font::charImages_t[i]->text = text[i];
			Font::charImages_t[i]->pixelSize = currentSize;
			Font::charImages_t[i]->font = fonts[fontIndex].font;
			imageSize = Font::charImages_t[i]->width * Font::charImages_t[i]->height;
			if (imageSize == 0 || text[i] == L'	') {
				Font::charImages_t[i]->bitmap = nullptr;
			} else {
				Font::charImages_t[i]->bitmap = new unsigned char[imageSize];

				Memory::Copy(Font::charImages_t[i]->bitmap, imageSize, glyph->bitmap.buffer, imageSize);
			}

			Font::charImages_t[i]->advanceX = glyph->advance.x >> 6;//하위 6비트는 버린다.
			if (glyph->advance.x & 0x3F)Font::charImages_t[i]->advanceX++;//26.6소수의 소숫점 아래 자리(6비트)가 0이상이면
			Font::charImages_t[i]->left = glyph->bitmap_left;
			Font::charImages_t[i]->top = glyph->bitmap_top;

			if (top < Font::charImages_t[i]->top)top = Font::charImages_t[i]->top;
		} else {
			Font::charImages_t[i] = Font::charImages[charImageIndex];
			if (top < Font::charImages_t[i]->top)top = Font::charImages_t[i]->top;
		}
		
		if (totalWidth + Font::charImages_t[i]->advanceX > width) {
			totalHeight += lineHeight;
			Font::heights[heightIndex++] = lineHeight;
			lineHeight = heightT;
			Font::widths[widthsIndex++] = totalWidth;
			paddingIndex++;
			
			lineSpacingLenIndex++;
			if(Font::charImages_t[i]->advanceX > width) {
				totalWidth = 0;
				break;
			} else {
				totalWidth = 0;
				if (Font::charImages_t[i]->left < 0) {
					Font::paddings[paddingIndex] = -Font::charImages_t[i]->left;
				} else Font::paddings[paddingIndex] = 0;
			}
		} else {
			if (Font::charImages_t[i]->left < 0 && totalWidth < -Font::charImages_t[i]->left) {
				if (Font::paddings[paddingIndex] < -Font::charImages_t[i]->left)Font::paddings[paddingIndex] = -Font::charImages_t[i]->left;
				totalWidth += Font::paddings[paddingIndex];
			};
		}
		totalWidth += Font::charImages_t[i]->advanceX;
	}
	Font::mutex.unlock();

	if (lineHeight > 0) {
		totalHeight += lineHeight;
		Font::heights[heightIndex++] = lineHeight;
	}
	if (totalWidth > 0)Font::widths[widthsIndex++] = totalWidth;

	imageSize = width * height;
	Memory::Set(Font::image, 0U, (size_t)imageSize);

	int scrollPx2 = scrollPx;
	if (isScrollBox) {
		if(totalHeight - (int)height < 0) scrollHeight = 0;
		else scrollHeight = totalHeight - height;
		if (scrollPx2 > scrollHeight) scrollPx2 = scrollHeight;
	}

	int advanceX = 0;
	switch (aligns[0].align) {
	case Align::Left:
		advanceX = 0;
		break;
	case Align::Center:
		advanceX = (width - Font::widths[lineIndex]) / 2;
		break;
	case Align::Right:
		advanceX = width - Font::widths[lineIndex];
		break;
	default:
		break;
	}
	int totalHeight2;
	if (height <= totalHeight) {
		totalHeight2 = Font::heights[lineIndex];
	} else {
		switch (verticalAlign) {
		case VerticalAlign::High:
			totalHeight2 = Font::heights[lineIndex];
			break;
		case VerticalAlign::Middle:
			totalHeight2 = (height - totalHeight) / 2 + Font::heights[lineIndex];
			break;
		case VerticalAlign::Bottom:
			totalHeight2 = height - totalHeight + Font::heights[lineIndex];
			break;
		default:
			break;
		}
	}
	for (size_t i = 0; i < len; i++, colorLenIndex++) {
		if (colorLenIndex >= colorLen) {
			colorIndex++;
			if (colors[colorIndex].len == 0)colorLen = len - i;
			else colorLen = colors[colorIndex].len;

			colorLenIndex = 0;
		}
		if (alignLenIndex >= alignLen) {
			alignIndex++;
			if (aligns[colorIndex].len == 0)alignLen = len - i;
			else alignLen = aligns[alignIndex].len;

			alignLenIndex = 0;
		}

		if (text[i] == L'\n') {
			lineIndex++;
			alignLenIndex++;
			totalHeight2 += Font::heights[lineIndex];
			switch (aligns[alignIndex].align) {
			case Align::Left:
				advanceX = 0;
				break;
			case Align::Center:
				advanceX = (width - Font::widths[lineIndex]) / 2;
				break;
			case Align::Right:
				advanceX = width - Font::widths[lineIndex];
				break;
			}
			continue;
		} else if((text[i] == L'\r')) {
			if (i < len - 1 && text[i + 1] == L'\n') {
				i++;//줄바꿈이 \r\n일 경우 한글자 더 건너뜀.
			}
			lineIndex++;
			alignLenIndex++;
			totalHeight2 += Font::heights[lineIndex];
			switch (aligns[alignIndex].align) {
			case Align::Left:
				advanceX = 0;
				break;
			case Align::Center:
				advanceX = (width - Font::widths[lineIndex]) / 2;
				break;
			case Align::Right:
				advanceX = width - Font::widths[lineIndex];
				break;
			}
		} else if ((advanceX + Font::charImages_t[i]->advanceX) > width) {
			lineIndex++;
			alignLenIndex++;
			totalHeight2 += Font::heights[lineIndex];
			switch (aligns[alignIndex].align) {
			case Align::Left:
				advanceX = 0;
				break;
			case Align::Center:
				advanceX = (width - Font::widths[lineIndex]) / 2;
				break;
			case Align::Right:
				advanceX = width - Font::widths[lineIndex];
				break;
			}
		}
		if (isScrollBox) {
			if (totalHeight2 - Font::heights[lineIndex] > scrollPx + height)break;
		} else {
			if (totalHeight2 > height)break;
		}
		
		if (width < Font::charImages_t[i]->advanceX)break;

		if (Font::charImages_t[i]->bitmap) {
			totalHeight2 -= Font::heights[lineIndex];
			const int xx = advanceX + Font::charImages_t[i]->left + Font::paddings[lineIndex];
			
			const int textWidth = Font::charImages_t[i]->width;
			const int textHeight = Font::charImages_t[i]->height;
			const unsigned char*const bitmap = Font::charImages_t[i]->bitmap;

			if (isScrollBox) {
				const int yy = top - Font::charImages_t[i]->top + totalHeight2 - scrollPx2;
				int startY;
				if (yy < 0) {
					startY = -yy;
				} else startY = 0;
				for (int y = startY; y < textHeight; y++) {
					const int y2 = yy + y - startY;
					if (y2 < 0) continue;
					else if (y2 >= height)break;
					for (int x = 0; x < textWidth; x++) {
						Font::image[width * y2 + xx + x] = ((unsigned)bitmap[textWidth * y + x] << 24) | colors[colorIndex].color;
					}
				}
			} else {
				const int yy = top - Font::charImages_t[i]->top + totalHeight2;
				for (int y = 0; y < textHeight; y++) {
					const int y2 = yy + y;
					for (int x = 0; x < textWidth; x++) {
						Font::image[width * y2 + xx + x] = ((unsigned)bitmap[textWidth * y + x] << 24) | colors[colorIndex].color;
					}
				}
			}
			totalHeight2 += Font::heights[lineIndex];
		}
		advanceX += Font::charImages_t[i]->advanceX;
	}
}
void TextBox::PrepareDraw() {
	_PrepareDraw();
	if(IsBuild())Edit(Font::image, width, height);
	else Build(Font::image, width, height);
}