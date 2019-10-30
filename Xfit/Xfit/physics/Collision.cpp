#include "Collision.h"
#include "../data/Array.h"

bool CollisionF::CollisionCircle(const PointF& a, const PointF& b, float r1, float r2) { return a.DistancePow(b) <= (r1 + r2)*(r1 + r2); }
bool CollisionF::CollisionCircleInPoint(const PointF& c, float r, const PointF& p) { return c.DistancePow(p) <= r * r; }
bool CollisionF::CollisionRectInLine(const RectF& rect, const PointF& a1, const PointF& a2, bool lineOnlyCollision/* = true*/) {
	if (!lineOnlyCollision) {
		if (rect.IsPointIn(a1) || rect.IsPointIn(a2))return true;
	}
	PointF b1(rect.left, rect.top), b2(rect.right, rect.top);

	if (CollisionLine(a1, a2, b1, b2))return true;


	b1.x = rect.left;
	b1.y = rect.top;
	b2.x = rect.left;
	b2.y = rect.bottom;

	if (CollisionLine(a1, a2, b1, b2))return true;

	b1.x = rect.right;
	b1.y = rect.top;
	b2.x = rect.right;
	b2.y = rect.bottom;

	if (CollisionLine(a1, a2, b1, b2))return true;

	b1.x = rect.left;
	b1.y = rect.bottom;
	b2.x = rect.right;
	b2.y = rect.bottom;

	if (CollisionLine(a1, a2, b1, b2))return true;

	return false;
}
bool CollisionF::CollisionLine(const PointF& a1, const PointF& a2, const PointF& b1, const PointF& b2, PointF* resultPoint) {
	float ax = a2.x - a1.x,
		ay = a2.y - a1.y,
		bx = b2.x - b1.x,
		by = b2.y - b1.y,
		abx = a1.x - b1.x,
		aby = a1.y - b1.y;

	float aba = ax*by - bx*ay;

	if (aba == 0)aba = 0.0001f;
	float A = (bx*aby - by*abx) / aba;
	float B = (ax*aby - ay*abx) / aba;
	if ((A <= 1) && (B <= 1) & (A >= 0) && (B >= 0)) {
		if (resultPoint) {
			resultPoint->x = a1.x + A*(a2.x - a1.x);
			resultPoint->y = a1.y + A*(a2.y - a1.y);
		}
		return true;
	} else {
		return false;
	}
}
bool CollisionF::CollisionLinesInLine(const LinesF& points, const PointF& a, const PointF& b) {
	for (unsigned i = 0; i < points.points.Size() - 1; ++i) {
		if (CollisionLine(points.points[i], points.points[i + 1], a, b))return true;
	}
	return false;
}
bool CollisionF::CollisionLines(const LinesF& points1, const LinesF& points2) {
	for (unsigned i = 0; i < points1.points.Size() - 1; ++i) {
		for (unsigned j = 0; j < points2.points.Size() - 1; ++j) {
			if (CollisionLine(points1.points[i],points1.points[i + 1], points2.points[j], points2.points[j + 1]))return true;
		}
	}
	return false;
}
bool CollisionF::CollisionHexagonInLine(const HexagonF& points, const PointF& a, const PointF& b, bool lineOnlyCollision/* = false*/) {
	if ((!lineOnlyCollision) && CollisionHexagonInPoint(points, a))return true;
	for (unsigned i = 0; i < points.points.Size() - 1; ++i) {
		if (CollisionLine(points.points[i], points.points[i + 1], a, b))return true;
	}
	if (CollisionLine(points.points[points.points.Size() - 1], points.points[0], a, b))return true;
	return false;
}
bool CollisionF::CollisionTriangleInPoint(const PointF& t1, const PointF& t2, const PointF& t3, const PointF& p) {
	const float a = (t1.x - p.x)*(t3.y - p.y) - (t3.x - p.x)*(t1.y - p.y);
	const float b = (t3.x - p.x)*(t2.y - p.y) - (t2.x - p.x)*(t3.y - p.y);
	const float c = (t2.x - p.x)*(t1.y - p.y) - (t1.x - p.x)*(t2.y - p.y);
	int co = 0;
	if (a == 0)co++; if (b == 0)co++; if (c == 0)co++;

	return co == 0 || co == 2;
}
bool CollisionF::CollisionCircleInLine(const PointF& _l1, const PointF& _l2, const PointF& cPos, float r, bool lineOnlyCollision/* = true*/) {
	PointF l1 = _l1, l2 = _l2;
	if (!lineOnlyCollision) {
		if (CollisionCircleInPoint(cPos, r, l1)) {
			return true;
		}
	}

	float a = l2.y - l1.y;
	float b = l1.x - l2.x;

	if (a == 0) {
		if (b == 0) return CollisionCircleInPoint(l1, r, cPos);
		a = 0.00001f;
		l1.y -= 0.00001f;
	} else if (b == 0) {
		b = 0.00001f;
		l2.x -= 0.00001f;
	}

	float c = l2.x*l1.y - l1.x*l2.y;
	float A = (a*a) / (b*b) + 1, B = (2 * a*c) / (b*b) + (2 * cPos.y*a) / b - 2 * cPos.x, C = (c*c) / (b*b) + (2 * cPos.y*c) / b + (cPos.x*cPos.x + cPos.y*cPos.y - r*r);
	float S = B*B - 4 * A*C;

	if (S >= 0) {
		S = sqrtf(S);

		float x1 = (-B + S) / (2 * A);
		float x2 = (-B - S) / (2 * A);
		if (l1.x>l2.x) {
			if (l1.x >= x1&&l2.x <= x1)return true;
			if (l1.x >= x2&&l2.x <= x2)return true;
		} else {
			if (l2.x >= x1&&l1.x <= x1)return true;
			if (l2.x >= x2&&l1.x <= x2)return true;
		}
		float y1 = -(a*x1 + c) / b;
		float y2 = -(a*x2 + c) / b;

		if (l1.y>l2.y) {
			if (l1.y >= y1&&l2.y <= y1)return true;
			if (l1.y >= y2&&l2.y <= y2)return true;
		} else {
			if (l2.y >= y1&&l1.y <= y1)return true;
			if (l2.y >= y2&&l1.y <= y2)return true;
		}
	}

	return false;
}
bool CollisionF::CollisionRectInCircle(const RectF& rect, const PointF& cPos, float r) {
	RectF rr(cPos.x - r, cPos.x + r, cPos.y - r, cPos.y + r);

	float a, b;
	bool ba = false;

	if ((cPos.x - r) >= rect.left && (cPos.x + r) <= rect.right) {
		ba = true;
		if (rr.IsRectIn(rect)) return true;
	} else {
		if ((cPos.y - r) >= rect.top && (cPos.y + r) <= rect.bottom) {
			ba = true;
			if (rr.IsRectIn(rect)) return true;
		}
	}
	if (!ba) {
		r *= r;
		a = rect.left - cPos.x;
		b = rect.top - cPos.y;
		if (a*a + b*b <= r) return true;
		a = rect.right - cPos.x;
		if (a*a + b*b <= r) return true;
		a = rect.left - cPos.x;
		b = rect.bottom - cPos.y;
		if (a*a + b*b <= r) return true;
		a = rect.right - cPos.x;
		if (a*a + b*b <= r) return true;
	}
	return false;
}
bool CollisionF::CollisionHexagonInPoint(const HexagonF& hexagon, const PointF& p) {
	unsigned cc = 0;
	const Array<PointF>& points = hexagon.points;
	float mx = points[0].x, my = points[0].y;
	for (unsigned i = 1; i<points.Size(); ++i) {
		if (points[i].x>mx)mx = points[i].x;
		if (points[i].y>my)my = points[i].y;
	}
	const float ax = mx + 1 - p.x,
		ay = my + 1 - p.y;

	for (int i = 0; i<points.Size(); ++i) {
		float bx, by, abx, aby;
		if (i == points.Size() - 1) {
			bx = points[0].x - points[i].x;
			by = points[0].y - points[i].y;
			abx = p.x - points[i].x;
			aby = p.y - points[i].y;
		} else {
			bx = points[i + 1].x - points[i].x;
			by = points[i + 1].y - points[i].y;
			abx = p.x - points[i].x;
			aby = p.y - points[i].y;
		}

		const float aba = ax*by - bx*ay;

		if (aba == 0) {
			cc++;
			continue;
		}
		float A = (bx*aby - by*abx) / aba;
		float B = (ax*aby - ay*abx) / aba;
		if ((A <= 1) && (B <= 1) & (A >= 0) && (B >= 0))cc++;
	}
	return (cc % 2) == 1;
}
bool CollisionF::CollisionHexagon(const HexagonF& points1, const HexagonF& points2, bool lineOnlyCollision/* = false*/) {
	const Array<PointF>& point1 = points1.points;
	const Array<PointF>& point2 = points2.points;
	if ((!lineOnlyCollision) && (CollisionHexagonInPoint(points1, point2[0]) || CollisionHexagonInPoint(points2, point1[0])))return true;
	for (unsigned i = 0; i< point1.Size() - 1; ++i) {
		for (unsigned j = 0; j<point2.Size() - 1; ++j) {
			if (CollisionLine(point1[i], point1[i + 1], point2[j], point2[j + 1]))return true;
		}
		if (CollisionLine(point1[i], point1[i + 1], point2[point2.Size() - 1], point2[0]))return true;
	}
	for (unsigned j = 0; j<point2.Size() - 1; ++j) {
		if (CollisionLine(point1[point1.Size() - 1], point1[0], point2[j], point2[j + 1]))return true;
	}
	if (CollisionLine(point1[point1.Size() - 1], point1[0], point2[point2.Size() - 1], point2[0]))return true;
	return false;
}