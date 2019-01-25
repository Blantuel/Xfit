#pragma once

#include "Button.h"


class Checkbox : public Button {
protected:
#ifdef OPENGL
	struct {
		GLuint checkPosUV;//..PosUV������ 0���� �̹����� Build�ƴ����� �Ǻ���.
		GLuint checkTexture;
	}openGL;
#elif VULKAN
#endif
	bool checked;

	virtual void ButtonUp(PointF _mousePos);
public:
	Checkbox();
	virtual ~Checkbox();
	bool IsChecked() const;
	virtual void SetCheck(bool _on);
	virtual void Draw();

	void CheckBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect, const PointF* _UVs);
	void CheckBuild(const void* _data, unsigned _width, unsigned _height, const RectF& _rect);
};