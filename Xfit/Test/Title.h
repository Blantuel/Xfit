#pragma once

#include "SizeLabel.h"
#include <resource/Vertex.h>
#include <text/Font.h>

class FadeLabelButton;
class LabelImage;

class Title {
	FontContainer titleFontContainer[1];
	FontColor titleFontColor[1];
	FontSize titleFontSize[1];
	unsigned titleBaseSize[1];
	unsigned titleButtonBaseSize[1];

	FontContainer startFontContainer[1];
	FontColor startFontColor[1];
	FontSize startFontSize[1];
	

	FontContainer statsFontContainer[1];
	FontColor statsFontColor[1];
	FontSize statsFontSize[1];


	FontContainer exitFontContainer[1];
	FontColor exitFontColor[1];
	FontSize exitFontSize[1];


	LabelImage* titleImage;

	FadeLabelButton* startButton;
	FadeLabelButton* statsButton;
	FadeLabelButton* exitButton;

	SizeLabel titleLabel;

	SizeLabel startLabel;
	SizeLabel statsLabel;
	SizeLabel exitLabel;
public:
	constexpr static unsigned titlePx = 65;
	constexpr static unsigned titleButtonPx = 50;

	Title();
	~Title();

	void Size();
	bool Update();//다시 그려야 할 때 true 반환
	void Draw();

	void Disable(bool _disable);
};

