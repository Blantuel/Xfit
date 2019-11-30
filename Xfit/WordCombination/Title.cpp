#include "Title.h"

#include "main.h"
#include "LabelImage.h"
#include "FadeLabelButton.h"


#include <system/System.h>
#include <file/File.h>
#include <decoder/OGGDecoder.h>

static void Exit(FadeLabelButton* _target, void* _data) {
	System::Exit();
}
Title::Title() {
	titleLabel.fonts = titleFontContainer;
	titleLabel.fonts[0].font = font;
	titleLabel.fonts[0].len = 0;

	titleLabel.colors = titleFontColor;
	titleLabel.colors[0].color = 0;
	titleLabel.colors[0].len = 0;

	titleLabel.sizes = titleFontSize;
	titleLabel.sizes[0].len = 0;

	titleLabel.baseSizes = titleBaseSize;
	titleLabel.sizeLen = 1;
	titleBaseSize[0] = titlePx;

	titleLabel.renders = fontRender;

	titleLabel.text = L"The Record";

	titleImage = new LabelImage(PosType::Stretch, &titleLabel, PointF(-300.f, 0.f),CenterPointPos::Center, Math::DIVPI_180F * -90.f,true);


	titleButtonBaseSize[0] = titleButtonPx;
	startLabel.fonts = startFontContainer;
	startLabel.fonts[0].font = font;
	startLabel.fonts[0].len = 0;

	startLabel.colors = startFontColor;
	startLabel.colors[0].color = 0;
	startLabel.colors[0].len = 0;

	startLabel.sizes = startFontSize;
	startLabel.sizeLen = 1;
	startLabel.sizes[0].len = 0;

	startLabel.baseSizes = titleButtonBaseSize;

	startLabel.renders = fontRender;

	startLabel.text = L"시작";

	startButton = new FadeLabelButton(PosType::Stretch, &startLabel, PointF(300.f, 140.f), CenterPointPos::Center);


	statsLabel.fonts = statsFontContainer;
	statsLabel.fonts[0].font = font;
	statsLabel.fonts[0].len = 0;

	statsLabel.colors = statsFontColor;
	statsLabel.colors[0].color = 0;
	statsLabel.colors[0].len = 0;

	statsLabel.sizes = statsFontSize;
	statsLabel.sizeLen = 1;
	statsLabel.sizes[0].len = 0;

	statsLabel.baseSizes = titleButtonBaseSize;

	statsLabel.renders = fontRender;

	statsLabel.text = L"통계";

	statsButton = new FadeLabelButton(PosType::Stretch, &statsLabel, PointF(300.f, 0.f), CenterPointPos::Center);


	exitLabel.fonts = exitFontContainer;
	exitLabel.fonts[0].font = font;
	exitLabel.fonts[0].len = 0;

	exitLabel.colors = exitFontColor;
	exitLabel.colors[0].color = 0;
	exitLabel.colors[0].len = 0;

	exitLabel.sizes = exitFontSize;
	exitLabel.sizeLen = 1;
	exitLabel.sizes[0].len = 0;

	exitLabel.baseSizes = titleButtonBaseSize;

	exitLabel.renders = fontRender;
	
	exitLabel.text = L"종료";

	exitButton = new FadeLabelButton(PosType::Stretch, &exitLabel, PointF(300.f, -140.f), CenterPointPos::Center);
	exitButton->faded = Exit;


	/*

	File file("The Record.ogg");

	unsigned fileSize = file.GetSize();
	void* bgSoundData = new unsigned char[fileSize];
	file.ReadBytes(fileSize, bgSoundData);

	OGGDecoder decoder;
	decoder.LoadHeader(bgSoundData, fileSize);

	bgSoundOutData = new unsigned char[decoder.GetOutputSize()];
	decoder.Decode(bgSoundOutData);

	delete[]bgSoundData;

	source = { bgSoundOutData,decoder.GetOutputSize() };
	bgSound->Decode(&source);
	bgSound->Play(0);
	*/
}
Title::~Title() {
	delete titleImage;

	delete startButton;
	delete statsButton;
	delete exitButton;
}
void Title::Size() {
	titleImage->Size();
	startButton->Size();
	statsButton->Size();
	exitButton->Size();
}
bool Title::Update() {
	bool result = startButton->Update();
	result = statsButton->Update() ? true : result;
	result = exitButton->Update() ? true : result;

	return result;
}
void Title::Draw() {
	titleImage->Draw();

	startButton->Draw();
	statsButton->Draw();
	exitButton->Draw();
}
void Title::Disable(bool _disable) {
	startButton->Disable(_disable);
	statsButton->Disable(_disable);
	exitButton->Disable(_disable);
}