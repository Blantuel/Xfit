#pragma once

#include "ImageMultiInstance.h"

struct AnimateMultiInstanceNode {
	bool playing;
	bool loop;
};

class AnimateMultiInstance : public ImageMultiInstance {
public:
	Array<AnimateMultiInstanceNode>* animateNodes;

	void Play(unsigned _index,bool _loop = false);
	void Stop(unsigned _index);
	bool IsPlay(unsigned _index)const;
	void GoToAndStop(unsigned _index,unsigned _frame = 0);
	void GoToAndPlay(unsigned _index,unsigned _frame = 0, bool _loop = false);
	unsigned NextFrame(unsigned _index);
	unsigned PrevFrame(unsigned _index);
	unsigned GetCurrectFrame(unsigned _index)const;
	unsigned GetTotalFrame()const;

	virtual void Update();
};