#pragma once

#include <component/Slider.h>
#include <object/LabelImage.h>

#include <text/SizeLabel.h>
#include <resource/ShapeVertex.h>
#include <text/Font.h>
#include <object/Shape.h>

class LabelButton;
class LabelToggleButton;

class Option {
	bool outClick;

	LabelToggleButton* toggleButton;
	LabelToggleButton* toggleScreenModeButton;
	LabelToggleButton* toggleTextAniButton;
	LabelImage* bgVolImage;
	LabelImage* effectVolImage;
	LabelImage* screenModeImage;
	LabelImage* pxImage;
	LabelButton* resetButton;
	LabelImage* textAniImage;

	unsigned optionBaseSize[1];
	unsigned optionSmallSize[1];
	unsigned resetBaseSize[1];
	unsigned uiBaseSize[1];

	FontContainer toggleFontContainer[1];
	FontColor toggleFontColor[1];
	FontSize toggleFontSize[1];

	FontContainer fontContainer[1];
	FontColor fontColor[1];
	FontSize fontSize[1];

	FontContainer resetFontContainer[1];
	FontColor resetFontColor[1];
	FontSize resetFontSize[1];
	
	SizeLabel toggleLabel;
	SizeLabel bgVolLabel;
	SizeLabel effectVolLabel;
	SizeLabel screenModeLabel;
	SizeLabel pxLabel;
	SizeLabel toggleScreenModeLabel;
	SizeLabel resetLabel;
	SizeLabel toggleTextAniLabel;
	SizeLabel textAniLabel;

	ShapeVertex rectShapeVertex;
	PointF rectShapeVertexData[4];
	Shape rectShape;

	Slider* bgVolSlider;
	Slider* effectVolSlider;
	Slider* pxSlider;

	Frame barFrame;
	Frame stickFrame;

	bool sized;

	void OnSize();
public:
	constexpr static unsigned Width = 1100;
	constexpr static unsigned Height = 1400;
	constexpr static unsigned optionLabelPx = 60;
	constexpr static unsigned optionSmallLabelPx = 40;
	constexpr static unsigned resetLabelPx = 45;

	Option();
	~Option();

	bool Update();
	void Draw();
	void Size();
	bool Reset();
};