#include "SizeRectHitTest.h"


void SizeRectHitTest::Size() {
	rect = baseRect;
	rect.MoveRatio(WindowRatio(), WindowRatio());

	rect.ExtendRatio(WindowRatio(), WindowRatio());
}