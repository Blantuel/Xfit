#include "main.h"

#include <resource/Sampler.h>
#include <system/System.h>
#include <system/Context.h>
#include <data/Array.h>
#include <file/File.h>
#include <text/Font.h>
#include <text/Label.h>
#include <effect/Blend.h>
#include <sound/Sound.h>
#include <text/TextBox.h>
#include <resource/ShapeVertex.h>
#include <system/Input.h>
#include <math/Rect.h>
#include <physics/Collision.h>
#include <component/Button.h>
#include <physics/RectHitTest.h>
#include <decoder/OGGDecoder.h>
#include <object/Shape.h>



static random_device rd;
mt19937_64 random(rd());

static void* fontData;
static void* boldFontData;

static File file;

static bool sizeRequest = true;


static void Init() {
	System::SetClearColor(1.f, 1.f, 1.f, 1.f);

	File file("../Test/NanumSquareR.ttf");
	unsigned fileSize = file.GetSize();
	fontData = new unsigned char[fileSize];
	file.ReadBytes(fileSize, fontData);
	font = new Font(fontData, fileSize, 0);
	file.Close();

	file.Open("../Test/NanumSquareB.ttf");
	fileSize = file.GetSize();
	boldFontData = new unsigned char[fileSize];
	file.ReadBytes(fileSize, boldFontData);
	boldFont = new Font(boldFontData, fileSize, 0);
	file.Close();
}



static void Update(void* _data) {
	bool requireDraw = sizeRequest;

	if (requireDraw) {
		sizeRequest = false;

		System::Clear(true);


		System::Render();
	} else {
		System::Wait(16);
	}
}
static void Activate(void* _data) {

}
static void Destroy() {

	Font::ThreadRelease();
	Font::Release();

	delete[]fontData;
	delete[]boldFontData;
}
void Main_Size(void* _data) {

	sizeRequest = true;
}

static void Create() {
	System::CreateInfo createInfo;
	createInfo.screenMode = System::ScreenMode::Window;
	createInfo.windowSize.width = 1280;
	createInfo.windowSize.height = 720;
	createInfo.windowShow = System::WindowShow::Default;

	createInfo.title = _T("The Record");
	createInfo.windowPos.x = System::WindowDefaultPos;
	createInfo.windowPos.y = System::WindowDefaultPos;
	createInfo.cursorResource = nullptr;
	createInfo.maximized = true;
	createInfo.minimized = true;
	createInfo.resizeWindow = true;

	createInfo.msaaCount = 1;
	createInfo.msaaQuality = 0;
	createInfo.vSync = true;
	createInfo.maxFrame = 0;
	createInfo.screenIndex = 1;

	System::updateFuncs = Update;
	System::activateFunc = Activate;
	System::destroyFunc = Destroy;
	System::sizeFunc = Main_Size;

	System::Init(&createInfo);

	Font::Init(30000, 30000);
	Font::ThreadInit(1000, 10000, 1920 * 1080, 1920 * 1080);

	System::SetClearColor(1.f, 1.f, 1.f, 1.f);

	Init();
}
int APIENTRY _tWinMain(HINSTANCE _hInstance, HINSTANCE, LPTSTR, int) {
	System::createFunc = Create;

	System::Create(_hInstance);

	return 0;
}
