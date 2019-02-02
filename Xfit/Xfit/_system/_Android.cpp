#ifdef __ANDROID__

#include "_Android.h"
#include "_Loop.h"

using namespace std;

thread mainThread;
mutex mainMutex;
condition_variable mainWait;


enum {
	LOOPER_ID_MAIN = 1,
	LOOPER_ID_INPUT = 2,
	LOOPER_ID_USER = 3,
};

enum {
	APP_CMD_INPUT_CHANGED,
	APP_CMD_INIT_WINDOW,
	APP_CMD_TERM_WINDOW,
	APP_CMD_GAINED_FOCUS,
	APP_CMD_LOST_FOCUS,
	APP_CMD_START,
	APP_CMD_RESUME,
	APP_CMD_PAUSE,
	APP_CMD_STOP,
	APP_CMD_DESTROY
};
bool writeWait = false;


GLuint buf[2];
GLuint vertexShader;
GLuint pixelShader;
GLuint program;


float vert[9];	// vertex array

EGLConfig config;
EGLint format;

GLuint frameBuf, renderBuf, texture;


namespace _System::_Android {
	void process_input();
	void app_pre_exec_cmd(int8_t cmd) {
		switch (cmd) {
		case APP_CMD_INPUT_CHANGED:
			if (!app.pendingInputQueue) {
				AInputQueue_detachLooper(app.inputQueue);
				app.inputQueue = nullptr;
			} else {
				app.inputQueue = app.pendingInputQueue;
				AInputQueue_attachLooper(app.inputQueue, app.looper, LOOPER_ID_INPUT, NULL, (void*)process_input);
			}
			break;
		case APP_CMD_INIT_WINDOW:
			app.window = app.pendingWindow;
			break;
		case APP_CMD_TERM_WINDOW:
			app.window = NULL;
			break;
		case APP_CMD_RESUME:
		case APP_CMD_START:
		case APP_CMD_PAUSE:
		case APP_CMD_STOP:
			app.activityState = cmd;
			break;
		case APP_CMD_DESTROY:
			app.destroyRequested = 1;
			break;
		}
	}
	void process_input() {
		AInputEvent* event = NULL;
		while (AInputQueue_getEvent(app.inputQueue, &event) >= 0) {
			if (AInputQueue_preDispatchEvent(app.inputQueue, event)) continue;
			int32_t handled = 0;
			if (app.onInputEvent != NULL) handled = app.onInputEvent(event);
			AInputQueue_finishEvent(app.inputQueue, event, handled);
		}
	}
	void process_cmd() {
		int8_t cmd;
		if (read(app.msgread, &cmd, sizeof(cmd)) != sizeof(cmd))return;

		mainMutex.lock();
		writeWait = true;
		mainWait.notify_one();
		mainMutex.unlock();

		app_pre_exec_cmd(cmd);
		app.onAppCmd(cmd);
	}
	void WriteCmd(int8_t cmd) {
		unique_lock<mutex> uniqueLock(mainMutex);
		write(app.msgwrite, &cmd, sizeof(cmd));
		while (!writeWait) mainWait.wait(uniqueLock);
		writeWait = false;
	}
	void onDestroy(ANativeActivity * activity) {
		WriteCmd(APP_CMD_DESTROY);
	}
	void onStart(ANativeActivity * activity) {
		WriteCmd(APP_CMD_START);
	}
	void onResume(ANativeActivity * activity) {
		WriteCmd(APP_CMD_RESUME);
	}
	void onPause(ANativeActivity * activity) {
		WriteCmd(APP_CMD_PAUSE);
	}
	void onStop(ANativeActivity * activity) {
		WriteCmd(APP_CMD_STOP);
	}
	void onWindowFocusChanged(ANativeActivity * activity, int focused) {
		WriteCmd(focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
	}
	void onNativeWindowCreated(ANativeActivity * activity, ANativeWindow * window) {
		app.pendingWindow = window;
		WriteCmd(APP_CMD_INIT_WINDOW);
	}
	void onNativeWindowDestroyed(ANativeActivity * activity, ANativeWindow * window) {
		WriteCmd(APP_CMD_TERM_WINDOW);
	}
	void onInputQueueCreated(ANativeActivity * activity, AInputQueue * queue) {
		app.pendingInputQueue = queue;
		WriteCmd(APP_CMD_INPUT_CHANGED);
	}
	void onInputQueueDestroyed(ANativeActivity * activity, AInputQueue * queue) {
		app.pendingInputQueue = nullptr;
		WriteCmd(APP_CMD_INPUT_CHANGED);
	}

	int32_t engine_handle_input(AInputEvent* _event) {
		if (AInputEvent_getType(_event) == AINPUT_EVENT_TYPE_MOTION) {
			return 1;
		}
		return 0;
	}
	void engine_handle_cmd(int32_t cmd);
	void Main() {
		ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
		ALooper_addFd(looper, app.msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL, (void*)process_cmd);
		app.looper = looper;

		memset(&engine, 0, sizeof(engine));
		app.onAppCmd = engine_handle_cmd;
		app.onInputEvent = engine_handle_input;

		engine.animating = 1;

		int ident;
		int events;
		void (*source)() = nullptr;

		while(true) {
			while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {
				if (source != nullptr) source();

				if (ident == LOOPER_ID_USER) {

				}
				if (app.destroyRequested)goto exitLabel;
			}
			if (engine.animating&& app.inited) {
				_System::_Loop::Loop();
			}
		}
	exitLabel:
		_System::_Loop::exited = true;
		System::destroyFunc();

		eglMakeCurrent(engine.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (app.inputQueue != NULL) AInputQueue_detachLooper(app.inputQueue);
		close(app.msgread);
		close(app.msgwrite);

		eglDestroyContext(engine.display, engine.context);
		eglTerminate(engine.display);

		_exit(0);
	}
	void Create(ANativeActivity* activity) {
		app.activity = activity;
		activity->callbacks->onDestroy = onDestroy;
		activity->callbacks->onStart = onStart;
		activity->callbacks->onResume = onResume;
		activity->callbacks->onPause = onPause;
		activity->callbacks->onStop = onStop;
		activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
		activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
		activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
		activity->callbacks->onInputQueueCreated = onInputQueueCreated;
		activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
		if (!app.inited) {
			int msgpipe[2];
			if (pipe(msgpipe)) return;

			app.msgread = msgpipe[0];
			app.msgwrite = msgpipe[1];

			mainThread = thread(Main);
		}
	}
	void WindowInit(System::WindowInfo* _info) {
		const EGLint attribs[] = {
			EGL_RENDERABLE_TYPE,EGL_OPENGL_ES3_BIT_KHR,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_ALPHA_SIZE,8,
			EGL_DEPTH_SIZE,24,
			EGL_STENCIL_SIZE,8,
			EGL_NONE
		};
		EGLint w, h;

		EGLint numConfigs;

		engine.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(engine.display, 0, 0);

		eglChooseConfig(engine.display, attribs, &config, 1, &numConfigs);

		int attrib_list[] = {
			EGL_CONTEXT_CLIENT_VERSION, 3,
			EGL_NONE
		};
		engine.context = eglCreateContext(engine.display, config, NULL, attrib_list);

		eglGetConfigAttrib(engine.display, config, EGL_NATIVE_VISUAL_ID, &format);
		ANativeWindow_setBuffersGeometry(app.window, 0, 0, format);

		engine.surface = eglCreateWindowSurface(engine.display, config, app.window, NULL);

		eglMakeCurrent(engine.display, engine.surface, engine.surface, engine.context);
		
		eglQuerySurface(engine.display, engine.surface, EGL_WIDTH, &w);
		eglQuerySurface(engine.display, engine.surface, EGL_HEIGHT, &h);
		engine.width = w;
		engine.height = h;
	}
	void engine_init_display() {
		EGLint w, h;

		ANativeWindow_setBuffersGeometry(app.window, 0, 0, format);

		engine.surface = eglCreateWindowSurface(engine.display, config, app.window, NULL);
		eglMakeCurrent(engine.display, engine.surface, engine.surface, engine.context);
		

		eglQuerySurface(engine.display, engine.surface, EGL_WIDTH, &w);
		eglQuerySurface(engine.display, engine.surface, EGL_HEIGHT, &h);

		engine.width = w;
		engine.height = h;
	}

	void engine_term_display() {
		eglMakeCurrent(engine.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroySurface(engine.display, engine.surface);

		engine.animating = 0;
		engine.surface = EGL_NO_SURFACE;
	}
	void engine_handle_cmd(int32_t cmd) {
		switch (cmd) {
		case APP_CMD_INIT_WINDOW:
			if (app.inited) {
				engine_init_display();
			} else {
				System::createFunc();
				app.inited = true;
			}
			break;
		case APP_CMD_TERM_WINDOW:
			engine_term_display();
			break;
		case APP_CMD_GAINED_FOCUS:
			engine.animating = 1;
			break;
		case APP_CMD_LOST_FOCUS:
			engine.animating = 0;
			break;
		}
	}
}



#endif