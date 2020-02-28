#pragma once

#include "MatrixObject.h"
#include "../data/Array.h"
#include "../math/Point3Dw.h"

class Vertex;

#pragma pack(push,1)
struct LineInstanceNode {
	Matrix mat;
	Point3DwF lineColor;
	float lineWidth;
};
#pragma pack(pop)

class LineInstance :public MatrixObject {
#ifdef _WIN32
	ID3D11Buffer* instanceBuffer;
#endif

	unsigned num;
	unsigned maxNum;
public:
	Array<LineInstanceNode> nodes;

	Vertex* vertex;

	void BuildInstance();
	void UpdateInstance(unsigned _start = 0, unsigned _num = UINT_MAX);
	void Delete();

	virtual void Draw();

	unsigned GetNum()const;
	unsigned GetMaxNum()const;
	

	LineInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, Vertex* _vertex);
	LineInstance();
	virtual ~LineInstance();
};

