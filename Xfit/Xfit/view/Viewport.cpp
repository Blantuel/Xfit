#include "Viewport.h"
#include "Camera3D.h"

#ifdef OPENGL
#include "../_system/_OpenGL.h"
#elif VULKAN
#include "../_system/_Vulkan.h"
#endif


Viewport::Viewport(Camera* _camera/* = nullptr*/, Camera3D* _camera3D/* = nullptr*/) :camera(_camera), camera3D(_camera3D) { }
Viewport::Viewport(const RectF& _rect, Camera* _camera /* = nullptr*/, Camera3D* _camera3D /* = nullptr*/) { Viewport(_camera, _camera3D); SetRect(_rect); }

const RectF& Viewport::GetRect()const { return rect; }
const PointF& Viewport::GetPosRatio()const { return posRatio; }
const PointF& Viewport::GetSizeRatio()const { return sizeRatio; }
void Viewport::SetRect(const RectF& _rect) {

}