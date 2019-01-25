#include "AnimateLoopObject.h"

void AnimateLoopObject::Update() {
	if (playing) {
		if (cFrame >= (len - 1)) {
			cFrame = 0;
		} else {
			cFrame++;
		}
	}
}
