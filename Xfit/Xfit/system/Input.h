#pragma once

#include "../math/Point.h"

namespace Input {
	PointF GetMousePos();
	bool IsLMouseClicked();
	bool IsLMouseClick();
	bool IsLMouseClicking();
	bool IsRMouseClicked();
	bool IsRMouseClick();
	bool IsRMouseClicking();
	bool IsWheelClicked();
	bool IsWheelClick();
	bool IsWheelClicking();

#ifdef __ANDROID__
	bool IsPointerClick(unsigned _index=0);
	bool IsPointerClicking(unsigned _index = 0);
	bool IsPointerClicked(unsigned _index = 0);

	PointF GetPointPos(unsigned _index = 0);
#endif
	
	static Tchar GetEnterChar();
	bool IsKeyPress(unsigned char _keyCode);
	bool IsKeyPressing(unsigned char _keyCode);
	bool IsKeyPressed(unsigned char _keyCode);

	
	short GetWheelScrolling();
	void ShowCursor(bool _show);
};

