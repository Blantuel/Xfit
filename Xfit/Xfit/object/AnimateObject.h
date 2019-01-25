#pragma once

#include "Object.h"
#include "../math/Rect.h"

class Sampler;

class AnimateObject : public Object {
	union {
		struct {
			GLuint* posUVs;
			GLuint* textures;
		}openGL;
	};
protected:
	unsigned cFrame, len;
	bool playing;
public:
	Sampler * sampler;

	AnimateObject();
	~AnimateObject();
	void Play();
	void Stop();
	bool IsPlay()const;
	void GoToAndStop(unsigned _frame = 0);
	void GoToAndPlay(unsigned _frame = 0);
	unsigned NextFrame();
	unsigned PrevFrame();
	unsigned GetCurrectFrame()const;
	unsigned GetTotalFrame()const;

	virtual void Update();

	virtual void Draw();

	void Build(unsigned _len,const void*const* _datas, const unsigned* _widths, const unsigned* _heights, const RectF* _rects, const PointF*const* _UVs);
	void Build(unsigned _len,const void*const* _datas, const unsigned* _widths, const unsigned* _heights, const RectF* _rects);
};
