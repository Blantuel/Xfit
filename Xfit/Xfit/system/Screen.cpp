#include "Screen.h"
#include "System.h"

namespace Screen {
	PointF Pos(const PointF& _pos) {
		return PointF(_pos.x/((float)System::GetWindowWidth()/2.f),_pos.y/((float)System::GetWindowHeight()/2.f));
	}
	PointF Scale(const PointF& _scale) {
		return PointF(_scale.x/(float)System::GetWindowWidth(),_scale.y/(float)System::GetWindowHeight());
	}
	float X(float _x) {
		return _x/((float)System::GetWindowWidth()/2.f);
	}
	float Y(float _y) {
		return _y/((float)System::GetWindowHeight()/2.f);
	}
	void VertexPos(PointF* _outVertexPos,const RectF& _rect) {
		_outVertexPos[0] = _rect.GetPoint1();
		_outVertexPos[1] = _rect.GetPoint2();
		_outVertexPos[2] = _rect.GetPoint3();
		_outVertexPos[3] = _rect.GetPoint4();
		_outVertexPos[0].x/=(float)System::GetWindowWidth()/2.f;
		_outVertexPos[1].x/=(float)System::GetWindowWidth()/2.f;
		_outVertexPos[2].x/=(float)System::GetWindowWidth()/2.f;
		_outVertexPos[3].x/=(float)System::GetWindowWidth()/2.f;

		_outVertexPos[0].y/=(float)System::GetWindowHeight()/2.f;
		_outVertexPos[1].y/=(float)System::GetWindowHeight()/2.f;
		_outVertexPos[2].y/=(float)System::GetWindowHeight()/2.f;
		_outVertexPos[3].y/=(float)System::GetWindowHeight()/2.f;
	}
}