#include "Context.h"

#include "../_system/_Windows.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"


Context::Context() {
#ifdef OPENGL
#ifdef _DEBUG
	if (!System::IsRendererInited());
#endif
#ifdef _DEBUG
	const int attribList2[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, (int)_System::_OpenGL::version.majorVersion,
		WGL_CONTEXT_MINOR_VERSION_ARB, (int)_System::_OpenGL::version.minorVersion,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_CONTEXT_FLAGS_ARB ,WGL_CONTEXT_DEBUG_BIT_ARB ,0 };
#else
	const int attribList2[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, (int)_System::_OpenGL::version.majorVersion,
		WGL_CONTEXT_MINOR_VERSION_ARB, (int)_System::_OpenGL::version.minorVersion,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,0 };
#endif
	context = _System::_OpenGL::wglCreateContextAttribsARB(_System::_Windows::hdc, _System::_OpenGL::context, attribList2);

#elif VULKAN
#endif
}
Context::~Context() {
	//wglDeleteContext(_System::_OpenGL::context);
}
void Context::Link() {
	//wglMakeCurrent(_System::_Windows::hdc, context);
}
void Context::Unlink() {
	//wglMakeCurrent(nullptr, nullptr);
}