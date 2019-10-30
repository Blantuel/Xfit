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
#include <object/ShapeInstance.h>

#include "LabelImage.h"
#include "FadeLabelButton.h"
#include "LabelImage.h"
#include "LabelToggleButton.h"
#include "Title.h"
#include "Option.h"
#include "List.h"
#include "VisualTextBox.h"

static random_device rd;
mt19937_64 random(rd());

static void* fontData;
static void* boldFontData;

static File file;

ShapeInstance* si;

List<>* list;

VisualTextBox* visualTextBox;

const PointF PixelPerfectPoint(PointF _p, unsigned _width, unsigned _height, CenterPointPos _centerPointPos, bool _isVertical /*= false*/) {
	_p.x = floor(_p.x);
	_p.y = floor(_p.y);
	if (System::GetWindowWidth() % 2 != 0)_p.x -= 0.5f;
	if (System::GetWindowHeight() % 2 != 0)_p.y += 0.5f;

	switch (_centerPointPos) {
	case CenterPointPos::Center:
		if (_isVertical) {
			if (_width % 2 != 0)_p.y -= 0.5f;
			if (_height % 2 != 0)_p.x += 0.5f;
		} else {
			if (_width % 2 != 0)_p.x += 0.5f;
			if (_height % 2 != 0)_p.y -= 0.5f;
		}
		break;
	case CenterPointPos::TopRight:break;
	case CenterPointPos::TopLeft:break;
	case CenterPointPos::BottomRight:break;
	case CenterPointPos::BottomLeft:break;
	case CenterPointPos::Left:
	case CenterPointPos::Right:
		if (_isVertical) {
			if (_height % 2 != 0)_p.x += 0.5f;
		} else {
			if (_height % 2 != 0)_p.y -= 0.5f;
		}
		break;
	case CenterPointPos::Top:
	case CenterPointPos::Bottom:
		if (_isVertical) {
			if (_width % 2 != 0)_p.y -= 0.5f;
		} else {
			if (_width % 2 != 0)_p.x += 0.5f;
		}
		break;
	}


	return _p;
}

Vertex* SelectVertex2D(CenterPointPos _centerPointPos) {
	switch (_centerPointPos) {
	case CenterPointPos::Center:return System::defaultVertex2D;
	case CenterPointPos::TopRight:return topRightVertex;
	case CenterPointPos::TopLeft:return topLeftVertex;
	case CenterPointPos::BottomRight:return bottomRightVertex;
	case CenterPointPos::BottomLeft:return bottomLeftVertex;
	case CenterPointPos::Left:return leftVertex;
	case CenterPointPos::Right:return rightVertex;
	case CenterPointPos::Top:return topVertex;
	case CenterPointPos::Bottom:return bottomVertex;
	}
}
Image* vectorImage;
wchar_t texts[1000][3];


/*
void plotLine(unsigned* _data,unsigned _width, unsigned _color, int x0, int y0, int x1, int y1) {
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; //error value e_xy
	for (;;) { //loop 
		_data[y0*_width + x0] = _color;
		e2 = 2 * err;
		if (e2 >= dy) { // e_xy+e_x > 0
			if (x0 == x1) break;
			err += dy; x0 += sx;
		}
		if (e2 <= dx) { // e_xy+e_y < 0
			if (y0 == y1) break;
			err += dx; y0 += sy;
		}
	}
}
*/
/*
void plotEllipse(unsigned* _data, unsigned _width, unsigned _color, int xm, int ym, int a, int b) {
	int x = -a, y = 0; // II. quadrant from bottom left to top right
	long e2 = (long)b * b, err = x * (2 * e2 + x) + e2; // error of 1.step
	do {
		_data[(ym + y) * _width + (xm - x)] = _color;// I. Quadrant 
		_data[(ym + y) * _width + (xm + x)] = _color;// II. Quadrant 
		_data[(ym - y) * _width + (xm + x)] = _color;// III. Quadrant 
		_data[(ym - y) * _width + (xm - x)] = _color;// IV. Quadrant 

		e2 = 2 * err;
		if (e2 >= (x * 2 + 1) * (long)b * b) // e_xy+e_x > 0 
			err += (++x * 2 + 1) * (long)b * b;
		if (e2 <= (y * 2 + 1) * (long)a * a) // e_xy+e_y < 0 
			err += (++y * 2 + 1) * (long)a * a;
	} while (x <= 0);
	while (y++ < b) { // to early stop of flat ellipses a=1, 
		_data[(ym + y) * _width + (xm)] = _color; // -> finish tip of ellipse
		_data[(ym - y) * _width + (xm)] = _color;
	}
}
*/

static void Init() {
	/*vectorImage = new Image(PointF(0, 0), PointF(500, 500), 0, System::defaultBlend, nullptr, new Frame, System::defaultVertex2D, System::defaultUV, System::defaultIndex);

	unsigned* data = new unsigned[500 * 500];

	Memory::Set(data, 0x00000000, 500 * 500);

	plotLine(data, 500, 0xff000000, 0, 0, 200, 200);
	plotEllipse(data, 500, 0xff000000, 200, 200, 100, 50);

	vectorImage->frame->Build(data, 500, 500);

	delete[]data;*/

	//------------------------------------------------------------------------------------------------

	fontRender = new FontRender[1];
	fontRender[0].len = 0;
	fontRender[0].type = FontRenderType::Normal;

	//------------------------------------------------------------------------------------------------

	System::SetClearColor(1.f, 1.f, 1.f, 1.f);

	lineVertex = new Vertex;
	lineVertex->vertices.Alloc(2);

	//"DungGeunMo.otf"//16배수 크기 픽셀 폰트
	File file("Spoqa Han Sans Regular.ttf");//16배수 크기 픽셀 폰트
	unsigned fileSize = file.GetSize();
	fontData = new unsigned char[fileSize];
	file.ReadBytes(fileSize, fontData);
	font = new Font(fontData, fileSize, 0);
	file.Close();

	file.Open("Spoqa Han Sans Bold.ttf");
	fileSize = file.GetSize();
	boldFontData = new unsigned char[fileSize];
	file.ReadBytes(fileSize, boldFontData);
	boldFont = new Font(boldFontData, fileSize, 0);
	file.Close();

	bgSound = new Sound;

	bgSound->SetVolume(0.5f);

	topRightVertex = new Vertex;
	topRightVertex->vertices.Alloc(4);
	topRightVertex->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::TopRight);
	topRightVertex->Build();

	topLeftVertex = new Vertex;
	topLeftVertex->vertices.Alloc(4);
	topLeftVertex->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::TopLeft);
	topLeftVertex->Build();

	bottomRightVertex = new Vertex;
	bottomRightVertex->vertices.Alloc(4);
	bottomRightVertex->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::BottomRight);
	bottomRightVertex->Build();

	bottomLeftVertex = new Vertex;
	bottomLeftVertex->vertices.Alloc(4);
	bottomLeftVertex->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::BottomLeft);
	bottomLeftVertex->Build();

	rightVertex = new Vertex;
	rightVertex->vertices.Alloc(4);
	rightVertex->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::Right);
	rightVertex->Build();

	leftVertex = new Vertex;
	leftVertex->vertices.Alloc(4);
	leftVertex->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::Left);
	leftVertex->Build();

	topVertex = new Vertex;
	topVertex->vertices.Alloc(4);
	topVertex->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::Top);
	topVertex->Build();

	bottomVertex = new Vertex;
	bottomVertex->vertices.Alloc(4);
	bottomVertex->MakeImageVertex2D(PointF(1.f, 1.f), CenterPointPos::Bottom);
	bottomVertex->Build();

	//------------------------------------------------------------------------------------------------

	title = new Title;



	option = new Option;


	//------------------------------------------------------------------------------------------------

	ShapeVertex* vertex = new ShapeVertex;
	vertex->vertices.Alloc(4);
	vertex->vertices.EmplaceLast(-100.f, 100.f);
	vertex->vertices.EmplaceLast(100.f, 100.f);
	vertex->vertices.EmplaceLast(100.f, -100.f);
	vertex->vertices.EmplaceLast(-100.f, -100.f);
	vertex->Build();

	si = new ShapeInstance(PointF(0, 0), PointF(1, 1), 0, System::defaultBlend, vertex);

	ShapeInstanceNode node;
	node.mat = Matrix::GetMove(100, 0, 0);
	node.lineWidth = 5.f;
	node.fillColor = Point3DwF(0.f, 1.f, 1.f, 1.f);
	node.lineColor = Point3DwF(1.f, 0.f, 0.f, 1.f);

	si->nodes.Alloc(4);
	si->nodes.InsertLast(node);
	node.mat = Matrix::GetMove(200, 0, 0);
	si->nodes.InsertLast(node);

	si->BuildInstance();

	//------------------------------------------------------------------------------------------------

	list = new List<>(PosType::Center, PointF(300.f, 500.f), 20.f, PointF(0.f, 0.f));


	list->nodes.Alloc(1000);
	for (unsigned i = 0; i < 100; i++)list->nodes.EmplaceLast(new SizeLabel);

	
	FontColor* colors = new FontColor[1];
	colors[0].color = 0;
	colors[0].len = 0;
	FontSize* sizes = new FontSize[1];
	sizes[0].len = 0;
	sizes[0].pixelSize = 10;
	unsigned* baseSizes = new unsigned[1];
	baseSizes[0] = 10;
	FontContainer* container = new FontContainer[1];
	container[0].font = font;
	container[0].len = 0;
	unsigned a = 0;
	
	for (unsigned i = 0; i < 100; i++) {
		texts[i][0] = L'안';
		texts[i][1] = L'0'+i;
		texts[i][2] = L'\0';
	}
	for (auto& i : list->nodes) {
		i.label->colors = colors;
		i.label->sizeLen = 1;
		i.label->baseSizes = baseSizes;
		i.label->sizes = sizes;
		i.label->fonts = container;
		i.label->renders = fontRender;
		i.label->text = texts[a];
		a++;
	}
	
	list->BuildNodes(1000);
	list->Select(3);
	
	//------------------------------------------------------------------------------------------------

	visualTextBox = new VisualTextBox(200,200);

	//------------------------------------------------------------------------------------------------

	
}



static void Update(void* _data) {
	bool requireDraw = sizeRequest;

	switch (scene) {
	case Scene::Title:
		requireDraw = title->Update() ? true : requireDraw;
		break;
	}
	requireDraw = option->Update() ?true: requireDraw;

	//list->Update();
	visualTextBox->Update();

	//if(requireDraw && !System::IsPause()) {
	if(!System::IsPause()) {
		sizeRequest = false;

		System::Clear(true);

		//vectorImage->Draw();

		//list->Draw();

		visualTextBox->Draw();

		switch (scene) {
		case Scene::Title:
			title->Draw();
			break;
		}
		option->Draw();

		System::Render();
	} else {
#ifdef _WIN32
		System::Wait(16);
#elif __ANDROID__
#endif
	}
}
static void Activate(void* _data) {

}
static void Destroy() {
	delete title;
	delete option;

	delete font;
	delete boldFont;


	Font::ThreadRelease();
	Font::Release();

	delete[]fontData;
	delete[]boldFontData;

	Sound::Release();
}
void Main_Size(void* _data) {
	if (!title)return;
	switch (scene) {
	case Scene::Title:
		title->Size();
		break;
	}

	option->Size();

	list->Size();

	sizeRequest = true;
}

static void Create() {
	System::CreateInfo createInfo;
	createInfo.screenMode = System::ScreenMode::Window;
	createInfo.windowSize.width = 1280;
	createInfo.windowSize.height = 720;
	createInfo.windowShow = System::WindowShow::Default;

#ifdef _WIN32
	createInfo.title = _T("The Record");
	createInfo.windowPos.x = System::WindowDefaultPos;
	createInfo.windowPos.y = System::WindowDefaultPos;
	createInfo.cursorResource = nullptr;
	createInfo.maximized = true;
	createInfo.minimized = true;
	createInfo.resizeWindow = true;
#endif

	createInfo.msaaCount = 1;
	createInfo.msaaQuality = 0;
	createInfo.vSync = true;
	createInfo.maxFrame = 0;
	createInfo.screenIndex = 0;
	createInfo.refleshRate = 0;

	System::updateFuncs = Update;
	System::activateFunc = Activate;
	System::destroyFunc = Destroy;
	System::sizeFunc = Main_Size;

	System::Init(&createInfo);

	Font::Init(30000, 30000);
	Font::ThreadInit(1000, 10000, 1920 * 1080, 1920 * 1080);

	Sound::Init(5, 48000);

	System::SetClearColor(1.f, 1.f, 1.f, 1.f);

	Init();
}

#ifdef _WIN32
int APIENTRY _tWinMain(HINSTANCE _hInstance, HINSTANCE, LPTSTR, int) {
	System::createFunc = Create;

	System::Create(_hInstance);

	return 0;
}
#elif __ANDROID__
void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
	System::createFunc = Create;

	System::Create(activity);
}
#endif