#pragma once

#include "RectHitTest.h"
#include "../object/ScaleHeader.h"

class SizeRectHitTest : public RectHitTest {
public:
	RectF baseRect;

	virtual void Size();
};
