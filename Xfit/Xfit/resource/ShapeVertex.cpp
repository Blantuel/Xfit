#include "ShapeVertex.h"
#include "../math/Hexagon.h"

void ShapeVertex::Build(bool _editable/* = false*/) {
	num = vertices.Size() + 1;
	PointF* vts = new PointF[num];

	HexagonF hexagon;
	hexagon.points.SetData(vertices.GetData(), vertices.Size(), vertices.Size());

	centerPos = hexagon.GetCenterPoint();

	Memory::Copy(vts, num, vertices.GetData(), num - 1);
	vts[num - 1] = vts[0];

	_Build(vts, num, _editable);
	delete[]vts;
}
void ShapeVertex::Edit() {
	PointF* vts = new PointF[num];

	HexagonF hexagon;
	hexagon.points.SetData(vertices.GetData(), vertices.Size(), vertices.Size());

	centerPos = hexagon.GetCenterPoint();

	Memory::Copy(vts, num, vertices.GetData(), num - 1);
	vts[num - 1] = vts[0];

	_Edit(vts, num);
	delete[]vts;
}
PointF ShapeVertex::GetCenterPos()const {
	return centerPos;
}
unsigned ShapeVertex::GetNum()const {
	return num - 1;
}