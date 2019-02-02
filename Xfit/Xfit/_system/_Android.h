#pragma once

#ifdef __ANDROID__

#include "../system/System.h"

namespace _System::_Android {
	struct App {
		void (*onAppCmd)(int32_t cmd);

		int32_t(*onInputEvent)(AInputEvent* _event);
		ANativeActivity* activity;

		ALooper* looper;
		AInputQueue* inputQueue;
		ANativeWindow* window;
		int activityState;

		int msgread;
		int msgwrite;

		int destroyRequested;
		AInputQueue* pendingInputQueue;
		ANativeWindow* pendingWindow;

		bool inited;
	};
	struct Engine {
		int animating;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;
	};
	inline App app = {};
	inline Engine engine = {};

	void Create(ANativeActivity* activity);

	void WindowInit(System::WindowInfo* _info);

	inline System::ScreenMode screenMode;
	inline System::OSversion _osVersion;
}

#endif