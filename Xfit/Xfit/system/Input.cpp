#include "Input.h"
#include "System.h"
#include "../_system/_Windows.h"
#include "../_system/_Android.h"


namespace Input {
#ifdef _WIN32
	PointF GetMousePos() {
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(_System::_Windows::hWnd, &p);
		PointF mousePos;
		mousePos.x = (float)p.x / (float)_System::_Windows::windowWidth*2.f - 1.f;
		mousePos.y = (float)p.y / (float)_System::_Windows::windowHeight*2.f - 1.f;
		return mousePos;
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
	Tchar GetEnterChar() { return _System::_Windows::tchar; }
	bool IsKeyPress(unsigned char _keyCode) { return _System::_Windows::keyState[_keyCode]==1; }
	bool IsKeyPressing(unsigned char _keyCode) { return _System::_Windows::keyState[_keyCode] == 2;}
	bool IsKeyPressed(unsigned char _keyCode) { return _System::_Windows::keyState[_keyCode] == 3;}
	short GetWheelScrolling() { return _System::_Windows::zScroll; }
	void ShowCursor(bool _show) { ::ShowCursor(_show); }
#elif __ANDROID__
#endif
}