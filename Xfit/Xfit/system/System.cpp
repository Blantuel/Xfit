#include "../object/Object.h"
#include "../resource/Sampler.h"
#include "../effect/Blend.h"
#include "../effect/Light.h"
#include "../_system/_Loop.h"
#include "../_system/_Renderer.h"

#include "../_system/_Windows.h"
#include "../_system/_Android.h"

#include "../_system/_GraphicsBase.h"

#include "../_system/_OpenGL.h"
#include "../_system/_DirectX11.h"
#include "../_system/_Vulkan.h"

#include "../_system/_DXGI.h"

#include "../time/Time.h"
#include "../resource/ShapeVertex.h"
#include "../resource/Sampler.h"
#include "../resource/Index.h"
#include "../object/ScaleHeader.h"


namespace System {
#ifdef _WIN32
	void Create(HINSTANCE _hInstance) {
		_System::_Windows::hInstance = _hInstance;

		Time::Init();

		_System::_Windows::Create();

		System::createFunc();

		while (true) {
			if (!_System::_Windows::Loop())break;
			_System::_Loop::Loop();
		}
		System::destroyFunc();

		if (defaultVertex2D) {
			defaultVertex2D->vertices.Free();
			delete defaultVertex2D;
		}
		if (rectShapeVertex2D) {
			rectShapeVertex2D->vertices.Free();
			delete rectShapeVertex2D;
		}
		if (ObjectVertex::topRight) {
			ObjectVertex::topRight->vertices.Free();
			delete ObjectVertex::topRight;
		}
		if (ObjectVertex::topLeft) {
			ObjectVertex::topLeft->vertices.Free();
			delete ObjectVertex::topLeft;
		}
		if (ObjectVertex::bottomRight) {
			ObjectVertex::bottomRight->vertices.Free();
			delete ObjectVertex::bottomRight;
		}
		if (ObjectVertex::bottomLeft) {
			ObjectVertex::bottomLeft->vertices.Free();
			delete ObjectVertex::bottomLeft;
		}
		if (ObjectVertex::right) {
			ObjectVertex::right->vertices.Free();
			delete ObjectVertex::right;
		}
		if (ObjectVertex::left) {
			ObjectVertex::left->vertices.Free();
			delete ObjectVertex::left;
		}
		if (ObjectVertex::top) {
			ObjectVertex::top->vertices.Free();
			delete ObjectVertex::top;
		}
		if (ObjectVertex::bottom) {
			ObjectVertex::bottom->vertices.Free();
			delete ObjectVertex::bottom;
		}

		if (defaultUV) {
			defaultUV->vertices.Free();
			delete defaultUV;
		}
		if (reverseUV) {
			reverseUV->vertices.Free();
			delete reverseUV;
		}
		if (defaultIndex) {
			defaultIndex->indices.Free();
			delete defaultIndex;
		}
		if (defaultSampler)delete defaultSampler;
		if (pointSampler)delete pointSampler;
		if (defaultBlend)delete defaultBlend;

		_System::_DXGI::Release();
		_System::_DirectX11::Release();
		//_System::_OpenGL::Release();
		_System::_Windows::Release();
	}
#elif __ANDROID__

#endif

	void Init(CreateInfo* _info) {
#ifdef _WIN32
		_System::_Windows::Init(_info);
#elif __ANDROID__
		_System::_Android::Init(_info);
#endif

		_System::_Loop::maxFrame = _info->maxFrame;

		defaultVertex2D = new Vertex;
		defaultVertex2D->vertices.Alloc(4);
		defaultVertex2D->vertices.InsertLast(PointF(-0.5f, 0.5f));
		defaultVertex2D->vertices.InsertLast(PointF(0.5f, 0.5f));
		defaultVertex2D->vertices.InsertLast(PointF(0.5f, -0.5f));
		defaultVertex2D->vertices.InsertLast(PointF(-0.5f, -0.5f));
		defaultVertex2D->Build();

		rectShapeVertex2D = new ShapeVertex;
		rectShapeVertex2D->vertices.Alloc(4);
		rectShapeVertex2D->vertices.InsertLast(PointF(-0.5f, 0.5f));
		rectShapeVertex2D->vertices.InsertLast(PointF(0.5f, 0.5f));
		rectShapeVertex2D->vertices.InsertLast(PointF(0.5f, -0.5f));
		rectShapeVertex2D->vertices.InsertLast(PointF(-0.5f, -0.5f));
		rectShapeVertex2D->Build();


		ObjectVertex::topRight = new Vertex;
		ObjectVertex::topRight->vertices.Alloc(4);
		ObjectVertex::topRight->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::TopRight);
		ObjectVertex::topRight->Build();

		ObjectVertex::topLeft = new Vertex;
		ObjectVertex::topLeft->vertices.Alloc(4);
		ObjectVertex::topLeft->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::TopLeft);
		ObjectVertex::topLeft->Build();

		ObjectVertex::bottomRight = new Vertex;
		ObjectVertex::bottomRight->vertices.Alloc(4);
		ObjectVertex::bottomRight->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::BottomRight);
		ObjectVertex::bottomRight->Build();

		ObjectVertex::bottomLeft = new Vertex;
		ObjectVertex::bottomLeft->vertices.Alloc(4);
		ObjectVertex::bottomLeft->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::BottomLeft);
		ObjectVertex::bottomLeft->Build();

		ObjectVertex::right = new Vertex;
		ObjectVertex::right->vertices.Alloc(4);
		ObjectVertex::right->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::Right);
		ObjectVertex::right->Build();

		ObjectVertex::left = new Vertex;
		ObjectVertex::left->vertices.Alloc(4);
		ObjectVertex::left->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::Left);
		ObjectVertex::left->Build();

		ObjectVertex::top = new Vertex;
		ObjectVertex::top->vertices.Alloc(4);
		ObjectVertex::top->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::Top);
		ObjectVertex::top->Build();

		ObjectVertex::bottom = new Vertex;
		ObjectVertex::bottom->vertices.Alloc(4);
		ObjectVertex::bottom->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::Bottom);
		ObjectVertex::bottom->Build();

		defaultUV = new Vertex;
		defaultUV->vertices.Alloc(4);
		defaultUV->vertices.InsertLast(PointF(0.f, 0.f));
		defaultUV->vertices.InsertLast(PointF(1.f, 0.f));
		defaultUV->vertices.InsertLast(PointF(1.f, 1.f));
		defaultUV->vertices.InsertLast(PointF(0.f, 1.f));
		defaultUV->Build();

		reverseUV = new Vertex;
		reverseUV->vertices.Alloc(4);
		reverseUV->vertices.InsertLast(PointF(0.f, 1.f));
		reverseUV->vertices.InsertLast(PointF(1.f, 1.f));
		reverseUV->vertices.InsertLast(PointF(1.f, 0.f));
		reverseUV->vertices.InsertLast(PointF(0.f, 0.f));
		reverseUV->Build();

		defaultSampler = new Sampler();
		pointSampler = new Sampler(Sampler::Filter::MinMagMipPoint);

		defaultBlend = new Blend(Blend::Value::SrcAlpha, Blend::Value::OneMinusSrcAlpha);

		defaultIndex = new Index;
		defaultIndex->indices.Alloc(6);
		defaultIndex->indices.InsertLast(0);
		defaultIndex->indices.InsertLast(1);
		defaultIndex->indices.InsertLast(2);
		defaultIndex->indices.InsertLast(2);
		defaultIndex->indices.InsertLast(3);
		defaultIndex->indices.InsertLast(0);
		defaultIndex->Build();
	}
	void Render() {
		//_System::_DirectX11::context->ExecuteCommandList(_System::_DirectX11::commandList, FALSE);

#ifdef _WIN32
		_System::_DXGI::Render();
#elif __ANDROID__
		eglSwapBuffers(_System::_Android::engine.display, _System::_Android::engine.surface);
#endif
	}
	bool IsVSync() { return _System::_Renderer::vSync; }

	unsigned GetMsaaCount() { return _System::_Renderer::msaaCount; }
	unsigned GetMsaaQuality() { return _System::_Renderer::msaaQuality; }

	RendererVersion GetCurrentRendererVersion() { return _System::_Renderer::version; }
	bool IsRendererInited() {
		return _System::_Renderer::version.name == System::RendererName::OpenGL;
	}
	void Clear(bool _clearDepth/* = false*/) {
#ifdef _WIN32
        _System::_DirectX11::Clear(_clearDepth);
#elif __ANDROID__
	    glClear(_clearDepth ? (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) : GL_COLOR_BUFFER_BIT);
#endif
	}
	void SetClearColor(float _r, float _g, float _b, float _a) {
		_System::_Renderer::backColor[0] = _r;
		_System::_Renderer::backColor[1] = _g;
		_System::_Renderer::backColor[2] = _b;
		_System::_Renderer::backColor[3] = _a;
#ifdef __ANDROID__
		glClearColor(_r, _g, _b, _a);
#endif
	}
	void SetWindowShow(WindowShow _windowShow) {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
#ifdef _WIN32
		ShowWindow(_System::_Windows::hWnd, (int)_windowShow);
#elif __ANDROID__

#endif
	}
	unsigned GetProcessorCoresNum() {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
#ifdef _WIN32
		return _System::_Windows::processorCoreNum;
#elif __ANDROID__
		return 0;
#endif
	}

	unsigned short GetProcessorArchitecture() {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
#ifdef _WIN32
		return _System::_Windows::processorArchitecture;
#elif __ANDROID__
		return 0;
#endif
	}

	bool IsExiting() { return _System::_Loop::exited; }
	unsigned GetWindowWidth() {
		return _System::_Renderer::windowSize.width;
	}
	unsigned GetWindowHeight() {
		return _System::_Renderer::windowSize.height;
	}
	PointU GetWindowSize() {
		return _System::_Renderer::windowSize;
	}
	Point GetWindowPos() {
#ifdef _WIN32
		return _System::_Renderer::windowPos;
#elif __ANDROID__
        return Point(0,0);
#endif
	}

	void SetTitle(const Tchar* _title) {
#ifdef _WIN32
		SetWindowText(_System::_Windows::hWnd, _title);
#elif __ANDROID__
#endif
	}

	bool IsResizeWindow() {
#ifdef _WIN32
		return _System::_Windows::resizeWindow;
#elif __ANDROID__
		return false;
#endif
	}
	bool IsMaximizedWindow() {
#ifdef _WIN32
		return _System::_Windows::maximized;
#elif __ANDROID__
		return false;
#endif
	}
	bool IsMinimizedWindow() {
#ifdef _WIN32
		return _System::_Windows::minimized;
#elif __ANDROID__
		return false;
#endif
	}

	bool IsMaximizingWindow() {
#ifdef _WIN32
		return _System::_Windows::maximizing;
#elif __ANDROID__
		return false;
#endif
	}
	bool IsMinimizingWindow() {
#ifdef _WIN32
		return _System::_Windows::minimizing;
#elif __ANDROID__
		return false;
#endif
	}

	void MinimizeWindow() {
#ifdef _WIN32
		ShowWindow(_System::_Windows::hWnd, SW_MINIMIZE);
#endif
	}
	void ResizeWindow(unsigned _width, unsigned _height) {
#ifdef _WIN32
		#ifdef _DEBUG
		if (!System::IsWindowInited());
		if (!_System::_Windows::resizeWindow);
		if ((_width == 0) || (_height == 0));
#endif
		RECT r, rs = { 0,0,_width,_height };
		GetWindowRect(_System::_Windows::hWnd, &r);
		AdjustWindowRect(&rs, GetWindowLong(_System::_Windows::hWnd, GWL_STYLE), false);
		MoveWindow(_System::_Windows::hWnd, r.left, r.top, rs.right, rs.bottom, false);
#elif __ANDROID__
#endif
	}
	ScreenMode GetScreenMode() {
#ifdef _WIN32
		return _System::_Windows::screenMode;
#elif __ANDROID__
		return _System::_Android::screenMode;
#endif
	}
	void OpenColsole() {
#ifdef _WIN32
		FILE* handle;
		freopen_s(&handle, "CONIN$", "r", stdin);
		freopen_s(&handle, "CONOUT$", "w", stdout);
		freopen_s(&handle, "CONERR$", "w", stderr);
		AllocConsole();
		_System::_Windows::consoleOpened = true;
#elif __ANDROID__
#endif
	}
	bool IsConsoleOpened() {
#ifdef _WIN32
		return _System::_Windows::consoleOpened;
#elif __ANDROID__
#endif
	}
	void CloseConsole() {
#ifdef _WIN32
		FreeConsole();
		_System::_Windows::consoleOpened = false;
#elif __ANDROID__
#endif
	}
	void DebugMemoryLeak() {
#ifdef _WIN32
		#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#else
#endif
	}
	void DebugMemoryLeakBreakPoint(int _value) {
#ifdef _WIN32
		#ifdef _DEBUG
		_CrtSetBreakAlloc(_value);
		#endif
#else
#endif
	}
	void Exit() {
#ifdef _WIN32
		DestroyWindow(_System::_Windows::hWnd);
#elif __ANDROID__
		_System::_Android::engine.app->destroyRequested = true;
#endif
	}
	bool IsPause() {
#ifdef _WIN32
		return _System::_Windows::pause;
#elif __ANDROID__
		return false;
#endif
	}
	OSversion GetOSVersion() {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
#ifdef _WIN32
		return _System::_Windows::_osversion;
#elif __ANDROID__
		return _System::_Android::_osVersion;
#endif
	}

	bool IsWindowInited() {
#ifdef _WIN32
		return (bool)_System::_Windows::hWnd;
#elif __ANDROID__
		return (bool)_System::_Android::engine.context;
#endif
	}
	float GetDeltaTime() { return _System::_Loop::deltaTime; }

	void SetMaxFrame(double _maxFrame) {
		_System::_Loop::maxFrame = _maxFrame;
	}
	float GetMaxFrame() { return _System::_Loop::maxFrame; }

#ifdef _WIN32
	void MsgBox(const Tchar* _msg, const Tchar* _title) {
#ifdef _DEBUG
		if (!System::IsWindowInited());
#endif
		MessageBox(_System::_Windows::hWnd, _msg, _title, MB_OK | MB_ICONERROR);
	}
#endif

#ifdef _WIN32
	void Wait(unsigned _milisecond) {
		Sleep(_milisecond);
	}
#elif __ANDROID__
	void Wait(unsigned _milisecond) {
		timespec s, o;
		s.tv_sec = 0;
		s.tv_nsec = _milisecond * 1000000;
		nanosleep(&s, &o);
	}
#endif

	bool IsActivated() {
#ifdef _WIN32
		return _System::_Windows::activated;
#elif __ANDROID__
		return true;
#endif
	}

#ifdef _WIN32
	int GetDisplayNum() {
		return _System::_DXGI::outputs.Size();
	}
	int GetCurrentDisplayIndex() {
		return _System::_DXGI::currentOutput;
	}
	int GetCurrentDisplayModeIndex() {
		return _System::_DXGI::currentDisplayMode;
	}
	int GetDisplayFromWindow() {
		return _System::_DXGI::GetDisplayFromWindow();
	}
	int GetDisplayModeNum(unsigned _displayIndex) {
		return _System::_DXGI::outputs[_displayIndex].num;
	}

	PointU GetDisplayModeSize(unsigned _displayIndex, unsigned _displayModeIndex) {
		if (_System::_DXGI::outputs[_displayIndex].output1) {
			return PointU(_System::_DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].Width,
						  _System::_DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].Height);
		} else {
			return PointU(_System::_DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].Width,
						  _System::_DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].Height);
		}
		return PointU(0, 0);
	}
	unsigned GetDisplayModeRefleshRateTop(unsigned _displayIndex, unsigned _displayModeIndex) {
		if (_System::_DXGI::outputs[_displayIndex].output1) {
			return _System::_DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].RefreshRate.Numerator;
		} else {
			return _System::_DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].RefreshRate.Numerator;
		}
		return 0;
	}
	unsigned GetDisplayModeRefleshRateBottom(unsigned _displayIndex, unsigned _displayModeIndex) {
		if (_System::_DXGI::outputs[_displayIndex].output1) {
			return _System::_DXGI::outputs[_displayIndex].displayModes1[_displayModeIndex].RefreshRate.Denominator;
		} else {
			return _System::_DXGI::outputs[_displayIndex].displayModes[_displayModeIndex].RefreshRate.Denominator;
		}
		return 0;
	}

	WindowState GetWindowState() {
		WINDOWPLACEMENT placeMent;
		placeMent.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(_System::_Windows::hWnd, &placeMent);

		if (placeMent.showCmd == SW_MAXIMIZE)return WindowState::Maximized;
		else if (placeMent.showCmd == SW_MINIMIZE)return WindowState::Minimized;
		return WindowState::Restore;
	}
	int GetCurrentDisplayModeIndex(unsigned _displayIndex) {
		return _System::_DXGI::outputs[_displayIndex].current;
	}
	void SetFullScreenMode(unsigned _displayIndex, unsigned _displayModeIndex) {
		_System::_DXGI::SetFullScreenMode(_displayIndex, _displayModeIndex);
		_System::_Windows::SetFullScreenMode(_displayIndex, _displayModeIndex);
	}
	void SetBorderlessScreenMode(unsigned _displayIndex) {
		_System::_DXGI::SetBorderlessScreenMode(_displayIndex);
		_System::_Windows::SetBorderlessScreenMode(_displayIndex);

	}
	void SetWindowMode(PointU _size, Point _pos, WindowState _state, bool _maximized, bool _minimized, bool _resizeWindow) {
		_System::_Windows::SetWindowMode(_pos, _size, _state, _maximized, _minimized, _resizeWindow);
		_System::_DXGI::SetWindowMode();
	}

	void MoveWindow(Point _pos) {
		SetWindowPos(_System::_Windows::hWnd, 0, _pos.x, _pos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	wchar_t* GetClipboardData() {
		if (IsClipboardFormatAvailable(CF_UNICODETEXT)) {
			OpenClipboard(_System::_Windows::hWnd);
			_System::_Windows::clipBoardMem = ::GetClipboardData(CF_UNICODETEXT);
			return (wchar_t*)GlobalLock(_System::_Windows::clipBoardMem);
		}
		return nullptr;
	}
	void SetClipbpardData(wchar_t* _text) {
		const size_t len = _tcslen(_text) + 1;

		HANDLE handle = GlobalAlloc(GMEM_MOVEABLE, len * sizeof(wchar_t));

		wchar_t* data = (TCHAR*)GlobalLock(handle);
		if (data != nullptr) {
#ifdef _WIN32
			memcpy_s(data, len * sizeof(wchar_t), _text, len * sizeof(wchar_t));
#else
			memcpy(data, _text, len * sizeof(TCHAR));
#endif
			GlobalUnlock(handle);
			if (OpenClipboard(_System::_Windows::hWnd)) {
				EmptyClipboard();
				SetClipboardData(CF_UNICODETEXT, handle);
				CloseClipboard();
			}
		}
	}
	//GetClipboardData를 호출했을 때만 호출
	void ClipboardClose() {
		GlobalUnlock(_System::_Windows::clipBoardMem);
		CloseClipboard();
	}

	void DragFileOn(bool _on /*= true*/) {
		DragAcceptFiles(_System::_Windows::hWnd, _on);
	}

	PointF GetDragFilePoint() {
		POINT pt;
		DragQueryPoint(_System::_Windows::hDrop, &pt);

		return PointF(MouseXToX(pt.x), MouseYToY(pt.y));
	}

	unsigned GetDragFileCount() {
		return DragQueryFileA(_System::_Windows::hDrop, UINT_MAX, nullptr, 0);
	}

	void GetDragFile(unsigned _index, char* _outFileName) {
		DragQueryFileA(_System::_Windows::hDrop, _index, _outFileName, 255);
	}
#elif __ANDROID__
	wchar_t* GetClipboardData() {
		JavaVM* javaVM = _System::_Android::engine.app->activity->vm;
		JNIEnv* jniEnv = _System::_Android::engine.app->activity->env;

		javaVM->AttachCurrentThread(&jniEnv, nullptr);

		// Retrieves NativeActivity.
		jobject lNativeActivity = _System::_Android::engine.app->activity->clazz;
		jclass ClassNativeActivity = jniEnv->GetObjectClass(lNativeActivity);

		jmethodID GetClipBoardDataF = jniEnv->GetMethodID(ClassNativeActivity, "GetClipBoardData", "()Ljava/lang/String;");
		jobject strObj = jniEnv->CallObjectMethod(lNativeActivity, GetClipBoardDataF);

		const char* str = jniEnv->GetStringUTFChars((jstring)strObj, nullptr);

		std::wstring wstr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
		wchar_t* data = new wchar_t[wstr.size()+1];
		memcpy(data, wstr.c_str(), (wstr.size()+1) * sizeof(wchar_t));

		jniEnv->ReleaseStringUTFChars((jstring)strObj, str);

		// Finished with the JVM.
		javaVM->DetachCurrentThread();

		return data;
	}
	void SetClipbpardData(wchar_t* _text) {
		JavaVM* javaVM = _System::_Android::engine.app->activity->vm;
		JNIEnv* jniEnv = _System::_Android::engine.app->activity->env;

		javaVM->AttachCurrentThread(&jniEnv, nullptr);

		// Retrieves NativeActivity.
		jobject lNativeActivity = _System::_Android::engine.app->activity->clazz;
		jclass ClassNativeActivity = jniEnv->GetObjectClass(lNativeActivity);

		jmethodID SetClipBoardDataF = jniEnv->GetMethodID(ClassNativeActivity, "SetClipBoardData", "(Ljava/lang/String;)V");

		jstring str = jniEnv->NewStringUTF(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(_text).c_str());

		jniEnv->CallVoidMethod(lNativeActivity, SetClipBoardDataF, str);

		jniEnv->DeleteLocalRef(str);

		// Finished with the JVM.
		javaVM->DetachCurrentThread();
	}
	void ClipboardClose() {
	}
#endif
	RectF GetScreenRect() {
		return RectF(-(float)_System::_Renderer::windowSize.width / 2.f, (float)_System::_Renderer::windowSize.width / 2.f,
			(float)_System::_Renderer::windowSize.height / 2.f, -(float)_System::_Renderer::windowSize.height / 2.f);
	}
	void OpenBrowser(const wchar_t* _url) {
#ifdef _WIN32
		ShellExecuteW(NULL, L"open", _url, L"", L"", SW_SHOW);
#elif __ANDROID__
		JavaVM* javaVM = _System::_Android::engine.app->activity->vm;
		JNIEnv* jniEnv = _System::_Android::engine.app->activity->env;

		javaVM->AttachCurrentThread(&jniEnv, nullptr);

		// Retrieves NativeActivity.
		jobject lNativeActivity = _System::_Android::engine.app->activity->clazz;
		jclass ClassNativeActivity = jniEnv->GetObjectClass(lNativeActivity);

		jmethodID ShowKeyboard = jniEnv->GetMethodID(ClassNativeActivity, "OpenBrowser", "(Ljava/lang/String;)V");
		jstring str;
		str = jniEnv->NewStringUTF(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(_url).c_str());

		jniEnv->CallVoidMethod(lNativeActivity, ShowKeyboard, str);


		// Finished with the JVM.
		javaVM->DetachCurrentThread();
#endif
	}
#ifdef __ANDROID__
	const char* GetProgramPath() {
		return _System::_Android::engine.app->activity->internalDataPath;
	}
	const wchar_t* GetProgramPathW() {
		return nullptr;
	}
#elif _WIN32
	unsigned GetProgramPath(char* _out, unsigned _len) {
		return GetCurrentDirectoryA(_len, _out);
	}
	unsigned GetProgramPathW(wchar_t* _out, unsigned _len) {
		return GetCurrentDirectoryW(_len, _out);
	}
#endif

    IP GetIP() {
#ifdef _WIN32
		wchar_t name[255];
		GetHostNameW(name, sizeof(name));
		ADDRINFOW info;
		ADDRINFOW* outInfo;
		memset(&info, 0, sizeof(info));
		info.ai_family = AF_INET;
		info.ai_socktype = SOCK_STREAM;
		info.ai_protocol = IPPROTO_TCP;

		GetAddrInfoW(name, NULL, &info, &outInfo);

		IP ipObject;
		ipObject.WriteIPInt(((sockaddr_in const*)outInfo->ai_addr)->sin_addr.S_un.S_addr);
		return ipObject;
#elif __ANDROID__
        JavaVM* javaVM = _System::_Android::engine.app->activity->vm;
        JNIEnv* jniEnv = _System::_Android::engine.app->activity->env;

        javaVM->AttachCurrentThread(&jniEnv, nullptr);

        // Retrieves NativeActivity.
        jobject lNativeActivity = _System::_Android::engine.app->activity->clazz;
        jclass ClassNativeActivity = jniEnv->GetObjectClass(lNativeActivity);

        jmethodID GetIP = jniEnv->GetMethodID(ClassNativeActivity, "GetIP", "()I");
        int ip = jniEnv->CallIntMethod(lNativeActivity, GetIP);

        // Finished with the JVM.
        javaVM->DetachCurrentThread();

		IP ipObject;
		ipObject.WriteIPInt(ip);
		return ipObject;
#endif
	}
}

