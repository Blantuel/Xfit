#pragma once

#include "../system/System.h"
#include "CenterPointPos.h"

#include "../_system/_Renderer.h"

inline float originalWindowWidth, originalWindowHeight;

inline float WindowWidthRatio() {
	return (float)System::GetWindowWidth() / originalWindowWidth;
}
inline float WindowHeightRatio() {
	return (float)System::GetWindowHeight() / originalWindowHeight;
}

inline bool IsWindowWidthRatio() {
	return (float)System::GetWindowWidth() / (float)System::GetWindowHeight() > originalWindowWidth / originalWindowHeight;
}

//originalWindowWidth, originalWindowHeight 설정하고 호출
inline void InitWindowRatio() {
	if ((float)System::GetWindowWidth() / (float)System::GetWindowHeight() > originalWindowWidth / originalWindowHeight) {
		_System::_Renderer::windowRatio = (float)System::GetWindowHeight() / originalWindowHeight;
	} else _System::_Renderer::windowRatio = (float)System::GetWindowWidth() / originalWindowWidth;
}

inline float WindowRatio() {
	return _System::_Renderer::windowRatio;
}


inline const PointF WindowSizeRatioPoint() {
	return PointF(WindowWidthRatio(), WindowHeightRatio());
}

inline const PointF WindowRatioPoint() {
	const float windowRatio = WindowRatio();
	return PointF(windowRatio, windowRatio);
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
inline float MouseYToY(float _y) {
	return -_y + (float)System::GetWindowHeight() / 2.f;
}
inline float GetLineWidth(float _width = 2.f) {
	return WindowRatio() * _width < 1.f ? 1.f : WindowRatio() * _width;
}

inline float MouseXToX(int _x) {
	return (float)_x - (float)System::GetWindowWidth() / 2.f;
}
inline float MouseYToY(int _y) {
	return -(float)_y + (float)System::GetWindowHeight() / 2.f;
}
const PointF PixelPerfectPoint(PointF _p, unsigned _width, unsigned _height, CenterPointPos _centerPointPos, bool _isVertical = false);


class Vertex;

namespace ObjectVertex {
	inline Vertex* topRight;
	inline Vertex* topLeft;
	inline Vertex* bottomRight;
	inline Vertex* bottomLeft;
	inline Vertex* left;
	inline Vertex* right;
	inline Vertex* top;
	inline Vertex* bottom;
}


Vertex* SelectVertex2D(CenterPointPos _centerPointPos);