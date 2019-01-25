#pragma once

#include "../math/Rect.h"

class Camera;
class Camera3D;


class Viewport {
	PointF posRatio,sizeRatio;
	RectF rect;
public:
	Camera* camera;
	Camera3D* camera3D;
	void SetRect(const RectF& _rect);
	Viewport(Camera* _camera = nullptr, Camera3D* _camera3D = nullptr);
	Viewport(const RectF& _rect, Camera* _camera = nullptr, Camera3D* _camera3D = nullptr);
	const RectF& GetRect()const;
	const PointF& GetPosRatio()const;
	const PointF& GetSizeRatio()const;
};


