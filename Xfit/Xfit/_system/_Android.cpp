#ifdef __ANDROID__

#include "_Android.h"
#include "_Loop.h"

#include "_OpenGL.h"
#include "_Renderer.h"

using namespace std;

static EGLConfig config;
static EGLint format;

namespace _System::_Android {
	int getUnicodeChar(int keyCode) {
		JavaVM* javaVM = engine.app->activity->vm;
		JNIEnv* jniEnv = engine.app->activity->env;

		javaVM->AttachCurrentThread(&jniEnv, NULL);

		jclass KeyEventClass = jniEnv->FindClass("android/view/KeyEvent");
		jmethodID GetUnicodeCharMethod = jniEnv->GetMethodID(KeyEventClass, "getUnicodeChar", "()I");
		jmethodID eventConstructor = jniEnv->GetMethodID(KeyEventClass, "<init>", "(II)V");
		jobject eventObj = jniEnv->NewObject(KeyEventClass, eventConstructor, AINPUT_EVENT_TYPE_KEY, keyCode);

		int result = jniEnv->CallIntMethod(eventObj, GetUnicodeCharMethod);
		javaVM->DetachCurrentThread();

		return result;
	}


	void Init(System::CreateInfo* _info) {
		_System::_OpenGL::Init(_info);
	}
}


using namespace _System::_Android;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

/**
 * Shared state for our app.
 */


void makeWindow() {
    ANativeWindow_setBuffersGeometry(engine.app->window, 0, 0, format);

    engine.surface = eglCreateWindowSurface(engine.display, config, engine.app->window, NULL);
    eglMakeCurrent(engine.display, engine.surface, engine.surface, engine.context);

    int w,h;

    eglQuerySurface(engine.display, engine.surface, EGL_WIDTH, &w);
    eglQuerySurface(engine.display, engine.surface, EGL_HEIGHT, &h);

    _System::_Renderer::windowSize.width = w;
    _System::_Renderer::windowSize.height = h;

    engine.width = w;
    engine.height = h;
}

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h;

    EGLint numConfigs;

    engine->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    EGLint major;
    EGLint minor;
    eglInitialize(engine->display, &major, &minor);

    eglChooseConfig(engine->display, attribs, &config, 1, &numConfigs);

    int attrib_list[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };
    engine->context = eglCreateContext(engine->display, config, NULL, attrib_list);

    eglGetConfigAttrib(engine->display, config, EGL_NATIVE_VISUAL_ID, &format);


    makeWindow();

    // Check openGL on the system
    auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
    for (auto name : opengl_info) {
        auto info = glGetString(name);
        LOGI("OpenGL Info: %s", info);
    }

    glDisable(GL_DEPTH_TEST);


    return 0;
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display() {
    if (engine.display != EGL_NO_DISPLAY) {
        eglMakeCurrent(_System::_Android::engine.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (engine.surface != EGL_NO_SURFACE) {
            eglDestroySurface(_System::_Android::engine.display, _System::_Android::engine.surface);
        }

    }
}

/**
 * Process the next input event.
 */

static bool keyboardToggleVisible = false;

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	const auto eventType = AInputEvent_getType(event);
	if (eventType == AINPUT_EVENT_TYPE_MOTION) {
		const auto action = AMotionEvent_getAction(event);
		const auto actionType = action & AMOTION_EVENT_ACTION_MASK;
		if (actionType == AMOTION_EVENT_ACTION_DOWN) {
			clicks[0] = 1;
		} else if (actionType == AMOTION_EVENT_ACTION_UP) {
			_System::_Android::clicks[0] = 3;
		} else if (actionType == AMOTION_EVENT_ACTION_POINTER_DOWN) {
			const auto pointerId = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
			if(pointerId<10)_System::_Android::clicks[pointerId] = 1;
		} else if (actionType == AMOTION_EVENT_ACTION_POINTER_UP) {
			const auto pointerId = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
			if (pointerId < 10)clicks[pointerId] = 3;
		} else if(actionType == AMOTION_EVENT_ACTION_SCROLL) {
			zScroll = (int)(AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_VSCROLL, 0) * 100.f);
		}
		for (unsigned i = 0; i < 10; i++) {
			poses[i].x = (float)AMotionEvent_getX(event, i);
			poses[i].y = (float)AMotionEvent_getY(event, i);
		}
		return 1;

	} else if(eventType == AINPUT_EVENT_TYPE_KEY) {
		const auto action = AKeyEvent_getAction(event);
		const auto keyCode = AKeyEvent_getKeyCode(event);
		switch(action) {
			case AKEY_EVENT_ACTION_DOWN:
				if (keyState[keyCode] == 0)keyState[keyCode] = 1;
				keyDownUpState[keyCode] = 1;


				if(playingIME) {
					if(keyCode == (int)Input::Key::BackSpace) {
						chars += L'\b';
						enterCharState = EnterCharState::Finish;
					} else {
						int result = getUnicodeChar(keyCode);
						if(result > 0) {
							chars += (wchar_t)result;
							enterCharState = EnterCharState::Finish;
						}
					}
				}
				break;
			case AKEY_EVENT_ACTION_UP:
				keyState[keyCode] = 3;
				keyDownUpState[keyCode] = 2;
				break;
		}
	}
	return 0;
}

static bool inited = false;

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine.app->window != NULL) {
				if (!inited) {
                    engine_init_display(&engine);
					System::createFunc();
					inited = true;
				} else {
                    makeWindow();
				}
                //engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display();
            break;
        case APP_CMD_GAINED_FOCUS:
            engine.animating = 1;
			if(System::activateFunc)System::activateFunc();
            break;
        case APP_CMD_LOST_FOCUS:
            // Also stop animating.
            engine.animating = 0;
			if(System::activateFunc)System::activateFunc();
            //engine_draw_frame(engine);
            break;
    }
}

void Main(struct android_app* state) {
    memset(&_System::_Android::engine, 0, sizeof(_System::_Android::engine));
    state->userData = &_System::_Android::engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
	_System::_Android::engine.app = state;


    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

		for (unsigned i = 0; i < 10; i++) {
			if (_System::_Android::clicks[i] == 1)_System::_Android::clicks[i] = 2;
			else if (_System::_Android::clicks[i] == 3)_System::_Android::clicks[i] = 0;
		}
		for (unsigned i = 0; i < 256; i++) {
			if (_System::_Android::keyState[i] == 1)_System::_Android::keyState[i] = 2;
			else if (_System::_Android::keyState[i] == 3)_System::_Android::keyState[i] = 0;
			_System::_Android::keyDownUpState[i] = 0;
		}
		_System::_Android::zScroll = 0;
		_System::_Android::prevCharsLen = 0;
		_System::_Android::chars.clear();
		_System::_Android::enterCharState = EnterCharState::None;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                                      (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }
        }

        if (engine.animating) {
			_System::_Loop::Loop();
        }
        if (state->destroyRequested != 0) {
            goto exitLabel;
        }
    }
    exitLabel:;

    _System::_Loop::exited = true;
    System::destroyFunc();

    eglMakeCurrent(engine.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    if (engine.app->inputQueue != NULL) AInputQueue_detachLooper(engine.app->inputQueue);
    close(engine.app->msgread);
    close(engine.app->msgwrite);

    eglDestroyContext(_System::_Android::engine.display, _System::_Android::engine.context);
    eglTerminate(_System::_Android::engine.display);
    _exit(0);
}

#endif