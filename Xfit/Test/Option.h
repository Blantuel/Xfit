#pragma once

#include "Slider.h"
#include "LabelImage.h"

#include "SizeLabel.h"
#include <resource/ShapeVertex.h>
#include <text/Font.h>

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

	Slider bgVolSlider;
	Slider effectVolSlider;
	Slider pxSlider;

	bool sized;

	void OnSize();
public:
	constexpr static unsigned Width = 350;
	constexpr static unsigned Height = 450;
	constexpr static unsigned optionLabelPx = 20;
	constexpr static unsigned resetLabelPx = 15;

	Option();
	~Option();

	bool Update();
	void Draw();
	void Size();
	bool Reset();
};