#pragma once

#include "../system/Error.h"

class AnimateBaseError : public Error {
public:
	enum class Code {
		NullFrames
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	AnimateBaseError(Code _code):code(_code) {}
};

class AnimateBase {
protected:
	unsigned cFrame;
	bool playing,loop;
public:
	virtual void Update();
	AnimateBase();
	void Play(bool _loop=false);
	void Stop();
	bool IsPlay()const;
	void GoToAndStop(unsigned _frame = 0);
	void GoToAndPlay(unsigned _frame = 0, bool _loop = false);
	unsigned NextFrame();
	unsigned PrevFrame();
	unsigned GetCurrectFrame()const;
	virtual unsigned GetTotalFrame()=0;
};