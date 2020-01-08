#include "ScaleHeader.h"

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
	case CenterPointPos::TopRight:return ObjectVertex::topRight;
	case CenterPointPos::TopLeft:return ObjectVertex::topLeft;
	case CenterPointPos::BottomRight:return ObjectVertex::bottomRight;
	case CenterPointPos::BottomLeft:return ObjectVertex::bottomLeft;
	case CenterPointPos::Left:return ObjectVertex::left;
	case CenterPointPos::Right:return ObjectVertex::right;
	case CenterPointPos::Top:return ObjectVertex::top;
	case CenterPointPos::Bottom:return ObjectVertex::bottom;
	}
}