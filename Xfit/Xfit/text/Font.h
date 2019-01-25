#pragma once

#include "../object/Image.h"
#include "../math/Rect.h"
#include "../system/Error.h"

class FontError : public Error {
public:
	FontError( int _code) : Error( _code) {}
};
class Font {
	friend class Label;

	size_t len;

	struct CharImage {
		Tchar text;
		unsigned char* bitmap;
		long advanceX, advanceY;
		int left, top, bearingY;
		unsigned px, width, height;
	};

	void* rawData;
public:
	unsigned fontIndex;
	Font():len(0),fontIndex(0),rawData(nullptr){}

	static void Init() {
	}
	static void Release() {
	}
	bool sBold(unsigned index/* = 0*/)const {
	}
	bool IsItalic(unsigned index/* = 0*/)const {
	}
	~Font() {
		delete[] rawData;
	}
	const char* GetFamilyName(unsigned index/* = 0*/)const {
	}
	const char*GetStyleName(unsigned index/* = 0*/)const {
	}
	void Load(void * data, unsigned size) {
		Font(data, size);
	}
	Font(void * data, unsigned size):fontIndex(0) {
		rawData = data;
	}
	size_t GetFontNum() const { return len; }
};

class Label : public Image {
	unsigned textWidth, textHeight;
public:
	enum class Align {
		Left, Center, Right
	};
	enum VerticalAlign {
		Top, Middle, Bottom
	};
	unsigned px,color;

	RectF textBox;
	Tstring text;
	Font* font;
	Align align;
	RectF GetRect() const;
	VerticalAlign verticalAlign;
	unsigned GetTextWidth()const { return textWidth; }
	unsigned GetTextHeight()const { return textHeight; }
	virtual ~Label() { delete frame; }


	void PrepareDraw();
};