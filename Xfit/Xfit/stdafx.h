#pragma once

#include <random>
#include <cstring> 
#include <cwchar>
#include <chrono>
#include <atomic>
#include <mutex>
#include <typeinfo>
#include <sstream>
#include <thread>
#include <memory>


#if defined(_M_AMD64) || defined(_M_IX86) || defined(__amd64__) || defined(__i386__)

#if defined(_M_AMD64) || defined(__amd64__)

#define SSE4 1
//#define AVX2 1
//#define AVX512 1

#endif

#include <immintrin.h>

#elif defined(_M_ARM) || defined(__arm__) || defined(__thumb__)

#define NEON 1

#include <arm_neon.h>

#endif

#if defined(_M_AMD64) || defined(__amd64__)

#elif defined(_M_IX86) || defined(__i386__)

#elif defined(_M_ARM) || defined(__arm__) || defined(__thumb__)

#elif defined(_M_ARM64) || defined(__aarch64__)

#endif


#if defined(_M_AMD64) || defined(__amd64__) || defined(_M_ARM64) || defined(__aarch64__)
#define BIT64 1
#else
#define BIT32 1
#endif


#ifdef _WIN32

#include <windowsx.h>
#include <Ws2tcpip.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <io.h>

#ifndef _tWinMain
#ifdef UNICODE
#define _tWinMain wWinMain
#else
#define _tWinMain WinMain
#endif
#endif


#include <d3d11_4.h>
#include <d3d12.h>
#include <d3dcompiler.h>


#ifdef _DEBUG

#define PRINTMSG(...) {char* __str = new char[16384];sprintf_s(__str,16384,__VA_ARGS__);OutputDebugStringA(__str);delete[]__str;} 

#else

#define PRINTMSG(...) ((void)0)

#endif

#elif __linux__

#include <unistd.h>
#include <sys/resource.h>

#ifdef __ANDROID__

#include <jni.h>

#include <android/sensor.h>

#include <android/log.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <android/asset_manager.h>
#include <SLES/OpenSLES.h>

#ifdef OPENGL

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>

#elif VULKAN
#define VK_USE_PLATFORM_ANDROID_KHR 1
#endif

#ifdef _DEBUG

#define PRINTMSG(...) ((void)__android_log_print(ANDROID_LOG_INFO, "XfitMSG", __VA_ARGS__))

#else

#define PRINTMSG(...) ((void)0)

#endif

#endif

#endif

#include <ft2build.h>
#include FT_FREETYPE_H
#include "../../Libpng/png.h"
#include "../../Libjpeg/jpeglib.h"
#include "../../Libogg/vorbis/vorbisfile.h"
#include "../../Zlib/zlib.h"
#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/mux.h"
#include "webp/demux.h"

using namespace std;

#ifdef UNICODE
using Tchar = wchar_t;
using Tstring = wstring;
using Tstringstream = wstringstream;
using Tostringstream = wostringstream;

#ifndef _T
#define _T(_str) L##_str
#endif

#else

using Tchar = char;
using Tstring = string;
using Tstringstream = stringstream;
using Tostringstream = ostringstream;

#ifndef _T
#define _T(_str) _str
#endif

#endif

#undef max
#undef min

#ifdef AVX2
#define ALIGN alignas(32)
#else
#define ALIGN alignas(16)
#endif


template<typename T> void SafeRelease(T** ppT) {
	if (*ppT) {
		(*ppT)->Release();
		*ppT = nullptr;
	}
}