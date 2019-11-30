#pragma once

#include <system/System.h>
#include <sound/Sound.h>
#include <resource/CenterPointPos.h>

extern mt19937_64 random;


constexpr unsigned uiButtonPx = 40;
constexpr unsigned windowWidth = 1280;
constexpr unsigned windowHeight = 720;
constexpr float windowWidthF = 1280.f;
constexpr float windowHeightF = 720.f;

class Font;

inline Font* font;
inline Font* boldFont;

enum class Scene {
	Title
};

enum class PosType {
	Stretch,
	Center
};

inline Scene scene;

class Title;
class Option;

inline Title* title;
inline Option* option;

inline float WindowWidthRatio() {
	return (float)System::GetWindowWidth() / windowWidthF;
}
inline float WindowHeightRatio() {
	return (float)System::GetWindowHeight() / windowHeightF;
}

inline bool IsWindowWidthRatio() {
	return (float)System::GetWindowWidth() / (float)System::GetWindowHeight() > windowWidthF / windowHeightF;
}

inline float WindowRatio() {
	return IsWindowWidthRatio() ? WindowHeightRatio() : WindowWidthRatio();
}

inline const PointF WindowRatioPoint(PosType _posType) {
	if (_posType == PosType::Stretch) {
		return PointF(WindowWidthRatio(), WindowHeightRatio());
	} else if (_posType == PosType::Center) {
		const float windowRatio = WindowRatio();
		return PointF(windowRatio, windowRatio);
	}
}

inline const PointF WindowRatioPoint() {
	const float windowRatio = WindowRatio();
	return PointF(windowRatio, windowRatio);
}

inline const PointF WindowSizeRatioPoint() {
	return PointF(WindowWidthRatio(), WindowHeightRatio());
}

inline float XToMouseX(float _x) {
	return _x + (float)System::GetWindowWidth() / 2.f;
}
inline float YToMouseY(float _y) {
	return -_y + (float)System::GetWindowHeight() / 2.f;
}

inline float MouseXToX(float _x) {
	return _x - (float)System::GetWindowWidth() / 2.f;
}
inline float MouseYTOY(float _y) {
	return -_y + (float)System::GetWindowHeight() / 2.f;
}
inline float GetLineWidth(float _width = 2.f) {
	return WindowRatio() * _width < 1.f ? 1.f : WindowRatio() * _width;
}

inline float MouseXToX(int _x) {
	return (float)_x - (float)System::GetWindowWidth() / 2.f;
}
inline float MouseYTOY(int _y) {
	return -(float)_y + (float)System::GetWindowHeight() / 2.f;
}
const PointF PixelPerfectPoint(PointF _p, unsigned _width, unsigned _height, CenterPointPos _centerPointPos, bool _isVertical = false);

struct FontRender;

inline FontRender* fontRender;

inline float textPx = 1.f;
inline Sound* bgSound;
inline void* bgSoundOutData;

inline bool sizeRequest = true;

void Main_Size(void* _data);

class Vertex;

inline Vertex* lineVertex;

inline Vertex* topRightVertex;
inline Vertex* topLeftVertex;
inline Vertex* bottomRightVertex;
inline Vertex* bottomLeftVertex;
inline Vertex* leftVertex;
inline Vertex* rightVertex;
inline Vertex* topVertex;
inline Vertex* bottomVertex;

Vertex* SelectVertex2D(CenterPointPos _centerPointPos);