#include "Hexagon.h"

const PointF HexagonF::GetCenterPoint()const {
	float area = 0.f, factor;
	PointF p(0.f, 0.f);

	const unsigned len = points.Size();
	for (unsigned i = 0; i < len; ++i) {
		unsigned j = (i + 1) % len;
		factor = ((points[i].x * points[j].y) - (points[j].x  * points[i].y));
		area += factor;
		p.x += (points[i].x + points[j].x)*factor;
		p.y += (points[i].y + points[j].y)*factor;
	}
	area /= 2.f;
	area *= 6.f;
	factor = 1.f / area;
	p *= factor;
	return p;
}
float HexagonF::GetArea()const {
	float area = 0;

	const unsigned len = points.Size();
	for (unsigned i = 0; i < len; ++i) {
		unsigned j = (i + 1) % len;
		area += ((points[i].x * points[j].y) - (points[j].x  * points[i].y));
	}

	return area / 2.f;
}