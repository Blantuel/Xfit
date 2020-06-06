#include "AnimateMultiInstance.h"
#include "../data/Array.h"
#include "../resource/FrameArray.h"


AnimateMultiInstance::AnimateMultiInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Sampler* _sampler, FrameArray* _frame, Vertex* _vertex, Vertex* _uv, Index* _index)
    :ImageMultiInstance(_pos, _scale, _rotation, _blend, _sampler, _frame, _vertex, _uv, _index) {
}

void AnimateMultiInstance::Play(unsigned _index, bool _loop/* = false*/) {
	animateNodes[_index].playing = true;
	animateNodes[_index].loop = _loop;
}
void AnimateMultiInstance::Stop(unsigned _index) {
	animateNodes[_index].playing = false;
}
bool AnimateMultiInstance::IsPlay(unsigned _index)const {
	return animateNodes[_index].playing;
}
void AnimateMultiInstance::GoToAndStop(unsigned _index, unsigned _frame/* = 0*/) {
	animateNodes[_index].playing = false;
	nodes[_index].imgIndex = _frame;
}
void AnimateMultiInstance::GoToAndPlay(unsigned _index, unsigned _frame/* = 0*/, bool _loop/* = false*/) {
	animateNodes[_index].playing = true;
	animateNodes[_index].loop = _loop;
	nodes[_index].imgIndex = _frame;
}
unsigned AnimateMultiInstance::NextFrame(unsigned _index) {
	nodes[_index].imgIndex++;
	return nodes[_index].imgIndex;
}
unsigned AnimateMultiInstance::PrevFrame(unsigned _index) {
	nodes[_index].imgIndex--;
	return nodes[_index].imgIndex;
}
unsigned AnimateMultiInstance::GetCurrectFrame(unsigned _index)const {
	return nodes[_index].imgIndex;
}
unsigned AnimateMultiInstance::GetTotalFrame()const {
	return frame->GetFrameLen();
}

void AnimateMultiInstance::Update() {
	const unsigned size = (unsigned)nodes.Size();
	const unsigned totalFrame = GetTotalFrame();

	for (unsigned i = 0; i < size; i++) {
		if (animateNodes[i].playing) {
			if (nodes[i].imgIndex >= (totalFrame - 1)) {
				if (animateNodes[i].loop) nodes[i].imgIndex = 0;
				else animateNodes[i].playing = false;
			} else {
				nodes[i].imgIndex++;
			}
		}
	}
}