#include "Input.h"
#include "System.h"
#include "../_system/_Windows.h"
#include "../_system/_Android.h"
#include "../object/ScaleHeader.h"

namespace Input {
#ifdef _WIN32
	PointF GetMousePosScreen() {
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(_System::_Windows::hWnd, &p);
		return PointF(p.x, p.y);
	}
	PointF GetMousePos() {
		const PointF p = GetMousePosScreen();
		return PointF(MouseXToX(p.x), MouseYToY(p.y));
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
	std::wstring& GetChars() {
		return _System::_Windows::chars;
	}
	void IMEStart() {

	}
	void IMEFinish() {
		
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
	bool IsKeyDown(Key _keyCode) { return _System::_Windows::keyDownUpState[(unsigned char)_keyCode] == 1; }
	bool IsKeyUp(Key _keyCode) { return _System::_Windows::keyDownUpState[(unsigned char)_keyCode] == 2; }
	int GetWheelScrolling() { return _System::_Windows::zScroll; }
	void ShowCursor(bool _show) { ::ShowCursor(_show); }

	bool IsMouseOut() { return _System::_Windows::mouseOut; }
	void ShowKeyboard(bool _show){}
#elif __ANDROID__
	bool IsMouseOut() {return false;}
	bool IsPointerClick(unsigned _index/* = 0*/) { 
		return _System::_Android::clicks[_index] == 1;
	}
	bool IsPointerClicking(unsigned _index/* = 0*/) {
		return _System::_Android::clicks[_index] == 2;
	}
	bool IsPointerClicked(unsigned _index/* = 0*/) {
		return _System::_Android::clicks[_index] == 3;
	}
    PointF GetPointPosScreen(unsigned _index/* = 0*/) {
	    return _System::_Android::poses[_index];
	}
	PointF GetPointPos(unsigned _index/* = 0*/) {
	    PointF pos = _System::_Android::poses[_index];

		return PointF(MouseXToX(pos.x), MouseYToY(pos.y));
	}
	PointF GetMousePos() {
		return GetPointPos();
	}
    PointF GetMousePosScreen() {
        return GetPointPosScreen();
	}
	bool IsLMouseClicked() { return IsPointerClicked(); }
	bool IsLMouseClick() { return IsPointerClick(); }
	bool IsLMouseClicking() { return IsPointerClicking(); }

	int GetWheelScrolling() {
        return _System::_Android::zScroll;
	}
	void ShowKeyboard(bool _show, std::wstring _text) {
		JavaVM* javaVM = _System::_Android::engine.app->activity->vm;
		JNIEnv* jniEnv = _System::_Android::engine.app->activity->env;


		javaVM->AttachCurrentThread(&jniEnv, nullptr);

		// Retrieves NativeActivity.
		jobject lNativeActivity = _System::_Android::engine.app->activity->clazz;
		jclass ClassNativeActivity = jniEnv->GetObjectClass(lNativeActivity);

		if(_show) {
            jmethodID ShowKeyboard = jniEnv->GetMethodID(ClassNativeActivity, "ShowKeyboard", "(Ljava/lang/String;)V");
            jstring str;
            if(_text == L"") {
				str = jniEnv->NewStringUTF("");
            } else {
				str = jniEnv->NewStringUTF(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(_text).c_str());
            }

            jniEnv->CallVoidMethod(lNativeActivity, ShowKeyboard, str);
		} else {
            jmethodID HideKeyboard = jniEnv->GetMethodID(ClassNativeActivity, "HideKeyboard", "()Ljava/lang/String;");

            jniEnv->CallObjectMethod(lNativeActivity, HideKeyboard);
		}

		// Finished with the JVM.
		javaVM->DetachCurrentThread();
	}

	bool IsKeyPress(Key _keyCode) { return _System::_Android::keyState[(int)_keyCode] == 1; }
	bool IsKeyPressing(Key _keyCode) { return _System::_Android::keyState[(int)_keyCode] == 2 || _System::_Android::keyState[(int)_keyCode] == 1; }
	bool IsKeyPressed(Key _keyCode) { return _System::_Android::keyState[(int)_keyCode] == 3; }
	bool IsKeyDown(Key _keyCode) { return _System::_Android::keyDownUpState[(int)_keyCode] == 1; }
	bool IsKeyUp(Key _keyCode) { return _System::_Android::keyDownUpState[(int)_keyCode] == 2; }

	void IMEStart() {
		_System::_Android::playingIME = true;
	}
	void IMEFinish() {
		_System::_Android::playingIME = false;
	}

	std::wstring& GetChars() {
		return _System::_Android::chars;
	}
	EnterCharState GetEnterCharState() {
		return _System::_Android::enterCharState;
	}

	unsigned GetPrevCharsLen() {
		return _System::_Android::prevCharsLen;
	}
#endif
}