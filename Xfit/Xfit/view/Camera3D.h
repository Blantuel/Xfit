#pragma once


#include "../math/Point3D.h"

class Camera3D {
	Point3DF eye, at;
	float rotation;
public:
	Matrix matrix;
	
	Camera3D(Point3DF _eye=Point3DF(0,0,-1), Point3DF _at = Point3DF(0, 0, 0),float _rotation=0):eye(_eye),at(_at),rotation(_rotation) { UpdateMatrix(); }
	Camera3D(const Matrix& _matrix) :eye(0,0,-1), at(0,0,0),rotation(0),matrix(_matrix) {}
	void UpdateMatrix() {
		const Point3DF& z = (at - eye).Normalize();
		const Point3DF& x = Point3DF(0,1,0).OuterProduct(z).Normalize();
		const Point3DF& y = z.OuterProduct(x);

		matrix = { x.x,x.y,x.z,-x.InnerProduct(eye),
			y.x,y.y,y.z,-y.InnerProduct(eye),
			z.x,z.y,z.z,-z.InnerProduct(eye),
			0,0,0,1 };
	}
	void SetEyePos(const Point3DF& _eye) { eye = _eye; UpdateMatrix(); }
	void SetAtPos(const Point3DF& _at) { at = _at; UpdateMatrix();}
	Point3DF GetEyePos() const { return eye; }
	Point3DF GetAtPos() const { return at; }
	float GetViewRotation() const { return rotation; }
	void SetViewRotation(float _rotation) { rotation = _rotation;UpdateMatrix(); }

	void SetRotationAndPos(float rotationX, float rotationY, float rotationZ, const Point3DF& pos) {
		matrix = Matrix::GetRotationXYZ(rotationX, rotationY, rotationZ);

		matrix.e[3] = -pos.InnerProduct(Point3DF(matrix.e[0], matrix.e[1], matrix.e[2]));
		matrix.e[7] = -pos.InnerProduct(Point3DF(matrix.e[4], matrix.e[5], matrix.e[6]));
		matrix.e[11] = -pos.InnerProduct(Point3DF(matrix.e[8], matrix.e[9], matrix.e[10]));
	}
};