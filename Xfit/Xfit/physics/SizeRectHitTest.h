#pragma once

#include "RectHitTest.h"
#include "../object/ScaleHeader.h"

class SizeRectHitTest : public RectHitTest {
public:
	PosType posType;
	RectF baseRect;

	virtual void Size();
};
