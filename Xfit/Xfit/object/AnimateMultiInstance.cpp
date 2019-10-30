#include "AnimateMultiInstance.h"
#include "../data/Array.h"
#include "../resource/FrameArray.h"

void AnimateMultiInstance::Play(unsigned _index, bool _loop/* = false*/) {
	(*animateNodes)[_index].playing = true;
	(*animateNodes)[_index].loop = _loop;
}
void AnimateMultiInstance::Stop(unsigned _index) {
	(*animateNodes)[_index].playing = false;
}
bool AnimateMultiInstance::IsPlay(unsigned _index)const {
	return (*animateNodes)[_index].playing;
}
void AnimateMultiInstance::GoToAndStop(unsigned _index, unsigned _frame/* = 0*/) {
	(*animateNodes)[_index].playing = false;
	(*nodes)[_index].frameIndex = _frame;
}
void AnimateMultiInstance::GoToAndPlay(unsigned _index, unsigned _frame/* = 0*/, bool _loop/* = false*/) {
	(*animateNodes)[_index].playing = true;
	(*animateNodes)[_index].loop = _loop;
	(*nodes)[_index].frameIndex = _frame;
}
unsigned AnimateMultiInstance::NextFrame(unsigned _index) {
	(*nodes)[_index].frameIndex++;
	return (*nodes)[_index].frameIndex;
}
unsigned AnimateMultiInstance::PrevFrame(unsigned _index) {
	(*nodes)[_index].frameIndex--;
	return (*nodes)[_index].frameIndex;
}
unsigned AnimateMultiInstance::GetCurrectFrame(unsigned _index)const {
	return (*nodes)[_index].frameIndex;
}
unsigned AnimateMultiInstance::GetTotalFrame()const {
	return frame->GetFrameLen();
}

void AnimateMultiInstance::Update() {
	const unsigned size = (unsigned)nodes->Size();
	const unsigned totalFrame = GetTotalFrame();

	for (unsigned i = 0; i < size; i++) {
		if ((*animateNodes)[i].playing) {
			if ((*nodes)[i].frameIndex >= (totalFrame - 1)) {
				if ((*animateNodes)[i].loop) (*nodes)[i].frameIndex = 0;
				else (*animateNodes)[i].playing = false;
			} else {
				(*nodes)[i].frameIndex++;
			}
		}
	}
}