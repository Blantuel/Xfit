#pragma once

#include "SizeMatrixObject.h"
#include "../data/Array.h"
#include "../math/Point3Dw.h"

class ShapeVertex;

#pragma pack(push,1)
struct ShapeInstanceNode {
	Matrix mat;
	Point3DwF lineColor;
	Point3DwF fillColor;
	float lineWidth;
};
#pragma pack(pop)

class ShapeInstance :public SizeMatrixObject {
#ifdef _WIN32
	ID3D11Buffer* instanceBuffer;
#elif __ANDROID__
	GLuint ins;//ins??? 0?? ???? BuildInstance???? ???.
#endif
	unsigned num;
	unsigned maxNum;
public:
	PointF basePos;
	PointF baseScale;

	Array<ShapeInstanceNode> nodes;

	ShapeVertex* vertex;

	void BuildInstance();
	void UpdateInstance(unsigned _start = 0, unsigned _num = UINT_MAX);
	void Delete();

	virtual void Draw();

	unsigned GetNum()const;
	unsigned GetMaxNum()const;
	bool IsBuild()const;

	ShapeInstance(PointF _pos, PointF _scale, float _rotation, Blend* _blend, ShapeVertex* _vertex);
	virtual ~ShapeInstance();
};

