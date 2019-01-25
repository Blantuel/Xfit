#include "Lines.h"
#include "../data/Array.h"

float LinesF::Distance() const {
	unsigned len = points->Size() - 1;
	float d = 0;
	for (unsigned i = 0; i < len; ++i) {
		d += (*points)[i].Distance((*points)[i + 1]);
	}
	return d;
}