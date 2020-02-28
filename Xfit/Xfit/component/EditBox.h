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


	void _Draw(bool* _changePos, bool* _requireDraw);

public:
#ifdef __ANDROID__
	static inline bool isFinish = false;
	static inline std::wstring staticText;
	static inline EditBox* thisEditBox = nullptr;
#endif

	void Draw();
	void Size();

	void SetPos(PointF _pos);

	PointF baseScale;
	PointF basePos;
	PosType posType;
	std::wstring text;
	SizeLabel* label;
	EditBox(FontColor* _fontColor, FontContainer* _fontContainer, ScaleImage* _box, ScaleImage* _cursor, PointF _basePos, PointF _baseScale, unsigned _width, unsigned _textPixelSize);
	bool Update();



	~EditBox();
};