#pragma once


#include "Rect.h"
#include "Hexagon.h"
#include "Lines.h"

class Object;
class Viewport;
class Camera;

class CollisionF {
public:
	static bool CollisionCircle(const PointF& a, const PointF& b, float r1, float r2);
	static bool CollisionCircleInPoint(const PointF& c, float r, const PointF& p);
	static bool CollisionLine(const PointF& a1, const PointF& a2, const PointF& b1, const PointF& b2, PointF* resultPoint = nullptr);
	static bool CollisionRectInLine(const RectF& rect, const PointF& a1, const PointF& a2, bool lineOnlyCollision = true);
	static bool CollisionCircleInLine(const PointF& _l1, const PointF& _l2, const PointF& cPos, float r, bool lineOnlyCollision = true);
	static bool CollisionTriangleInPoint(const PointF& t1, const PointF& t2, const PointF& t3, const PointF& p);
	static bool CollisionRectInCircle(const RectF& rect, const PointF& cPos, float r);
	static bool CollisionHexagonInPoint(const HexagonF& hexagon, const PointF& p);
	static bool CollisionLinesInLine(const LinesF& points, const PointF& a, const PointF& b);
	static bool CollisionLines(const LinesF& points1, const LinesF& points2);
	static bool CollisionHexagonInLine(const HexagonF& points, const PointF& a, const PointF& b, bool lineOnlyCollision = false);
	static bool CollisionHexagon(const HexagonF& points1, const HexagonF& points2, bool lineOnlyCollision = false);
};
