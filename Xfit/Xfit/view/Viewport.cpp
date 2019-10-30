#include "Viewport.h"
#include "Camera3D.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"


Viewport::Viewport(Camera* _camera/* = nullptr*/, Camera3D* _camera3D/* = nullptr*/) :camera(_camera), camera3D(_camera3D) { }
Viewport::Viewport(const RectF& _rect, Camera* _camera /* = nullptr*/, Camera3D* _camera3D /* = nullptr*/) { Viewport(_camera, _camera3D); SetRect(_rect); }

const RectF& Viewport::GetRect()const { return rect; }
const PointF& Viewport::GetPosRatio()const { return posRatio; }
const PointF& Viewport::GetSizeRatio()const { return sizeRatio; }
void Viewport::SetRect(const RectF& _rect) {

}