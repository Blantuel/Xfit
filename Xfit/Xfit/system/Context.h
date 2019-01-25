#pragma once

#ifdef _WIN32
#include "../_system/_Windows.h"
#elif __ANDROID__
#endif

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif


class Context {
public:
	static void Create() {
		if (System::GetCurrentRendererVersion().name == System::RendererName::OpenGL) {
#ifdef _DEBUG
			if (!System::IsRendererInited());
			if (_System::_OpenGL::threadContext);
#endif
			_System::_OpenGL::contextLinked = false;
#ifdef _DEBUG
			const int attribList2[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, (int)_System::_OpenGL::version.majorVersion,
				WGL_CONTEXT_MINOR_VERSION_ARB, (int)_System::_OpenGL::version.minorVersion,
				WGL_CONTEXT_FLAGS_ARB ,WGL_CONTEXT_DEBUG_BIT_ARB ,0 };
#else
			const int attribList2[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, (int)_System::_Renderer::version.majorVersion,
				WGL_CONTEXT_MINOR_VERSION_ARB, (int)_System::_Renderer::version.minorVersion,0 };
#endif
			_System::_OpenGL::threadContext = _System::_OpenGL::wglCreateContextAttribsARB(_System::_Windows::hdc, _System::_OpenGL::context, attribList2);
		} else if (System::GetCurrentRendererVersion().name == System::RendererName::Vulkan) {
		}
	}
	static void Link() {
		if (System::GetCurrentRendererVersion().name == System::RendererName::OpenGL) {
#ifdef _DEBUG
			if (!System::IsRendererInited());
			if (!_System::_OpenGL::threadContext);
			if (_System::_OpenGL::contextLinked);
#endif
			_System::_OpenGL::contextLinked = true;
			wglMakeCurrent(_System::_Windows::hdc, _System::_OpenGL::threadContext);
		} else if (System::GetCurrentRendererVersion().name == System::RendererName::Vulkan) {
		}
	}
	static bool IsLinked() {
		return _System::_OpenGL::contextLinked;
	}
	static void Unlink() {
		if (System::GetCurrentRendererVersion().name == System::RendererName::OpenGL) {
#ifdef _DEBUG
			if (!System::IsRendererInited());
			if (!_System::_OpenGL::threadContext);
			if (!_System::_OpenGL::contextLinked);
#endif
			_System::_OpenGL::contextLinked = false;
			wglMakeCurrent(nullptr,nullptr);
		} else if (System::GetCurrentRendererVersion().name == System::RendererName::Vulkan) {
		}
	}
	static void Delete() {
		if (System::GetCurrentRendererVersion().name == System::RendererName::OpenGL) {
#ifdef _DEBUG
			if (!System::IsRendererInited());
			if (!_System::_OpenGL::threadContext);
			if (_System::_OpenGL::threadContext == _System::_OpenGL::context);
			if (_System::_OpenGL::contextLinked);
#endif
			wglDeleteContext(_System::_OpenGL::threadContext);
		} else if (System::GetCurrentRendererVersion().name == System::RendererName::Vulkan) {
		}
	}
};