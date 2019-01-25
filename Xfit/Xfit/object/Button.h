#pragma once

#include "Object.h"
#include "../math/Rect.h"

class Sampler;

class Button : public Object {
protected:
	void Build(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs, GLuint* _outPosUV, GLuint* _outTexture);
#ifdef OPENGL
	struct {
		GLuint upPosUV, overPosUV, downPosUV, disablePosUV;//..PosUV변수는 0으로 이미지가 Build됐는지를 판별함.
		GLuint upTexture, overTexture, downTexture, disableTexture;
	}openGL;
#elif VULKAN
#endif

	void DrawImage(GLuint _posUV, GLuint _texture);

	virtual void ButtonOver(PointF _mousePos) {}
	virtual void ButtonDown(PointF _mousePos) {}
	virtual void ButtonUp(PointF _mousePos) {}
public:
	Collision* collision;
	Sampler * sampler;
	enum class State {
		UP,
		OVER,
		DOWN,
		DISABLE
	};
	Button();
	virtual ~Button();
	void Disible(bool _on);
	State GetState() const;

	virtual void Draw();
	virtual void Update();
	void UpBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs);
	void UpBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect);

	void OverBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs);
	void OverBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect);

	void DownBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs);
	void DownBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect);

	void DisableBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs);
	void DisableBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect);

	void(*buttonOver)(Button* _target,PointF _mousePos);
	void(*buttonDown)(Button* _target, PointF _mousePos);
	void(*buttonUp)(Button* _target, PointF _mousePos);
private:
	State state;
};