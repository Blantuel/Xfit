#include "SizeMatrixObject.h"

#include "ScaleHeader.h"

SizeMatrixObject::SizeMatrixObject(PointF _pos,PointF _scale,float _rotation,Blend* _blend):
basePos(_pos), baseScale(_scale), MatrixObject(_pos * WindowRatio(), _scale * WindowRatio(),
        _rotation, _blend){
}

void SizeMatrixObject::Size() {
    pos = basePos * WindowRatio();

    scale = baseScale * WindowRatio();
    UpdateMatrix();
}
void SizeMatrixObject::SetPos(PointF _pos) {
    basePos = _pos;
    Size();
}
void SizeMatrixObject::SetX(float _x) {
    basePos.x = _x;
    Size();
}
void SizeMatrixObject::SetY(float _y) {
    basePos.y = _y;
    Size();
}
void SizeMatrixObject::SetScale(PointF _scale) {
    baseScale = _scale;
    Size();
}
void SizeMatrixObject::SetScaleX(float _scaleX) {
    baseScale.x = _scaleX;
    Size();
}
void SizeMatrixObject::SetScaleY(float _scaleY) {
    baseScale.y = _scaleY;
    Size();
}
void SizeMatrixObject::SetRotation(float _rotation) {
    rotation = _rotation;
    Size();
}
void SizeMatrixObject::SetTransform(PointF _pos, PointF _scale, float _rotation) {
    basePos = _pos;
    baseScale = _scale;
    rotation = _rotation;
    Size();
}