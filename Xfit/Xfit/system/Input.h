#pragma once

#include "../math/Point.h"


enum class EnterCharState {
	Making,
	Finish,
	None
};

namespace Input {
	enum class Key : unsigned char {
		A = 'A', B = 'B', C = 'C', D = 'D', E = 'E', F = 'F', G = 'G', H = 'H', I = 'I', J = 'J', K = 'K', L = 'L', M = 'M', N = 'N', O = 'O', P = 'P', Q = 'Q', R = 'R', S = 'S', T = 'T', U = 'U', V = 'V', W = 'W', X = 'X', Y = 'Y', Z = 'Z'
		, One = '1', Two = '2', Three = '3', Four = '4', Five = '5', Six = '6', Seven = '7', Eight = '8', Nine = '9', Zero = '0',
		Numpad1 = VK_NUMPAD1, Numpad2 = VK_NUMPAD2, Numpad3 = VK_NUMPAD3, Numpad4 = VK_NUMPAD4, Numpad5 = VK_NUMPAD5,
		Numpad6 = VK_NUMPAD6, Numpad7 = VK_NUMPAD7, Numpad8 = VK_NUMPAD8, Numpad9 = VK_NUMPAD9, Numpad0 = VK_NUMPAD0,
		NumLock = VK_NUMLOCK, ScrollLock = VK_SCROLL, CapsLock = VK_CAPITAL,
		Numpad_Add = VK_ADD, Numpad_Subtract = VK_SUBTRACT, Numpad_Multiply = VK_MULTIPLY, Numpad_Divide = VK_DIVIDE, Numpad_Dot = VK_DECIMAL,
		Enter = VK_RETURN,//Numpad and Standard
		LShift = VK_LSHIFT, RShift = VK_RSHIFT, Shift = VK_SHIFT,
		Control = VK_CONTROL, LControl = VK_LCONTROL, RControl = VK_RCONTROL,
		Tap = VK_TAB,
		Esc = VK_ESCAPE,
		IMEProcess = VK_PROCESSKEY,
		Apps = VK_APPS,
		SpaceBar = VK_SPACE,
		F1 = VK_F1, F2 = VK_F2, F3 = VK_F3, F4 = VK_F4, F5 = VK_F5, F6 = VK_F6, F7 = VK_F7, F8 = VK_F8, F9 = VK_F9, F10 = VK_F10, F11 = VK_F11, F12 = VK_F12,
		Dot = VK_OEM_PERIOD,
		Comma = VK_OEM_COMMA,
		Plus = VK_OEM_PLUS,
		Minus = VK_OEM_MINUS,
		BackSlash = VK_OEM_5,
		Slash = VK_OEM_2,
		LSquareBracket = VK_OEM_4,
		RSquareBracket = VK_OEM_6,
		SwungDash = VK_OEM_3,
		Insert = VK_INSERT,
		Home = VK_HOME,
		PageUp = VK_PRIOR,
		PageDown = VK_NEXT,
		Delete = VK_DELETE,
		End = VK_END,
		Pause = VK_PAUSE,
		Semicolon = VK_OEM_1,
		Apostrophe = VK_OEM_7,
		Up = VK_UP,
		Down = VK_DOWN,
		Left = VK_LEFT,
		Right = VK_RIGHT
	};

	Point GetMousePosScreen();
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

#ifdef _WIN32
	bool IsMouseOut();
#endif

#ifdef __ANDROID__
	bool IsPointerClick(unsigned _index=0);
	bool IsPointerClicking(unsigned _index = 0);
	bool IsPointerClicked(unsigned _index = 0);

	PointF GetPointPos(unsigned _index = 0);
#endif
	
	wstring& GetChars();
	EnterCharState GetEnterCharState();
	unsigned GetPrevCharsLen();

	void ClearMakingChar();
	bool IsKeyPress(Key _keyCode);
	bool IsKeyPressing(Key _keyCode);
	bool IsKeyPressed(Key _keyCode);

	
	short GetWheelScrolling();
	void ShowCursor(bool _show);
};

