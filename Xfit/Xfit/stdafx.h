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
#include <cstdlib>
#include <codecvt>
#include <array>
#include <regex>


#if defined(_M_AMD64) || defined(_M_IX86) || defined(__amd64__) || defined(__i386__)

#if defined(_M_AMD64) || defined(__amd64__)

//#define SSE4 1
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
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <io.h>
#include <tchar.h>

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


#define PRINTMSG(...) {char* __str = new char[2000];sprintf_s(__str,2000,__VA_ARGS__);OutputDebugStringA(__str);delete[]__str;} 


#elif __linux__

	#include <unistd.h>
	#include <sys/resource.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
    #include <sys/ioctl.h>
    #include <net/if.h>
    #include <ifaddrs.h>
	#include <dlfcn.h>

	#ifdef __ANDROID__

		#include <jni.h>

		#include <android/sensor.h>

		#include <android/log.h>

		#include <android/configuration.h>
		#include <android/looper.h>
		#include <android/native_activity.h>
		#include <android/asset_manager.h>
		#include <android/keycodes.h>
		#include <SLES/OpenSLES.h>

		#include <EGL/egl.h>
		#include <EGL/eglext.h>
		#include <GLES3/gl3.h>
		#include <GLES3/gl3ext.h>
		#include <GLES3/gl31.h>
		#include <GLES3/gl32.h>

		#include <android_native_app_glue.h>

		#define VK_USE_PLATFORM_ANDROID_KHR 1

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

#ifndef __ANDROID__
#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/mux.h"
#include "webp/demux.h"
#endif

#ifdef UNICODE
using Tchar = wchar_t;
using Tstring = std::wstring;
using Tstringstream = std::wstringstream;
using Tostringstream = std::wostringstream;

#ifndef _T
#define _T(_str) L##_str
#endif

#else

using Tchar = char;
using Tstring = std::string;
using Tstringstream = std::stringstream;
using Tostringstream = std::ostringstream;

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