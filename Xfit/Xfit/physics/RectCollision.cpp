#include "RectCollision.h"

bool RectCollision::HitTest(PointF _point) {return rect.IsPointIn(_point);}