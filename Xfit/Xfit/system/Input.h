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
	
	static Tchar GetEnterChar();
	bool IsKeyPress(unsigned char _keyCode);
	bool IsKeyPressing(unsigned char _keyCode);
	bool IsKeyPressed(unsigned char _keyCode);

	
	short GetWheelScrolling();
	void ShowCursor(bool _show);
};

