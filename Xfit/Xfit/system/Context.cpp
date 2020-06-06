#include "Context.h"

#include "../_system/_Windows.h"
#include "../_system/_DirectX11.h"
#include "../_system/_Renderer.h"
#include "../_system/_Android.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"

#ifdef _WIN32
Context::Context() {
//#ifdef OPENGL
//#ifdef _DEBUG
//	if (!System::IsRendererInited());
//#endif
//#ifdef _DEBUG
//	const int attribList2[] = {
//		WGL_CONTEXT_MAJOR_VERSION_ARB, (int)_System::_OpenGL::version.majorVersion,
//		WGL_CONTEXT_MINOR_VERSION_ARB, (int)_System::_OpenGL::version.minorVersion,
//		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
//		WGL_CONTEXT_FLAGS_ARB ,WGL_CONTEXT_DEBUG_BIT_ARB ,0 };
//#else
//	const int attribList2[] = {
//		WGL_CONTEXT_MAJOR_VERSION_ARB, (int)_System::_OpenGL::version.majorVersion,
//		WGL_CONTEXT_MINOR_VERSION_ARB, (int)_System::_OpenGL::version.minorVersion,
//		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,0 };
//#endif
//	context = _System::_OpenGL::wglCreateContextAttribsARB(_System::_Windows::hdc, _System::_OpenGL::context, attribList2);
//
//#elif VULKAN
//#endif
	if (_System::_DirectX11::context1) {
		_System::_DirectX11::device1->CreateDeferredContext1(0, &context1);
		context = nullptr;
	} else {
		_System::_DirectX11::device->CreateDeferredContext(0, &context);
		context1 = nullptr;
	}
	commandList = nullptr;
}
Context::~Context() {
	if (context1) {
		context1->Release();
	} else {
		context->Release();
	}
	//wglDeleteContext(_System::_OpenGL::context);
}
void Context::Link() {
	_System::_DirectX11::contextT = context;
	_System::_DirectX11::context1T = context1;

	//wglMakeCurrent(_System::_Windows::hdc, context);
}
void Context::Finish() {
	HRESULT hr;
	if (context1) {
		hr = context1->FinishCommandList(FALSE, &commandList);
	} else {
		hr = context->FinishCommandList(FALSE, &commandList);
	}
}

void Context::Unlink() {
	//wglMakeCurrent(nullptr, nullptr);
	_System::_DirectX11::contextT = nullptr;
	_System::_DirectX11::context1T = nullptr;
}
void Context::Execute() {
	if (commandList) {
		_System::_DirectX11::context->ExecuteCommandList(commandList, FALSE);
		commandList->Release();
		commandList = nullptr;
	}
}

void Context::GlobalLock() {}
void Context::GlobalUnlock() {}

#elif __ANDROID__

Context::Context() {
    int attrib_list[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };
   context = eglCreateContext(_System::_Android::engine.display, _System::_Android::engine.config, _System::_Android::engine.context, attrib_list);
}
Context::~Context() {
    eglDestroyContext(_System::_Android::engine.display, context);
}
void Context::Link() {
    _System::_Android::contextT = context;
    //
}
void Context::Finish() {
}

void Context::Unlink() {
    _System::_Android::contextT = nullptr;
    //eglMakeCurrent(_System::_Android::engine.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

void Context::Execute() {
}
void Context::GlobalLock() {
    _System::_Android::threadMutex.lock();
    eglMakeCurrent(_System::_Android::engine.display, _System::_Android::engine.surface, _System::_Android::engine.surface,  _System::_Android::engine.context);
}

void Context::GlobalUnlock() {
    eglMakeCurrent(_System::_Android::engine.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    _System::_Android::threadMutex.unlock();
}

#endif