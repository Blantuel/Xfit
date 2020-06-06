#pragma once

#include "../math/Point.h"

class RectF {
public:
	float left, right, top, bottom;

	static const RectF MakeRect(float x, float y, float width, float height) {
		RectF rect;

		const int width2 = width / 2;
		const int height2 = height / 2;

		rect.left = -width2 + x;
		rect.right = width2 + x;

		rect.top = height2 + y;
		rect.bottom = -height2 + y;

		return rect;
	}

	RectF() {}
	constexpr RectF(float nLeft, float nRight, float nTop, float nBottom) :left(nLeft), right(nRight), top(nTop), bottom(nBottom) {}
	void SetRect(float nLeft, float nRight, float nTop, float nBottom) {
		left = nLeft;
		right = nRight;
		top = nTop;
		bottom = nBottom;
	}
	void SetTransform(const Matrix& matrix) {
		const PointF p2 = PointF(right, top).SetTransform(matrix);
		const PointF p3 = PointF(left, bottom).SetTransform(matrix);

		left = p3.x;
		right = p2.x;
		top = p2.y;
		bottom = p3.y;
	}
	const PointF GetPoint1() const { return PointF(left, top); }
	const PointF GetPoint2() const { return PointF(right, top); }
	const PointF GetPoint3() const { return PointF(left, bottom); }
	const PointF GetPoint4() const { return PointF(right, bottom); }
	bool IsEmpty() const {
		return (left == right) && (top == bottom);
	}
	bool IsPointIn(const PointF& point)const {
		return (point.x >= left) && (point.x <= right) && (point.y <= top) && (point.y >= bottom);
	}
	bool Compare(const RectF& nRect)const {
		return (nRect.left == left) && (nRect.right == right) && (nRect.top == top) && (nRect.bottom == bottom);
	}
	RectF& Move(float x, float y) {
		left += x;
		right += x;
		top += y;
		bottom += y;
		return *this;
	}
	RectF& MoveRatio(float x, float y) {
		const float leftT = left;
		const float rightT = right;
		const float topT = top;
		const float bottomT = bottom;

		left = (0.5f * x + 0.5f) * leftT + (0.5f * x - 0.5f) * rightT;
		right = (0.5f * x - 0.5f) * leftT + (0.5f * x + 0.5f) * rightT;
		top = (0.5f * y + 0.5f) * topT + (0.5f * y - 0.5f) * bottomT;
		bottom = (0.5f * y - 0.5f) * topT + (0.5f * y + 0.5f) * bottomT;

		return *this;
	}
	void Rotate90() {
		float topT = top;
		float bottomT = bottom;
		top = right;
		bottom = left;
		left = bottomT;
		right = topT;
	}

	RectF& Extend(float x, float y) {
		left -= x / 2.f;
		right += x / 2.f;
		top += y / 2.f;
		bottom -= y / 2.f;
		return *this;
	}
	RectF& ExtendRatio(float x, float y) {
		const float rx = (right - left) / 2.f;
		const float ry = -(bottom - top) / 2.f;
		left -= (rx * x) - rx;
		right += (rx * x) - rx;
		top += (ry * y) - ry;
		bottom -= (ry * y) - ry;
		return *this;
	}
	bool IsRectIn(const RectF& rect)const {
		return (left <= rect.right) && (right >= rect.left) && (top >= rect.bottom) && (bottom <= rect.top);
	}
	bool IsRectInOBB(const RectF& rect, const PointF& dd1, const PointF& dd2)const {
		PointF m((rect.right - rect.left) / 2.f + rect.left, (rect.bottom - rect.top) / 2.f + rect.bottom);
		PointF m2((right - left) / 2.f + left, (bottom - top) / 2.f + bottom);

		PointF d1 = dd1;
		PointF d2(dd1.y, -dd1.x);
		PointF d3 = dd2;
		PointF d4(dd2.y, -dd2.x);

		float a = fabs(d1.InnerProduct(d3)) + fabs(d2.InnerProduct(d3));
		float b = (rect.right - rect.left) / 2.f;
		float c = fabs((m - m2).InnerProduct(d3));
		if (c <= a + b)return false;

		a = fabs(d1.InnerProduct(d4)) + fabs(d2.InnerProduct(d4));
		b = (rect.bottom - rect.top) / 2.f;
		c = fabs((m - m2).InnerProduct(d4));
		if (c <= a + b)return false;

		a = fabs(d3.InnerProduct(d1)) + fabs(d4.InnerProduct(d1));
		b = (right - left) / 2.f;
		c = fabs((m - m2).InnerProduct(d1));
		if (c <= a + b)return false;

		a = fabs(d3.InnerProduct(d2)) + fabs(d4.InnerProduct(d2));
		b = (bottom - top) / 2.f;
		c = fabs((m - m2).InnerProduct(d2));
		if (c <= a + b)return false;

		return true;
	}
	static const RectF And(const RectF& rect1, const RectF& rect2) {
		RectF rect(rect1.left>rect2.left ? rect1.left : rect2.left,
			rect1.right>rect2.right ? rect2.right : rect1.right,
			rect1.top>rect2.top ? rect2.top : rect1.top,
			rect1.bottom>rect2.bottom ? rect1.bottom : rect2.bottom);
		return rect;
	}
	float GetArea()const {
		return (right - left) * (top - bottom);
	}
	static const RectF Or(const RectF& rect1, const RectF& rect2) {
		RectF rect(rect1.left<rect2.left ? rect1.left : rect2.left,
			rect1.right<rect2.right ? rect2.right : rect1.right,
			rect1.top<rect2.top ? rect2.top : rect1.top,
			rect1.bottom<rect2.bottom ? rect1.bottom : rect2.bottom);
		return rect;
	}
	const RectF operator+(const PointF& _point) {
		return RectF(left + _point.x, right + _point.x, top + _point.y, bottom + _point.y);
	}
};


class Rect {
public:
	int left, right, top, bottom;

	static const Rect MakeRect(int x, int y, int width, int height) {
		Rect rect;

		const int width2 = width / 2;
		const int height2 = height / 2;

		rect.left = -width2 + x;
		if (width % 2 == 1) {
			rect.right = width2 + 1 + x;
		} else {
			rect.right = width2 + x;
		}

		rect.top = height2 + y;
		if (height % 2 == 1) {
			rect.bottom = -height2 - 1 + y;
		} else {
			rect.bottom = -height2 + y;
		}

		return rect;
	}

	Rect() {}
	Rect(int nLeft, int nRight, int nTop, int nBottom) :left(nLeft), right(nRight), top(nTop), bottom(nBottom) {}
	void SetRect(int nLeft, int nRight, int nTop, int nBottom) {
		left = nLeft;
		right = nRight;
		top = nTop;
		bottom = nBottom;
	}
	const Point GetPoint1() const { return Point(left, top); }
	const Point GetPoint2() const { return Point(right, top); }
	const Point GetPoint3() const { return Point(left, bottom); }
	const Point GetPoint4() const { return Point(right, bottom); }
	bool IsEmpty() const {
		return (left == right) && (top == bottom);
	}
	bool IsPointIn(const Point& point)const {
		return (point.x >= left) && (point.x <= right) && (point.y <= top) && (point.y >= bottom);
	}
	bool Compare(const Rect& nRect)const {
		return (nRect.left == left) && (nRect.right == right) && (nRect.top == top) && (nRect.bottom == bottom);
	}

	//사각형을 위치를 비율로 곱한 값만큼 이동
	Rect& MoveRatio(float x, float y) {
		const float leftT = left;
		const float rightT = right;
		const float topT = top;
		const float bottomT = bottom;

		left = (int)((0.5f * x + 0.5f) * leftT + (0.5f * x - 0.5f) * rightT);
		right = (int)((0.5f * x - 0.5f) * leftT + (0.5f * x + 0.5f) * rightT);
		top = (int)((0.5f * y + 0.5f) * topT + (0.5f * y - 0.5f) * bottomT);
		bottom = (int)((0.5f * y - 0.5f) * topT + (0.5f * y + 0.5f) * bottomT);

		return *this;
	}

	Rect& Move(int x, int y) {
		left += x;
		right += x;
		top += y;
		bottom += y;

		return *this;
	}
	Rect& Extend(int x, int y) {
		const int xx = x / 2;
		const int yy = y / 2;

		left -= xx;
		if (x % 2 == 1) {
			right += xx + 1;
		} else {
			right += xx;
		}
		
		top += yy;
		if (y % 2 == 1) {
			bottom -= yy + 1;
		} else {
			bottom -= yy;
		}
		return *this;
	}
	Rect& ExtendRatio(float x, float y) {
		const float rx = (float)(right - left) / 2.f;
		const float ry = -(float)(bottom - top) / 2.f;
		left -= (int)((rx * x) - rx);
		right += (int)((rx * x) - rx);
		top += (int)((ry * y) - ry);
		bottom -= (int)((ry * y) - ry);
		return *this;
	}
	bool IsRectIn(const Rect& rect)const {
		return (left <= rect.right) && (right >= rect.left) && (top >= rect.bottom) && (bottom <= rect.top);
	}
	static const Rect And(const RectF& rect1, const RectF& rect2) {
		Rect rect(rect1.left > rect2.left ? rect1.left : rect2.left,
			rect1.right > rect2.right ? rect2.right : rect1.right,
			rect1.top > rect2.top ? rect2.top : rect1.top,
			rect1.bottom > rect2.bottom ? rect1.bottom : rect2.bottom);
		return rect;
	}
	float GetArea()const {
		return (right - left) * (top - bottom);
	}
	static const Rect Or(const Rect& rect1, const Rect& rect2) {
		Rect rect(rect1.left < rect2.left ? rect1.left : rect2.left,
			rect1.right < rect2.right ? rect2.right : rect1.right,
			rect1.top < rect2.top ? rect2.top : rect1.top,
			rect1.bottom < rect2.bottom ? rect1.bottom : rect2.bottom);
		return rect;
	}
	const Rect operator+(const Point& _point) {
		return Rect(left + _point.x, right + _point.x, top + _point.y, bottom + _point.y);
	}
};

