#include "MatrixObject.h"

MatrixObject::MatrixObject(PointF _pos,PointF _scale,float _rotation,Blend* _blend):pos(_pos),scale(_scale),rotation(_rotation),Object(_blend) {
	UpdateMatrix();
}
MatrixObject::MatrixObject():pos(0.f,0.f),scale(1.f,1.f),rotation(0.f) {
	mat.Identity();
}
void MatrixObject::UpdateMatrix() {
	mat = Matrix::GetMatrix2D(pos.x,pos.y,scale.x,scale.y,rotation * Math::DIVPI_180F);
}

void MatrixObject::SetPos(PointF _pos) { pos = _pos; UpdateMatrix(); }
void MatrixObject::SetX(float _x) { pos.x = _x; UpdateMatrix(); }
void MatrixObject::SetY(float _y) { pos.y = _y; UpdateMatrix(); }
void MatrixObject::SetScale(PointF _scale) { scale = _scale; UpdateMatrix(); }
void MatrixObject::SetScaleX(float _scaleX) { scale.x = _scaleX; UpdateMatrix(); }
void MatrixObject::SetScaleY(float _scaleY) { scale.y = _scaleY; UpdateMatrix(); }
void MatrixObject::SetRotation(float _rotation) { rotation = _rotation; UpdateMatrix(); }
void MatrixObject::SetTransform(PointF _pos, PointF _scale, float _rotation) { pos = _pos; scale = _scale; rotation = _rotation; UpdateMatrix(); }