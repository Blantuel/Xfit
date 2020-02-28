#include "SizeRectHitTest.h"


void SizeRectHitTest::Size() {
	rect = baseRect;
	const PointF ratioPoint = WindowRatioPoint(posType);
	rect.MoveRatio(ratioPoint.x, ratioPoint.y);

	rect.ExtendRatio(WindowRatio(), WindowRatio());
}