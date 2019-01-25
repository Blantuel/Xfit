#pragma once

#include "../math/Point.h"

class RectF {
public:
	float left, right, top, bottom;
	RectF() {}
	RectF(float nLeft, float nRight, float nTop, float nBottom) :left(nLeft), right(nRight), top(nTop), bottom(nBottom) {}
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
		left += rx - (rx*x);
		right -= rx - (rx*x);
		top += -(ry - (ry*x));
		bottom -= -(ry - (ry*x));
		return *this;
	}
	bool IsRectIn(const RectF& rect)const {
		return (left <= rect.right) && (right >= rect.left) && (top >= rect.bottom) && (bottom <= rect.top);
	}
	bool IsRectInOBB(const RectF& rect, const Point& dd1, const Point& dd2)const {
		Point m((rect.right - rect.left) / 2.f + rect.left, (rect.bottom - rect.top) / 2.f + rect.bottom);
		Point m2((right - left) / 2.f + left, (bottom - top) / 2.f + bottom);

		Point d1 = dd1;
		Point d2(dd1.y, -dd1.x);
		Point d3 = dd2;
		Point d4(dd2.y, -dd2.x);

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
			rect1.top>rect2.top ? rect1.top : rect2.top,
			rect1.bottom>rect2.bottom ? rect2.bottom : rect1.bottom);
		return rect;
	}
	static const RectF Or(const RectF& rect1, const RectF& rect2) {
		RectF rect(rect1.left<rect2.left ? rect1.left : rect2.left,
			rect1.right<rect2.right ? rect2.right : rect1.right,
			rect1.top<rect2.top ? rect1.top : rect2.top,
			rect1.bottom<rect2.bottom ? rect2.bottom : rect1.bottom);
		return rect;
	}
};
