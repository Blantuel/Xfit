#include "AnimateBase.h"

AnimateBase::AnimateBase():cFrame(0),playing(false) {}
void AnimateBase::Play(bool _loop/* = false*/) { playing = true; loop = _loop; }
void AnimateBase::Stop() { playing = false; }
bool AnimateBase::IsPlay()const { return playing; }
void AnimateBase::GoToAndStop(unsigned _frame/* = 0*/) { cFrame = _frame; playing = false; }
void AnimateBase::GoToAndPlay(unsigned _frame/* = 0*/, bool _loop/* = false*/) {
	cFrame = _frame;
	playing = true;
	loop = _loop;
}
unsigned AnimateBase::NextFrame() {
	cFrame++;
	return cFrame;
}
unsigned AnimateBase::PrevFrame() {
	cFrame--;
	return cFrame;
}
unsigned AnimateBase::GetCurrectFrame()const { return cFrame; }
void AnimateBase::Update() {
	if (playing) {
		if (cFrame >= (GetTotalFrame() - 1)) {
			if (loop) cFrame = 0;
			else playing = false;
		} else {
			cFrame++;
		}
	}
}