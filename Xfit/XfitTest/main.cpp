#include <object/Image.h>
#include <object/ImageInstance.h>
#include <resource/Sampler.h>
#include <system/System.h>
#include <system/Context.h>
#include <data/Array.h>
#include <data/Converter.h>
#include <file/File.h>
#include <object/Button.h>
#include <physics/RectCollision.h>
#include <object/AnimateLoopObject.h>
#include <text/Font.h>
#include <text/Label.h>
#include <effect/Blend.h>
#include <sound/Sound.h>
#include <text/TextBox.h>

float d = 0;

Image* shape;
ImageInstance* instance;
Button* button;
AnimateLoopObject* sprite;
Label* label;
Font* font;
Sound* sound;
SoundSource soundSource;
TextBox* textBox;

void Init() {
	File file;
	file.Load(L"test.jpg", false, true);
	size_t size = file.GetSize();
	char* data = new char[size];
	file.ReadBytes(size, data);
	file.Close();

	JPEGConverter jpegConverter;
	PNGConverter pngConverter;
	jpegConverter.Decode(data,size,ImageConverter::Type::RGBA);
	RectF r = { -0.1,0.1,0.1,-0.1 };

	shape = new Image;
	shape->blend = nullptr;
	shape->sampler = new Sampler;
	shape->sampler->SetMagFilter(Sampler::MagFilter::Linear);
	shape->sampler->SetMinFilter(Sampler::MinFilter::Linear);
	shape->sampler->SetWrapModeU(Sampler::WarpMode::Repeat);
	shape->sampler->SetWrapModeV(Sampler::WarpMode::Repeat);
	shape->mat = Matrix::GetScale(0.5625, 1);

	unsigned* data2 = (unsigned*)jpegConverter.GetOutputData();

	shape->Build(data2, jpegConverter.GetWidth(), jpegConverter.GetHeight(),r);

	instance = new ImageInstance;
	instance->blend = nullptr;
	instance->sampler = shape->sampler;

	instance->Build(data2, jpegConverter.GetWidth(), jpegConverter.GetHeight(), r);

	delete[] data;
	delete[] data2;

	button = new Button;
	button->blend = nullptr;
	button->sampler = shape->sampler;
	button->collision = new RectCollision();
	((RectCollision*)button->collision)->rect = RectF(-0.6, -0.4, 0.1, -0.1);

	file.Load(L"ButtonUp.png", false, true);
	size = file.GetSize();
	data = new char[size];
	file.ReadBytes(size, data);
	pngConverter.Decode(data, size, ImageConverter::Type::RGBA);
	data2 = (unsigned*)pngConverter.GetOutputData();
	button->UpBuild(data2, pngConverter.GetWidth(), pngConverter.GetHeight(), r);

	delete[]data;
	delete[]data2;
	file.Close();
	file.Load(L"ButtonOver.png", false, true);
	size = file.GetSize();
	data = new char[size];
	file.ReadBytes(size, data);
	pngConverter.Decode(data, size, ImageConverter::Type::RGBA);
	data2 = (unsigned*)pngConverter.GetOutputData();
	button->OverBuild(data2, pngConverter.GetWidth(), pngConverter.GetHeight(), r);

	delete[]data;
	delete[]data2;
	file.Close();
	file.Load(L"ButtonDown.png", false, true);
	size = file.GetSize();
	data = new char[size];
	file.ReadBytes(size, data);
	pngConverter.Decode(data, size, ImageConverter::Type::RGBA);
	data2 = (unsigned*)pngConverter.GetOutputData();
	button->DownBuild(data2, pngConverter.GetWidth(), pngConverter.GetHeight(), r);

	delete[]data;
	delete[]data2;
	file.Close();
	file.Load(L"ButtonDisable.png", false, true);
	size = file.GetSize();
	data = new char[size];
	file.ReadBytes(size, data);
	pngConverter.Decode(data, size, ImageConverter::Type::RGBA);
	data2 = (unsigned*)pngConverter.GetOutputData();
	button->DisableBuild(data2, pngConverter.GetWidth(), pngConverter.GetHeight(), r);

	delete[]data;
	delete[]data2;
	file.Close();

	
	sprite = new AnimateLoopObject;
	sprite->blend = new Blend(Blend::Value::SRC_ALPHA, Blend::Value::ONE_MINUS_SRC_ALPHA);
	sprite->sampler = shape->sampler;
	sprite->mat = Matrix::GetMove(0.5f, 0.f);
	

	void** datas = new void*[10];
	unsigned* widths = new unsigned[10];
	unsigned* heights = new unsigned[10];
	RectF* rects = new RectF[10];
	for (int i = 0; i < 10; i++) {
		Tostringstream ss;
		ss << _T("dead") << i << _T(".png");
		file.Load(ss.str().c_str(), false, true);
		size = file.GetSize();
		data = new char[size];
		file.ReadBytes(size, data);
		pngConverter.Decode(data, size, ImageConverter::Type::RGBA);
		datas[i] = (unsigned*)pngConverter.GetOutputData();

		widths[i] = 200;
		heights[i] = 312;
		rects[i] = RectF(-0.1, 0.1, 0.2, -0.2);

		delete[]data;
		file.Close();
	}
	sprite->Build(10, datas, widths, heights, rects);
	for (int i = 0; i < 10; i++) {
		delete[]datas[i];
	}
	delete[]datas;
	delete[]widths;
	delete[]heights;
	delete[]rects;

	sprite->Play();

	Matrix insPos[64];
	instance->mat = Matrix::GetScale(0.5625, 1);
	button->mat = Matrix::GetMove(-0.5f,0.f);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			insPos[i * 8 + j] = Matrix::GetMove((float)j*0.25f - 0.875f, (float)i*0.25f - 0.875f);
		}
	}
	instance->BuildInstance(200,64, insPos);

	file.Load(L"NanumBarunGothic.ttf", false, true);
	size = file.GetSize();
	data = new char[size];
	file.ReadBytes(size, data);
	file.Close();

	Font::Init();
	Font::CreateCharImages(1000);
	font = new Font(data, size);
	
	label = new Label;
	label->blend = sprite->blend;
	label->sampler = shape->sampler;
	

	unsigned width, height;
	label->PrepareDraw(L"Hello", font, 300, 0xff,&width,&height);

	label->mat = Matrix::GetScale(((float)width/1920.f), ((float)height / 1080.f));

	Sound::Init(100);

	sound = new Sound;

	OGGConverter oggConverter;
	file.Load(L"test.ogg", false, true);
	size = file.GetSize();
	data = new char[size];
	file.ReadBytes(size, data);
	file.Close();

	oggConverter.Decode(data, size);
	delete[] data;

	soundSource.rawData = oggConverter.GetOutputData();
	soundSource.size = oggConverter.GetOutputSize();

	sound->Decode(&soundSource);
	sound->Play(1);

	textBox = new TextBox;
	textBox->blend = sprite->blend;
	textBox->sampler = shape->sampler;

	textBox->PrepareDraw(L"안녕하세요, 반갑습니다. 잘가요. 안녕하세요, 반갑습니다. 잘가요. 안녕하세요, 반갑습니다. 잘가요. 안녕하세요, 반갑습니다. 잘가요. 안녕하세요, 반갑습니다. 잘가요. 안녕하세요,\n 반갑습니다. 잘가요.",
		font, 600, 600, TextBox::Align::Left, TextBox::VerticalAlign::Top, 30, 0xffffff);

	textBox->mat = Matrix::GetScale((600.f / 1920.f), (600.f/ 1080.f));

}
int i = 0;
void Update() {
	System::Clear(false);
	instance->Draw();
	shape->Draw();
	button->Update();
	button->Draw();
	shape->mat = Matrix::GetMatrix2D(0, 0, 0.5625, 1, (float)i / 200.f);
	instance->mat = Matrix::GetMatrix2D(0, 0, 0.5625, 1, (float)i / 200.f);
	i++;

	sprite->Draw();
	sprite->Update();

	label->Draw();
	textBox->Draw();

	System::Render();
}
void Activate(bool _activated, bool _minimized) {

}
void Destroy() {
	delete sound;
	Sound::Release();
	delete[]soundSource.rawData;
	delete label;
	delete textBox;
	delete font;
	Font::FreeCharImages();
	Font::Release();
	delete sprite->blend;
	delete sprite;
	delete shape->sampler;
	delete shape;
	delete instance;
	delete button->collision;
	delete button;
}


int APIENTRY _tWinmain(HINSTANCE _hInstance, HINSTANCE, LPTSTR, int) {
#ifdef _WIN32
	System::WindowCreate(_hInstance);
#elif __ANDROID__
#endif
	System::RendererCreate();

	System::WindowInfo winInfo;
	winInfo.cursorResource = nullptr;
	winInfo.maximized = false;
	winInfo.minimized = false;
	winInfo.resizeWindow = false;
	winInfo.screenMode = System::ScreenMode::Fullscreen;
	winInfo.title = _T("Test");
	winInfo.windowHeight = 1080;
	winInfo.windowWidth = 1920;
	winInfo.windowShow = System::WindowShow::Default;
	winInfo.windowPos.x = CW_USEDEFAULT;
	winInfo.windowPos.y = CW_USEDEFAULT;

	System::updateFuncs = Update;
	System::activateFunc = Activate;
	System::destroyFunc = Destroy;

	System::WindowInit(&winInfo);

	System::RendererInfo renderInfo;

	renderInfo.msaaCount = 1;
	renderInfo.vSync = true;
	System::RendererInit(&renderInfo);

	Init();

	System::Loop();

	System::RendererRelease();
	System::WindowRelease();
	return 0;
}