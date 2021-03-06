#pragma once


#ifdef __ANDROID__

#include "../system/System.h"
#include "../system/Input.h"



namespace _System::_Android {

	struct engine {
		struct android_app* app;

		ASensorManager* sensorManager;
		const ASensor* accelerometerSensor;
		ASensorEventQueue* sensorEventQueue;

		int animating;
		EGLDisplay display;
		EGLConfig config;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;
	};

	inline thread_local EGLContext contextT = nullptr;

	inline engine engine = {};

	inline unsigned char clicks[10];
	inline PointF poses[10];

	void Init(System::CreateInfo* _info);

	inline System::ScreenMode screenMode;
	inline System::OSversion _osVersion;

	inline int zScroll;
    inline std::wstring chars;
    inline EnterCharState enterCharState;
    inline unsigned char keyState[300] = {};
    inline unsigned char keyDownUpState[300] = {};
    inline unsigned prevCharsLen = 0;

	inline bool playingIME = false;
	inline std::mutex threadMutex;

	inline void Lock() {
		if (_System::_Android::contextT) {
			threadMutex.lock();
			eglMakeCurrent(_System::_Android::engine.display, _System::_Android::engine.surface, _System::_Android::engine.surface, _System::_Android::contextT);
		}
	}
	inline void Unlock() {
		if(_System::_Android::contextT) {
			eglMakeCurrent(_System::_Android::engine.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			threadMutex.unlock();
		}
	}

}

#endif