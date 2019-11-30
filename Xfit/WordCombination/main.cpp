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
#include <decoder/WAVDecoder.h>
#include <object/Shape.h>
#include <object/ShapeInstance.h>

#include "LabelImage.h"
#include "FadeLabelButton.h"
#include "LabelImage.h"
#include "LabelToggleButton.h"
#include "List.h"
#include "VisualTextBox.h"
#include "EditBox.h"

static random_device rd;
mt19937_64 random(rd());

static void* fontData;
static void* boldFontData;

static File file;

List<>* list;

//VisualTextBox* visualTextBox;
EditBox* editBox;


struct Node {
	List<>* list;
	EditBox* editBox;
	LabelImage* labelImage;
	LabelButton* AddButton;
	LabelButton* DeleteButton;
};

vector<Node> nodes;
LabelImage* nodeCountImage;
LabelButton* nodeIncreaseButton;
LabelButton* nodeDecreaseButton;
LabelButton* startButton;

LabelButton* nextNodeButton;
LabelButton* prevNodeButton;

bool playing = false;


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

bool AddButtonUp(Button* _target, Point _mousePos, void* _data) {
	if (!playing) {
		int i = (int)_data;
		if (nodes[i].editBox->text.size() > 0) {
			auto label = new SizeLabel;
			label->sizes = new FontSize[1];
			label->colors = new FontColor[1];
			label->baseSizes = new unsigned[1];
			label->fonts = new FontContainer[1];
			label->renders = new FontRender[1];
			label->sizeLen = 1;

			label->sizes[0].pixelSize = 15;
			label->sizes[0].len = 0;

			label->colors[0].color = 0;
			label->colors[0].len = 0;

			label->baseSizes[0] = 15;

			label->fonts[0].font = font;
			label->fonts[0].len = 0;

			label->renders[0].type = FontRenderType::Normal;
			label->renders[0].len = 0;

			auto text = new wchar_t[nodes[i].editBox->text.size() + 1];
			wcscpy_s(text, nodes[i].editBox->text.size() + 1, nodes[i].editBox->text.c_str());
			label->text = text;
			label->SizePrepareDraw(WindowRatio());
			if (nodes[i].list->nodes.Size() >= nodes[i].list->nodes.MaxSize()) {
				nodes[i].list->nodes.ReAlloc(nodes[i].list->nodes.MaxSize() + 1000);
			}
			nodes[i].list->nodes.EmplaceLast(DefaultNode(label));
			nodes[i].list->UpdateNodes();
		}
	}
	return true;
}
bool DeleteButtonUp(Button* _target, Point _mousePos, void* _data) {
	if (!playing) {
		int i = (int)_data;
		if (nodes[i].list->GetSelectIndex() != UINT_MAX) {
			delete []nodes[i].list->nodes[nodes[i].list->GetSelectIndex()].label->text;
			delete nodes[i].list->nodes[nodes[i].list->GetSelectIndex()].label;
			nodes[i].list->nodes.EraseIndex(nodes[i].list->GetSelectIndex());
			nodes[i].list->UpdateNodes();
		}
	}
	return true;
}

unsigned playCount = 0;
unsigned maxPlayCount;
unsigned nodeCount = 0;
int textIndex = -1;
float speed = 0.5f;
float speedT = 0.f;

bool StartButtonUp(Button* _target, Point _mousePos, void* _data) {
	if (playing)return true;
	playing = true;
	playCount = 0;
	nodeCount = 0;
	speed = 0.5f;
	speedT = 0.f;
	textIndex = -1;
	maxPlayCount = 30 + random() % 10;
	for (int i = 0; i < nodes.size(); i++) {
		if (wcscmp(nodes[i].labelImage->GetLabel()->text, L"\"\"") != 0) {
			nodes[i].labelImage->GetLabel()->text = L"\"\"";
			nodes[i].labelImage->GetLabel()->SizePrepareDraw(WindowRatio());

			nodes[i].labelImage->SetPos(nodes[i].labelImage->basePos);

			nodes[i].labelImage->SetScale(PointF(nodes[i].labelImage->GetLabel()->GetWidth(), nodes[i].labelImage->GetLabel()->GetHeight()));
		}
	}
	return true;
}

Sound* sound1, * sound2;
SoundSource source;
SoundSource source2;
static void Init() {
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

	//file.Open("Spoqa Han Sans Bold.ttf");
	//fileSize = file.GetSize();
	//boldFontData = new unsigned char[fileSize];
	//file.ReadBytes(fileSize, boldFontData);
	//boldFont = new Font(boldFontData, fileSize, 0);
	//file.Close();

	//bgSound = new Sound;

	sound1 = new Sound;
	sound2 = new Sound;

	file.Open("sound1.wav");

	fileSize = file.GetSize();
	void* sound1Data = new unsigned char[fileSize];
	file.ReadBytes(fileSize, sound1Data);

	WAVDecoder decoder1;

	auto data = decoder1.LoadHeaderAndDecode(sound1Data, fileSize);
	source.rawData = data;
	source.size = decoder1.GetOutputSize();
	sound1->Decode(&source);

	file.Close();

	file.Open("sound2.wav");

	fileSize = file.GetSize();
	void* sound2Data = new unsigned char[fileSize];
	file.ReadBytes(fileSize, sound2Data);

	WAVDecoder decoder2;

	data = decoder2.LoadHeaderAndDecode(sound2Data, fileSize);
	source2.rawData = data;
	source2.size = decoder2.GetOutputSize();
	sound2->Decode(&source2);

	file.Close();

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

	auto nodeCountImageLabel = new SizeLabel;
	nodeCountImageLabel->sizes = new FontSize[1];
	nodeCountImageLabel->colors = new FontColor[1];
	nodeCountImageLabel->baseSizes = new unsigned[1];
	nodeCountImageLabel->fonts = new FontContainer[1];
	nodeCountImageLabel->renders = new FontRender[1];
	nodeCountImageLabel->sizeLen = 1;

	nodeCountImageLabel->sizes[0].pixelSize = 25;
	nodeCountImageLabel->sizes[0].len = 0;

	nodeCountImageLabel->colors[0].color = 0;
	nodeCountImageLabel->colors[0].len = 0;

	nodeCountImageLabel->baseSizes[0] = 25;

	nodeCountImageLabel->fonts[0].font = font;
	nodeCountImageLabel->fonts[0].len = 0;

	nodeCountImageLabel->renders[0].type = FontRenderType::Normal;
	nodeCountImageLabel->renders[0].len = 0;

	nodeCountImageLabel->text = L"3";
	nodeCountImageLabel->SizePrepareDraw(WindowRatio());

	nodeCountImage = new LabelImage(PosType::Center, nodeCountImageLabel, PointF(0.f, 300.f));

	auto nodeIncreaseImageLabel = new SizeLabel;
	nodeIncreaseImageLabel->sizes = new FontSize[1];
	nodeIncreaseImageLabel->colors = new FontColor[1];
	nodeIncreaseImageLabel->baseSizes = new unsigned[1];
	nodeIncreaseImageLabel->fonts = new FontContainer[1];
	nodeIncreaseImageLabel->renders = new FontRender[1];
	nodeIncreaseImageLabel->sizeLen = 1;

	nodeIncreaseImageLabel->sizes[0].pixelSize = 20;
	nodeIncreaseImageLabel->sizes[0].len = 0;

	nodeIncreaseImageLabel->colors[0].color = 0x444444;
	nodeIncreaseImageLabel->colors[0].len = 0;

	nodeIncreaseImageLabel->baseSizes[0] = 20;

	nodeIncreaseImageLabel->fonts[0].font = font;
	nodeIncreaseImageLabel->fonts[0].len = 0;

	nodeIncreaseImageLabel->renders[0].type = FontRenderType::Normal;
	nodeIncreaseImageLabel->renders[0].len = 0;

	nodeIncreaseImageLabel->text = L"증가";
	nodeIncreaseImageLabel->SizePrepareDraw(WindowRatio());

	nodeIncreaseButton = new LabelButton(PosType::Center, nodeIncreaseImageLabel, PointF(-50.f, 300.f));

	auto nodeDecreaseImageLabel = new SizeLabel;
	nodeDecreaseImageLabel->sizes = new FontSize[1];
	nodeDecreaseImageLabel->colors = new FontColor[1];
	nodeDecreaseImageLabel->baseSizes = new unsigned[1];
	nodeDecreaseImageLabel->fonts = new FontContainer[1];
	nodeDecreaseImageLabel->renders = new FontRender[1];
	nodeDecreaseImageLabel->sizeLen = 1;

	nodeDecreaseImageLabel->sizes[0].pixelSize = 20;
	nodeDecreaseImageLabel->sizes[0].len = 0;

	nodeDecreaseImageLabel->colors[0].color = 0x444444;
	nodeDecreaseImageLabel->colors[0].len = 0;

	nodeDecreaseImageLabel->baseSizes[0] = 20;

	nodeDecreaseImageLabel->fonts[0].font = font;
	nodeDecreaseImageLabel->fonts[0].len = 0;

	nodeDecreaseImageLabel->renders[0].type = FontRenderType::Normal;
	nodeDecreaseImageLabel->renders[0].len = 0;

	nodeDecreaseImageLabel->text = L"감소";
	nodeDecreaseImageLabel->SizePrepareDraw(WindowRatio());

	nodeDecreaseButton = new LabelButton(PosType::Center, nodeDecreaseImageLabel, PointF(50.f, 300.f));

	nodes.resize(3);

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

	auto addNodeLabel = new SizeLabel;
	addNodeLabel->sizes = new FontSize[1];
	addNodeLabel->colors = new FontColor[1];
	addNodeLabel->baseSizes = new unsigned[1];
	addNodeLabel->fonts = new FontContainer[1];
	addNodeLabel->renders = new FontRender[1];
	addNodeLabel->sizeLen = 1;

	addNodeLabel->sizes[0].pixelSize = 20;
	addNodeLabel->sizes[0].len = 0;

	addNodeLabel->colors[0].color = 0x00ff00;
	addNodeLabel->colors[0].len = 0;

	addNodeLabel->baseSizes[0] = 20;

	addNodeLabel->fonts[0].font = font;
	addNodeLabel->fonts[0].len = 0;

	addNodeLabel->renders[0].type = FontRenderType::Normal;
	addNodeLabel->renders[0].len = 0;

	addNodeLabel->text = L"항목 추가";
	addNodeLabel->SizePrepareDraw(WindowRatio());

	auto deleteNodeLabel = new SizeLabel;
	deleteNodeLabel->sizes = new FontSize[1];
	deleteNodeLabel->colors = new FontColor[1];
	deleteNodeLabel->baseSizes = new unsigned[1];
	deleteNodeLabel->fonts = new FontContainer[1];
	deleteNodeLabel->renders = new FontRender[1];
	deleteNodeLabel->sizeLen = 1;

	deleteNodeLabel->sizes[0].pixelSize = 20;
	deleteNodeLabel->sizes[0].len = 0;

	deleteNodeLabel->colors[0].color = 0xff0000;
	deleteNodeLabel->colors[0].len = 0;

	deleteNodeLabel->baseSizes[0] = 20;

	deleteNodeLabel->fonts[0].font = font;
	deleteNodeLabel->fonts[0].len = 0;

	deleteNodeLabel->renders[0].type = FontRenderType::Normal;
	deleteNodeLabel->renders[0].len = 0;

	deleteNodeLabel->text = L"항목 삭제";
	deleteNodeLabel->SizePrepareDraw(WindowRatio());

	//1280x720일때 간격 320
	for (int i = 0; i < 3; i++) {
		nodes[i].list = new List<>(PosType::Center, PointF(300.f, 400.f), 20.f, PointF(-320.f + i * 320.f, 70.f));

		nodes[i].list->nodes.Alloc(1000);

		nodes[i].list->BuildNodes(1000);

		nodes[i].editBox = new EditBox(PointF(-320.f + i * 320.f, -170.f), PointF(1.f, 1.f), 300, 20);

		nodes[i].AddButton = new LabelButton(PosType::Center, addNodeLabel, PointF(-320.f + i * 320.f, -210.f));
		nodes[i].DeleteButton = new LabelButton(PosType::Center, deleteNodeLabel, PointF(-320.f + i * 320.f, -250.f));

		nodes[i].AddButton->data = (void*)i;
		nodes[i].DeleteButton->data = (void*)i;

		nodes[i].AddButton->buttonUp = AddButtonUp;
		nodes[i].DeleteButton->buttonUp = DeleteButtonUp;

		auto label = new SizeLabel;
		label->sizes = new FontSize[1];
		label->colors = new FontColor[1];
		label->baseSizes = new unsigned[1];
		label->fonts = new FontContainer[1];
		label->renders = new FontRender[1];
		label->sizeLen = 1;

		label->sizes[0].pixelSize = 20;
		label->sizes[0].len = 0;

		label->colors[0].color = 0;
		label->colors[0].len = 0;

		label->baseSizes[0] = 20;

		label->fonts[0].font = font;
		label->fonts[0].len = 0;

		label->renders[0].type = FontRenderType::Normal;
		label->renders[0].len = 0;

		label->text = L"\"\"";
		label->SizePrepareDraw(WindowRatio());

		nodes[i].labelImage = new LabelImage(PosType::Center, label, PixelPerfectPoint(PointF(-320.f + i * 320.f, -290.f),
			label->GetWidth(), label->GetHeight(), CenterPointPos::Center));
	}
	auto startLabel = new SizeLabel;
	startLabel->sizes = new FontSize[1];
	startLabel->colors = new FontColor[1];
	startLabel->baseSizes = new unsigned[1];
	startLabel->fonts = new FontContainer[1];
	startLabel->renders = new FontRender[1];
	startLabel->sizeLen = 1;

	startLabel->sizes[0].pixelSize = 30;
	startLabel->sizes[0].len = 0;

	startLabel->colors[0].color = 0;
	startLabel->colors[0].len = 0;

	startLabel->baseSizes[0] = 30;

	startLabel->fonts[0].font = font;
	startLabel->fonts[0].len = 0;

	startLabel->renders[0].type = FontRenderType::Normal;
	startLabel->renders[0].len = 0;

	startLabel->text = L"시작!";
	startLabel->SizePrepareDraw(WindowRatio());
	startButton = new LabelButton(PosType::Center, startLabel, PointF(560.f, 310.f));

	startButton->buttonUp = StartButtonUp;

	/*unsigned a = 0;
	
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
	}*/
	
	
	
	//------------------------------------------------------------------------------------------------
	//editBox = new EditBox(PointF(0.f,0.f),PointF(1.f,1.f),200, 20);

	//------------------------------------------------------------------------------------------------

	
}


static void Update(void* _data) {
	bool requireDraw = sizeRequest;

	if (playing) {
		while (speedT >= speed) {
			speedT -= speed;
			if (playCount >= maxPlayCount) {
				speed = 0.5f;
				speedT = 0.f;
				nodeCount++;
				playCount = 0;
				maxPlayCount = 30 + random() % 10;
				sound2->SetPos(0);
				sound2->Play(1);
			} else {
				sound1->SetPos(0);
				sound1->Play(1);
			}
			while (nodeCount < nodes.size()) {
				if (nodes[nodeCount].list->nodes.Size() == 0) {
					nodeCount++;
				} else if (nodes[nodeCount].list->nodes.Size() == 1) {
					nodes[nodeCount].labelImage->GetLabel()->text = nodes[nodeCount].list->nodes[0].label->text;
					nodes[nodeCount].labelImage->GetLabel()->SizePrepareDraw(WindowRatio());

					nodes[nodeCount].labelImage->SetPos(nodes[nodeCount].labelImage->basePos);

					nodes[nodeCount].labelImage->SetScale(PointF(nodes[nodeCount].labelImage->GetLabel()->GetWidth(), nodes[nodeCount].labelImage->GetLabel()->GetHeight()));

					nodeCount++;
				} else break;
			}
			if (nodeCount == nodes.size()) {
				playing = false;
				goto end;
			}
			unsigned len = nodes[nodeCount].list->nodes.Size();
			unsigned index;
			if (textIndex == -1) {
				index = random() % len;
			} else {
				while (true) {
					index = random() % len;
					if (textIndex != index)break;
				}
			}
			textIndex = index;
			nodes[nodeCount].labelImage->GetLabel()->text = nodes[nodeCount].list->nodes[textIndex].label->text;
			nodes[nodeCount].labelImage->GetLabel()->SizePrepareDraw(WindowRatio());

			nodes[nodeCount].labelImage->SetPos(nodes[nodeCount].labelImage->basePos);

			nodes[nodeCount].labelImage->SetScale(PointF(nodes[nodeCount].labelImage->GetLabel()->GetWidth(), nodes[nodeCount].labelImage->GetLabel()->GetHeight()));

			playCount++;
			speed *= 0.9f;
		}
		speedT += System::GetDeltaTime();
	}
end:;
	

	startButton->Update();
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i].list->Update();
		nodes[i].editBox->Update();
		nodes[i].AddButton->Update();
		nodes[i].DeleteButton->Update();
	}

	//if(requireDraw && !System::IsPause()) {
	if(!System::IsPause()) {
		sizeRequest = false;

		System::Clear(true);

		//nodeCountImage->Draw();
		startButton->Draw();
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i].list->Draw();
			nodes[i].editBox->Draw();
			nodes[i].AddButton->Draw();
			nodes[i].DeleteButton->Draw();
			nodes[i].labelImage->Draw();
		}

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
	delete font;
	delete boldFont;


	Font::ThreadRelease();
	Font::Release();

	delete[]fontData;
	delete[]boldFontData;

	Sound::Release();
}
void Main_Size(void* _data) {
	if (!nodeCountImage) return;

	nodeCountImage->Size();
	startButton->Size();
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i].list->Size();
		nodes[i].editBox->Size();
		nodes[i].AddButton->Size();
		nodes[i].DeleteButton->Size();
		nodes[i].labelImage->Size();
	}

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

	Sound::Init(100, 44100);

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