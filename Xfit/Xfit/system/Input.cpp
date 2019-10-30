#include "Input.h"
#include "System.h"
#include "../_system/_Windows.h"
#include "../_system/_Android.h"


namespace Input {
#ifdef _WIN32
	Point GetMousePosScreen() {
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(_System::_Windows::hWnd, &p);
		return Point(p.x, p.y);
	}
	PointF GetMousePos() {
		const Point p = GetMousePosScreen();
		return PointF((float)p.x - (float)System::GetWindowWidth()/2.f, (float)-p.y + (float)System::GetWindowHeight() / 2.f);
	}
	bool IsLMouseClicked() { return _System::_Windows::click == 3; }
	bool IsLMouseClick() { return _System::_Windows::click == 1; }
	bool IsLMouseClicking() { return _System::_Windows::click == 2; }
	bool IsRMouseClicked() { return _System::_Windows::click2 == 3; }
	bool IsRMouseClick() { return _System::_Windows::click2 == 1; }
	bool IsRMouseClicking() { return _System::_Windows::click2 == 2; }
	bool IsWheelClicked() { return _System::_Windows::click3 == 3; }
	bool IsWheelClick() { return _System::_Windows::click3 == 1; }
	bool IsWheelClicking() { return _System::_Windows::click3 == 2; }
	wstring& GetChars() {
		return _System::_Windows::chars;
	}
	EnterCharState GetEnterCharState() {
		return _System::_Windows::enterCharState;
	}
	unsigned GetPrevCharsLen() {
		return _System::_Windows::prevCharsLen;
	}

	bool IsKeyPress(Key _keyCode) { return _System::_Windows::keyState[(unsigned char)_keyCode] == 1; }
	bool IsKeyPressing(Key _keyCode) { return _System::_Windows::keyState[(unsigned char)_keyCode] == 2 || _System::_Windows::keyState[(unsigned char)_keyCode] == 1; }
	bool IsKeyPressed(Key _keyCode) { return _System::_Windows::keyState[(unsigned char)_keyCode] == 3; }
	short GetWheelScrolling() { return _System::_Windows::zScroll; }
	void ShowCursor(bool _show) { ::ShowCursor(_show); }

	bool IsMouseOut() { return _System::_Windows::mouseOut; }
#elif __ANDROID__

	bool IsPointerClick(unsigned _index/* = 0*/) { 
		return _System::_Android::clicks[_index] == 1;
	}
	bool IsPointerClicking(unsigned _index/* = 0*/) {
		return _System::_Android::clicks[_index] == 2;
	}
	bool IsPointerClicked(unsigned _index/* = 0*/) {
		return _System::_Android::clicks[_index] == 3;
	}

	PointF GetPointPos(unsigned _index/* = 0*/) {
		return _System::_Android::poses[_index];
	}
#endif
}