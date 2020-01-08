#include "Option.h"
#include "main.h"
#include <component/LabelToggleButton.h>
#include <system/System.h>
#include <system/Input.h>
#include <file/File.h>
#include <decoder/PNGDecoder.h>

#include "Title.h"

static PointU windowSize = PointU(1280,720);
static Point windowPos = Point(200, 200);
static System::WindowState windowState = System::WindowState::Restore;

static bool OptionButtonUp(Button* _target, Point _mousePos, void* _data) {
	switch (scene) {
	case Scene::Title:
		title->Disable(((LabelToggleButton*)_target)->IsToggle());
		break;
	}
	return false;
}
static bool ResetButtonUp(Button* _target, Point _mousePos, void* _data) {
	return option->Reset();
}
bool Option::Reset() {
	bool result = false;
	if (bgVolSlider->GetValue() != 0.5f) {
		bgVolSlider->SetValue(0.5f);
		result = true;
	}
	if (effectVolSlider->GetValue() != 0.5f) {
		effectVolSlider->SetValue(0.5f);
		result = true;
	}
	if (pxSlider->GetValue() != 0.5f) {
		pxSlider->SetValue(0.5f);
		result = true;
	}
	return result;
}
static bool ToggleScreenModeButtonUp(Button* _target, Point _mousePos, void* _data) {
	if (((LabelToggleButton*)_target)->IsToggle()) {
		((LabelToggleButton*)_target)->GetLabel()->text = L"전체화면";

		windowPos = System::GetWindowPos();
		windowSize = System::GetWindowSize();
		windowState = System::GetWindowState();

		const int displayIndex = System::GetDisplayFromWindow();
		System::SetFullScreenMode(displayIndex, System::GetCurrentDisplayModeIndex(displayIndex));
	} else {
		((LabelToggleButton*)_target)->GetLabel()->text = L"창";

		const int displayIndex = System::GetDisplayFromWindow();
		System::SetWindowMode(windowSize, windowPos, windowState, true, true, true);
	}
	return true;
}
static bool ToggleTextAniButtonUp(Button* _target, Point _mousePos, void* _data) {
	if (((LabelToggleButton*)_target)->IsToggle()) {
		((LabelToggleButton*)_target)->GetLabel()->text = L"켜짐";
	} else {
		((LabelToggleButton*)_target)->GetLabel()->text = L"꺼짐";
	}
	((LabelToggleButton*)_target)->PrepareDraw(textPx);
	return true;
}
static void BgVolControlling(Slider* _target) {
	bgSound->SetVolume(_target->GetValue());
}
static void PxControlling(Slider* _target) {
	textPx = _target->GetValue() + 0.5f;
	Main_Size(nullptr);
}

Option::Option():
rectShape(PointF(0.f,0.f), WindowRatioPoint(), 0,nullptr,&rectShapeVertex,Point3DwF(1.f,1.f,1.f,1.f), Point3DwF(0.f, 0.f, 0.f, 1.f)),
outClick(false),sized(false)
{
	File file;
	PNGDecoder decoder;

	file.Open("bar.png");
	unsigned size = file.GetSize();
	unsigned char* data = new unsigned char[size];
	file.ReadBytes(size, data);

	decoder.LoadHeader(data, size);
	unsigned char* outData = new unsigned char[decoder.GetOutputSize()];
	decoder.Decode(outData);

	barFrame.Build(outData, decoder.GetWidth(), decoder.GetHeight());

	delete[]data;
	delete[]outData;

	file.Close();
	file.Open("stick.png");
	size = file.GetSize();
	data = new unsigned char[size];
	file.ReadBytes(size, data);

	decoder.LoadHeader(data, size);
	outData = new unsigned char[decoder.GetOutputSize()];
	decoder.Decode(outData);

	stickFrame.Build(outData, decoder.GetWidth(), decoder.GetHeight());

	delete[]data;
	delete[]outData;

	bgVolSlider = new Slider(PosType::Center, &barFrame, &stickFrame, PointF(200.f, 600.f), 0.5f);
	effectVolSlider = new Slider(PosType::Center, &barFrame, &stickFrame, PointF(200.f, 400.f), 0.5f);
	pxSlider = new Slider(PosType::Center, &barFrame, &stickFrame, PointF(200.f, 200.f), 0.5f);


	toggleLabel.fonts = toggleFontContainer;
	toggleLabel.fonts[0].font = font;
	toggleLabel.fonts[0].len = 0;

	toggleLabel.colors = toggleFontColor;
	toggleLabel.colors[0].color = 0;
	toggleLabel.colors[0].len = 0;

	toggleLabel.sizes = toggleFontSize;
	toggleLabel.sizes[0].len = 0;
	toggleLabel.sizeLen = 1;
	toggleLabel.baseSizes = uiBaseSize;

	uiBaseSize[0] = uiButtonPx;
	optionBaseSize[0] = optionLabelPx;
	optionSmallSize[0] = optionSmallLabelPx;
	resetBaseSize[0] = resetLabelPx;

	toggleLabel.renders = fontRender;

	toggleLabel.text = L"옵션";

	toggleButton = new LabelToggleButton(PosType::Stretch, &toggleLabel, PointF(originalWindowWidth / 2.f, originalWindowHeight / 2.f), CenterPointPos::TopRight);

	toggleButton->buttonUp = OptionButtonUp;

	bgVolLabel.fonts = fontContainer;
	bgVolLabel.fonts[0].font = font;
	bgVolLabel.fonts[0].len = 0;

	bgVolLabel.colors = fontColor;
	bgVolLabel.colors[0].color = 0;
	bgVolLabel.colors[0].len = 0;

	bgVolLabel.sizes = fontSize;
	bgVolLabel.sizes[0].len = 0;
	bgVolLabel.sizeLen = 1;
	bgVolLabel.baseSizes = optionBaseSize;

	bgVolLabel.renders = fontRender;

	bgVolLabel.text = L"배경음";

	bgVolImage = new LabelImage(PosType::Center, &bgVolLabel, PointF(-300.f, 590.f));
	bgVolSlider->controlling = BgVolControlling;


	effectVolLabel.fonts = fontContainer;

	effectVolLabel.colors = fontColor;

	effectVolLabel.sizes = fontSize;
	effectVolLabel.sizeLen = 1;
	effectVolLabel.baseSizes = optionBaseSize;

	effectVolLabel.renders = fontRender;

	effectVolLabel.text = L"효과음";
	effectVolImage = new LabelImage(PosType::Center, &effectVolLabel,  PointF(-300.f, 390.f));


	pxLabel.fonts = fontContainer;

	pxLabel.colors = fontColor;

	pxLabel.sizes = fontSize;
	pxLabel.sizeLen = 1;
	pxLabel.baseSizes = optionBaseSize;

	pxLabel.renders = fontRender;

	pxLabel.text = L"글자크기";
	pxImage = new LabelImage(PosType::Center, &pxLabel,  PointF(-300.f, 190.f));
	pxSlider->controlling = PxControlling;


	screenModeLabel.fonts = fontContainer;

	screenModeLabel.colors = fontColor;

	screenModeLabel.sizes = fontSize;
	screenModeLabel.sizeLen = 1;
	screenModeLabel.baseSizes = optionBaseSize;

	screenModeLabel.renders = fontRender;

	screenModeLabel.text = L"화면모드";
	screenModeImage = new LabelImage(PosType::Center, &screenModeLabel,  PointF(-300.f, -10.f));

	toggleScreenModeLabel.fonts = fontContainer;

	toggleScreenModeLabel.colors = fontColor;

	toggleScreenModeLabel.sizes = fontSize;
	toggleScreenModeLabel.sizeLen = 1;
	toggleScreenModeLabel.baseSizes = optionBaseSize;
	toggleScreenModeLabel.renders = fontRender;

	toggleScreenModeLabel.text = L"창";

	toggleScreenModeButton = new LabelToggleButton(PosType::Center, &toggleScreenModeLabel,  PointF(300.f, -10.f));

	toggleScreenModeButton->buttonUp = ToggleScreenModeButtonUp;
	//toggleScreenModeButton->SetToogle(true);

	textAniLabel.fonts = fontContainer;

	textAniLabel.colors = fontColor;

	textAniLabel.sizes = fontSize;
	textAniLabel.sizeLen = 1;
	textAniLabel.baseSizes = optionSmallSize;

	textAniLabel.renders = fontRender;

	textAniLabel.text = L"텍스트 애니메이션";
	textAniImage = new LabelImage(PosType::Center, &textAniLabel, PointF(-300.f, -210.f));

	toggleTextAniLabel.fonts = fontContainer;

	toggleTextAniLabel.colors = fontColor;

	toggleTextAniLabel.sizes = fontSize;

	toggleTextAniLabel.renders = fontRender;

	toggleTextAniLabel.text = L"켜짐";
	toggleTextAniLabel.sizeLen = 1;
	toggleTextAniLabel.baseSizes = optionBaseSize;

	toggleTextAniButton = new LabelToggleButton(PosType::Center, &toggleTextAniLabel, PointF(300.f, -210.f));

	toggleTextAniButton->buttonUp = ToggleTextAniButtonUp;

	toggleTextAniButton->SetToogle(true);


	resetLabel.fonts = resetFontContainer;
	resetLabel.fonts[0].font = font;
	resetLabel.fonts[0].len = 0;

	resetLabel.colors = resetFontColor;
	resetLabel.colors[0].color = 0;
	resetLabel.colors[0].len = 0;

	resetLabel.sizes = resetFontSize;
	resetLabel.sizes[0].pixelSize = resetLabelPx;
	resetLabel.sizes[0].len = 0;
	resetLabel.sizeLen = 1;
	resetLabel.baseSizes = resetBaseSize;

	resetLabel.renders = fontRender;

	resetLabel.text = L"초기화";

	resetButton = new LabelButton(PosType::Center, &resetLabel, PointF(400.f, 100.f));

	resetButton->buttonUp = ResetButtonUp;

	rectShapeVertex.vertices.SetData(rectShapeVertexData, 0, 4);

	rectShapeVertex.MakeImageVertex2D(PointF((float)Width, (float)Height));

	rectShapeVertex.Build();

	rectShape.lineWidth = GetLineWidth(7);
}
bool Option::Update() {
	bool result = false;
	if (toggleButton->IsToggle()) {
		result = bgVolSlider->Update() ? true : result;
		result = effectVolSlider->Update() ? true : result;
		result = pxSlider->Update() ? true : result;

		result = toggleScreenModeButton->Update() ? true : result;
		result = toggleTextAniButton->Update() ? true : result;
		result = resetButton->Update() ? true : result;

		if (Input::IsLMouseClicked() && outClick) {
			outClick = false;
			const Point mousePos = Input::GetMousePosScreen();
			const PointF mousePosF = PointF((float)mousePos.x - (float)System::GetWindowWidth() / 2.f, -mousePos.y + (float)System::GetWindowHeight() / 2.f);

			RectF rect(rectShapeVertex.vertices[0].x * WindowRatio(), rectShapeVertex.vertices[1].x * WindowRatio(), rectShapeVertex.vertices[0].y * WindowRatio(), rectShapeVertex.vertices[2].y * WindowRatio());

			if (!rect.IsPointIn(mousePosF)) {
				toggleButton->ButtonUp(mousePos, nullptr);
				OptionButtonUp(toggleButton, mousePos, nullptr);
				return true;
			}
		} else if (Input::IsLMouseClick()) {
			const Point mousePos = Input::GetMousePosScreen();
			const PointF mousePosF = PointF((float)mousePos.x - (float)System::GetWindowWidth() / 2.f, -mousePos.y + (float)System::GetWindowHeight() / 2.f);

			RectF rect(rectShapeVertex.vertices[0].x * WindowRatio(), rectShapeVertex.vertices[1].x * WindowRatio(), rectShapeVertex.vertices[0].y * WindowRatio(), rectShapeVertex.vertices[2].y * WindowRatio());

			if (!rect.IsPointIn(mousePosF))outClick = true;
		}
		return result;
	}
	return toggleButton->Update();
}
void Option::Draw() {
	toggleButton->Draw();
	if (toggleButton->IsToggle()) {//옵션이 켜졌을 때
		if (sized) {
			OnSize();
		}
		rectShape.Draw();

		bgVolSlider->Draw();
		bgVolImage->Draw();

		effectVolSlider->Draw();
		effectVolImage->Draw();

		pxSlider->Draw();
		pxImage->Draw();

		screenModeImage->Draw();
		toggleScreenModeButton->Draw();
		resetButton->Draw();

		textAniImage->Draw();
		toggleTextAniButton->Draw();
	}
}
void Option::OnSize() {
	rectShape.lineWidth = GetLineWidth(7);
	rectShape.SetScale(WindowRatioPoint());


	bgVolSlider->Size();
	bgVolImage->Size(textPx);

	effectVolSlider->Size();
	effectVolImage->Size(textPx);

	pxSlider->Size();
	pxImage->Size(textPx);

	screenModeImage->Size(textPx);
	toggleScreenModeButton->Size(true, textPx);

	textAniImage->Size(textPx);
	toggleTextAniButton->Size(true, textPx);

	resetButton->Size(true, textPx);

	sized = false;
}
void Option::Size() {
	toggleButton->Size();

	if (toggleButton->IsToggle()) {//옵션이 켜졌을 때
		OnSize();
	} else {
		sized = true;
	}
}
Option::~Option() {
	delete toggleButton;
	delete bgVolImage;
}