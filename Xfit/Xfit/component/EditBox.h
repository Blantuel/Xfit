#pragma once

#include "../text/SizeLabel.h"

#include "../object/ScaleImage.h"
#include "../resource/ShapeVertex.h"

class EditBox {
	bool focus;
#ifndef __ANDROID__
	unsigned prevCharsLen;
	int cursorPos;
	float dt;
#endif
	unsigned baseTextWidth;

	Image* textImage;
	ScaleImage* box;
#ifndef __ANDROID__
	ScaleImage* cursor;

	std::vector<int> textWidths;

	void EnterChar(bool* _changePos, bool* _requireDraw);
#endif
	PointF basePos;
	void _Draw(bool* _changePos, bool* _requireDraw);
public:
#ifdef __ANDROID__
	static inline bool isFinish = false;
	static inline bool isEnter = false;
	static inline std::wstring staticText;
	static inline EditBox* thisEditBox = nullptr;
#endif

	void Draw();
	void Size();

	void SetPos(PointF _pos);
	void SetX(float _x);
	void SetY(float _y);
	PointF GetPos()const;
	float GetX()const;
	float GetY()const;

	void SetWidth(unsigned _width);
	void SetTextPx(unsigned _px);

	unsigned GetTextPx()const;
	unsigned GetWidth()const;
	
	std::wstring text;
	SizeLabel* label;
	EditBox(FontColor* _fontColor, FontContainer* _fontContainer, ScaleImage* _box, ScaleImage* _cursor, PointF _basePos, unsigned _width, unsigned _textPixelSize);
	bool Update();

	void SetFocus(bool _focus);

	ScaleImage* GetBox()const;
	ScaleImage* GetCursor()const;

	~EditBox();
};