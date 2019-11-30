#include "Label.h"
#include "Font.h"
#include "../data/Memory.h"

unsigned Label::GetMaxScrollTextCount()const {
	return maxScrollTextCount;
}
Label::Label() :text(nullptr), fonts(nullptr), colors(nullptr), sizes(nullptr), renders(nullptr), maxScrollTextCount(0), scrollTextCount(0), charWidths(nullptr) {}
void Label::PrepareDraw() {
	unsigned len = wcslen(text);

	int top = INT_MIN;
	int width = 1, height = 1;
	int imageSize;
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

	unsigned renderIndex = 0;
	unsigned renderLenIndex = 0;
	unsigned renderLen;

	int padding = 0;

	if (sizes[0].len == 0)sizeLen = len;
	else sizeLen = sizes[0].len;

	if (fonts[0].len == 0)fontLen = len;
	else fontLen = fonts[0].len;

	if (colors[0].len == 0)colorLen = len;
	else colorLen = colors[0].len;

	if (renders[0].len == 0)renderLen = len;
	else renderLen = renders[0].len;

	Font::mutex.lock();
	for (size_t i = 0; i < len; i++, fontLenIndex++, sizeLenIndex++, renderLenIndex++) {
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
		if (renderLenIndex >= renderLen) {
			renderIndex++;
			if (renders[renderIndex].len == 0)renderLen = len - i;
			else renderLen = renders[renderIndex].len;

			renderLenIndex = 0;
		}
		if ((text[i] == L'\n'))continue;
		else if ((text[i] == L'\r')) {
			if (i < len - 1 && text[i + 1] == L'\n') {
				i++;//줄바꿈이 \r\n일 경우 한글자 더 건너뜀.
			}
			continue;
		}
		
		const auto face = fonts[fontIndex].font->face;
		if (currentSize != sizes[sizeIndex].pixelSize) {
			currentSize = sizes[sizeIndex].pixelSize;
			FT_Set_Pixel_Sizes(face, currentSize, 0);
		}
		const int heightT = face->size->metrics.height >> 6;
		if (height < heightT)height = heightT;

		size_t charImageIndex = SIZE_MAX;

		for (size_t j = 0; j < Font::charImages.Size(); j++) {
			if ((Font::charImages[j].text == text[i]) && (Font::charImages[j].pixelSize == currentSize) && (Font::charImages[j].font == fonts[fontIndex].font)
				&& (Font::charImages[j].renderType == renders[renderIndex].type)) {
				charImageIndex = j;
				break;
			}
		}
		if (charImageIndex == SIZE_MAX) {
			auto index = FT_Get_Char_Index(face, text[i]);


			if (Font::charImages.Size() >= Font::charImages.MaxSize())
				Font::charImages.ReAlloc(Font::charImages.MaxSize() + Font::charImageBlockLen);
			Font::charImages.Resize(Font::charImages.Size() + 1);

			Font::charImages_t[i] = &Font::charImages[Font::charImages.Size() - 1];
			Font::charImages_t[i]->renderType = renders[renderIndex].type;

			FT_GlyphSlot glyph = nullptr;
			if (Font::charImages_t[i]->renderType == FontRenderType::Normal) {
				FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP | FT_LOAD_TARGET_NORMAL);
				glyph = face->glyph;
				FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);

				imageSize = glyph->bitmap.width * glyph->bitmap.rows;

				if (imageSize == 0) Font::charImages_t[i]->bitmap = nullptr;
				else {
					Font::charImages_t[i]->bitmap = new unsigned char[imageSize];

					Memory::Copy(Font::charImages_t[i]->bitmap, imageSize, glyph->bitmap.buffer, imageSize);
				}

			} else if (Font::charImages_t[i]->renderType == FontRenderType::Mono) {
				FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP | FT_LOAD_TARGET_MONO);//FT_LOAD_MONOCHROME는  FT_LOAD_RENDER와 같이 쓰는 옵션이라 필요 없음.
				glyph = face->glyph;
				FT_Render_Glyph(glyph, FT_RENDER_MODE_MONO);

				imageSize = glyph->bitmap.pitch * glyph->bitmap.rows;

				if (imageSize == 0) Font::charImages_t[i]->bitmap = nullptr;
				else {
					Font::charImages_t[i]->bitmap = new unsigned char[imageSize];

					Memory::Copy(Font::charImages_t[i]->bitmap, imageSize, glyph->bitmap.buffer, imageSize);
				}

			} else if (Font::charImages_t[i]->renderType == FontRenderType::LCD) {
				FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP | FT_LOAD_TARGET_NORMAL);
				glyph = face->glyph;
				FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);

				imageSize = glyph->bitmap.width * glyph->bitmap.rows;

				if (imageSize == 0) Font::charImages_t[i]->bitmap = nullptr;
				else {
					Font::charImages_t[i]->bitmap = (unsigned char*)new unsigned[imageSize];

					Memory::Copy((unsigned*)Font::charImages_t[i]->bitmap, imageSize, (unsigned*)glyph->bitmap.buffer, imageSize);
				}
			} else {
			}

			Font::charImages_t[i]->width = glyph->bitmap.width;
			Font::charImages_t[i]->pitch = glyph->bitmap.pitch;
			Font::charImages_t[i]->height = glyph->bitmap.rows;

			Font::charImages_t[i]->text = text[i];
			Font::charImages_t[i]->pixelSize = currentSize;
			Font::charImages_t[i]->font = fonts[fontIndex].font;
			

			Font::charImages_t[i]->advanceX = glyph->advance.x >> 6;//하위 6비트는 버린다.
			if (glyph->advance.x & 0x3F)Font::charImages_t[i]->advanceX++;//26.6소수의 소숫점 아래 자리(6비트)가 0이상이면
			Font::charImages_t[i]->left = glyph->bitmap_left;
			Font::charImages_t[i]->top = glyph->bitmap_top;
			if (top < Font::charImages_t[i]->top)top = Font::charImages_t[i]->top;

			if (Font::charImages_t[i]->left < 0 && width < -Font::charImages_t[i]->left) {
				if(padding< -Font::charImages_t[i]->left)padding = -Font::charImages_t[i]->left;
			}
			width += Font::charImages_t[i]->advanceX;
		} else {
			Font::charImages_t[i] = &Font::charImages[charImageIndex];
			if (top < Font::charImages_t[i]->top)top = Font::charImages_t[i]->top;

			if (Font::charImages_t[i]->left < 0 && width < -Font::charImages_t[i]->left) {
				if (padding < -Font::charImages_t[i]->left)padding = -Font::charImages_t[i]->left;
			}
			width += Font::charImages_t[i]->advanceX;
		}
	}
	Font::mutex.unlock();
	width += padding;

	if (textWidth > 0) {
		unsigned textWidthG = 0;
		int e = (int)len;
		e--;
		for (; e >= 0; e--) {
			if (textWidthG + Font::charImages_t[e]->advanceX > textWidth) {
				maxScrollTextCount = e + 1;
				if (scrollTextCount > maxScrollTextCount)scrollTextCount = maxScrollTextCount;
				break;
			}
			textWidthG += Font::charImages_t[e]->advanceX;
		}
		if (e == -1) {
			maxScrollTextCount = 0;
			scrollTextCount = maxScrollTextCount;
		}
		width = textWidth;
	}

	imageSize = width * height;
	bool refresh = false;
	while (imageSize > Font::imageSize) {
		Font::imageSize = imageSize + Font::blockImageSize;
		refresh = true;
	}
	if (refresh) {
		delete[]Font::image;

		Font::image = new unsigned[Font::imageSize];
	}
	Memory::Set(Font::image, 0U, (size_t)imageSize);


	int advanceX = 0;
	
	if (charWidths) {
		for (unsigned i = 0; i < scrollTextCount; i++) {
			charWidths[i] = -1;
		}
	}

	unsigned i;
	for (i = scrollTextCount; i < len; i++, colorLenIndex++) {
		if (colorLenIndex >= colorLen) {
			colorIndex++;
			if (colors[colorIndex].len == 0)colorLen = len - i;
			else colorLen = colors[colorIndex].len;

			colorLenIndex = 0;
		}
		if ((text[i] == L'\n'))continue;
		else if ((text[i] == L'\r')) {
			if (i < len - 1 && text[i + 1] == L'\n') {
				i++;//줄바꿈이 \r\n일 경우 한글자 더 건너뜀.
			}
			continue;
		}

		if (Font::charImages_t[i]->bitmap) {
			const int xx = advanceX + Font::charImages_t[i]->left+padding;
			int textWidth = Font::charImages_t[i]->width;
			if (xx >= width)break;
			else if (xx + textWidth >= width) {
				textWidth = width - xx;
			}

			const int yy = top - Font::charImages_t[i]->top;
			const int textPitch = Font::charImages_t[i]->pitch;
			const int textHeight = Font::charImages_t[i]->height;

			if (Font::charImages_t[i]->renderType == FontRenderType::Normal) {
				const unsigned char* const bitmap = Font::charImages_t[i]->bitmap;
				for (int y = 0; y < textHeight; y++) {
					const int y2 = yy + y;
					for (int x = 0; x < textWidth; x++) {
						Font::image[width * y2 + xx + x] = ((unsigned)bitmap[textPitch * y + x] << 24) | colors[colorIndex].color;
					}
				}

			} else if (Font::charImages_t[i]->renderType == FontRenderType::Mono) {
				const unsigned char* const bitmap = Font::charImages_t[i]->bitmap;
				for (int y = 0; y < textHeight; y++) {
					int x;
					const int y2 = yy + y;
					const unsigned width2 = textWidth >> 3;
					for (x = 0; x < width2; x++) {
						const unsigned char bit = bitmap[textPitch * y + x];
						for (unsigned char a = 0; a < 8; a++) {
							Font::image[width * y2 + xx + x * 8 + a] = (bit & (128 >> a) ? 0xff000000U : 0U) | colors[colorIndex].color;
						}
						
					}
					const int remain = textWidth - (width2 << 3);
					const unsigned char bit = bitmap[textPitch * y + x];
					for (unsigned char a = 0; a < remain; a++) {
						Font::image[width * y2 + xx + x * 8 + a] = (bit & (128 >> a) ? 0xff000000U : 0U) | colors[colorIndex].color;
					}
				}

			} else if (Font::charImages_t[i]->renderType == FontRenderType::LCD) {
			} else {

			}
		} else {
			if (advanceX + Font::charImages_t[i]->advanceX + padding >= width)break;
		}
		advanceX += Font::charImages_t[i]->advanceX;
		if (charWidths) {
			if (advanceX <= width) {
				charWidths[i] = Font::charImages_t[i]->advanceX;
			} else charWidths[i] = -1;
		}
	}
	if (charWidths) {
		for (; i < len; i++) {
			charWidths[i] = -1;
		}
	}
	
	if (IsBuild())Delete();

	

	Build(Font::image, (unsigned)width, (unsigned)height);
}