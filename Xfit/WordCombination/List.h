#pragma once

#include <object/Shape.h>
#include <object/ShapeInstance.h>
#include <resource/ShapeVertex.h>
#include "SizeLabel.h"
#include <object/Image.h>
#include "main.h"

class DefaultNode {
public:
	bool isDraw;
	SizeLabel* label;
	DefaultNode() {}
	DefaultNode(SizeLabel* _label):label(_label),isDraw(false) {}
};

template <typename NodeType = DefaultNode> class List {
	bool scrolling;

	unsigned selectIndex;
	unsigned selectIndex2;
	unsigned scrollVisibleTime;
	unsigned scrollIndex;

	float nodeHeight;
	float scrollBarHeight;
	float scrollBarY;
	
	float scrollMoveY;
	float scrollMouseY;

	PointF baseScale;
	PointF basePos;
	PointF size;

	Image nodeImage;
	Shape rectShape;
	Shape scrollBar;

	ShapeInstance nodeShape;

	void Make(unsigned _start, unsigned _num) {
		if (_num > 0) {
			ShapeInstanceNode node = { Matrix::GetScale(size.width - 20.f ,nodeHeight),Point3DwF(0.f,0.f,0.f,1.f),Point3DwF(0.7f,0.7f,0.7f,1.f),GetLineWidth() };
			ShapeInstanceNode selectedNode = node;
			selectedNode.fillColor = Point3DwF(0.f, 0.7f, 0.f, 1.f);
			_num += _start;
			for (unsigned i = _start; i < _num; i++) {
				if (i == selectIndex) {
					selectedNode.mat.e[7] = -10.f + size.height / 2.f - nodeHeight / 2.f - (float)i * (nodeHeight + 10.f);//y
					if ((selectedNode.mat.e[7] - nodeHeight / 2.f - 10.f) < (-size.height / 2.f))break;
					nodeShape.nodes.InsertLast(selectedNode);
				} else {
					node.mat.e[7] = -10.f + size.height / 2.f - nodeHeight / 2.f - (float)i * (nodeHeight + 10.f);//y
					if ((node.mat.e[7] - nodeHeight / 2.f - 10.f) < (-size.height / 2.f))break;
					nodeShape.nodes.InsertLast(node);
				}
				const unsigned j = i + scrollIndex;
				if (!nodes[j].isDraw) {
					nodes[j].label->SizePrepareDraw(WindowRatio());
					nodes[j].isDraw = true;
				}
			}
		}
	}

	void _Select(unsigned _index) {
		if (selectIndex2 != UINT_MAX) {
			if (selectIndex2 != _index) {
				nodeShape.nodes[selectIndex2].fillColor = Point3DwF(0.7f, 0.7f, 0.7f, 1.f);
				nodeShape.nodes[_index].fillColor = Point3DwF(0.f, 0.7f, 0.f, 1.f);
				if (_index <= selectIndex2) {
					nodeShape.UpdateInstance(_index, selectIndex2 - _index + 1);
				} else {
					nodeShape.UpdateInstance(selectIndex2, _index - selectIndex2 + 1);
				}
			}
		} else {
			nodeShape.nodes[_index].fillColor = Point3DwF(0.f, 0.7f, 0.f, 1.f);
			nodeShape.UpdateInstance(_index, 1);
		}
		selectIndex2 = _index;
	}
	void _Deselect() {
		if (selectIndex2 != UINT_MAX) {
			nodeShape.nodes[selectIndex2].fillColor = Point3DwF(0.7f, 0.7f, 0.7f, 1.f);
			nodeShape.UpdateInstance(selectIndex2, 1);
			selectIndex2 = UINT_MAX;
		}
	}
	void SetScrollBarPos() {
		scrollBar.visible = true;
		scrollVisibleTime = 0;
		float dt = (float)nodeShape.nodes.Size() / (float)nodes.Size();
		if (dt < 0.05f) {
			scrollBarHeight = 0.05f * size.height;
		} else {
			scrollBarHeight = dt * size.height;
		}
		scrollBarY = size.height / 2.f - scrollBarHeight / 2.f - (size.height - scrollBarHeight) * ((float)scrollIndex / (float)(nodes.Size() - nodeShape.nodes.Size()));
		scrollBar.pos = rectShape.pos + PointF(size.width / 2.f, scrollBarY) * baseScale * WindowRatioPoint(posType);
		scrollBar.scale.y = scrollBarHeight * baseScale.y * WindowRatio();
		scrollBar.UpdateMatrix();
	}
	void Scroll(int _scrollDt, bool _setscrollPos) {
		if (_scrollDt == 0)return;
		if (_scrollDt < 0 && scrollIndex < -_scrollDt) {
			if (scrollIndex > 0) {
				if (scrollIndex > nodeShape.nodes.Size()) {
					ReleaseDraw(scrollIndex, nodeShape.nodes.Size());
					PrepareDraw(0, nodeShape.nodes.Size());
				} else {
					ReleaseDraw(nodeShape.nodes.Size(), scrollIndex);
					PrepareDraw(0, scrollIndex);
				}
				scrollIndex = 0;
				if(_setscrollPos)SetScrollBarPos();
			}
			if (selectIndex != UINT_MAX) {
				if (selectIndex < nodeShape.nodes.Size())_Select(selectIndex);
				else _Deselect();
			}

		} else if (_scrollDt > 0 && (scrollIndex + _scrollDt) > (nodes.Size() - nodeShape.nodes.Size())) {
			const unsigned tempSize = nodes.Size() - nodeShape.nodes.Size();
			if (scrollIndex < tempSize) {
				if (tempSize - scrollIndex > nodeShape.nodes.Size()) {
					ReleaseDraw(scrollIndex, nodeShape.nodes.Size());
					PrepareDraw(tempSize, nodeShape.nodes.Size());
				} else {
					ReleaseDraw(scrollIndex, tempSize - scrollIndex);
					PrepareDraw(scrollIndex + nodeShape.nodes.Size(), tempSize - scrollIndex);
				}
				scrollIndex = tempSize;
				if (_setscrollPos)SetScrollBarPos();
			} else scrollIndex = tempSize;

			if (selectIndex != UINT_MAX) {
				if (selectIndex >= scrollIndex)_Select(selectIndex - scrollIndex);
				else _Deselect();
			}
		} else {
			if (_scrollDt > 0) {
				if (_scrollDt > nodeShape.nodes.Size()) {
					ReleaseDraw(scrollIndex, nodeShape.nodes.Size());
					PrepareDraw(scrollIndex + _scrollDt, nodeShape.nodes.Size());
				} else {
					ReleaseDraw(scrollIndex, _scrollDt);
					PrepareDraw(scrollIndex + nodeShape.nodes.Size(), _scrollDt);
				}
			} else if (_scrollDt < 0) {
				if (-_scrollDt > nodeShape.nodes.Size()) {
					PrepareDraw(scrollIndex + _scrollDt, nodeShape.nodes.Size());
					ReleaseDraw(scrollIndex, nodeShape.nodes.Size());
				} else {
					PrepareDraw(scrollIndex + _scrollDt, -_scrollDt);
					ReleaseDraw(scrollIndex + nodeShape.nodes.Size() + _scrollDt, -_scrollDt);
				}
			}
			scrollIndex += _scrollDt;
			if (_setscrollPos)SetScrollBarPos();

			if (selectIndex != UINT_MAX) {
				if (selectIndex >= scrollIndex && selectIndex - scrollIndex < nodeShape.nodes.Size())_Select(selectIndex - scrollIndex);
				else _Deselect();
			}
		}
	}
public:
	Array<NodeType> nodes;
	unsigned blockNum;
	PosType posType;
	
	
	List(PosType _posType, PointF _size, float _nodeHeight, PointF _pos, PointF _scale = PointF(1.f, 1.f)) :nodeShape(_pos* WindowRatioPoint(posType), _scale* WindowRatio(), 0, nullptr, System::rectShapeVertex2D),
		baseScale(_scale), basePos(_pos), posType(_posType), blockNum(0), size(_size), nodeHeight(_nodeHeight), selectIndex(UINT_MAX), scrollIndex(0), selectIndex2(UINT_MAX), scrollBarY(0),scrolling(false),
		nodeImage(PointF(0.f, 0.f), PointF(1.f, 1.f), 0.f, System::defaultBlend, nullptr, nullptr, leftVertex, System::defaultUV, System::defaultIndex), scrollVisibleTime(0), scrollBarHeight(0),
		rectShape(_pos* WindowRatioPoint(posType), _size* _scale* WindowRatio(), 0.f, nullptr, System::rectShapeVertex2D, Point3DwF(0.85f, 0.85f, 0.85f, 1.f), Point3DwF(0.f, 0.f, 0.f, 1.f), GetLineWidth()),
		scrollBar(PointF(0.f, 0.f), PointF(7.f, 1.f)* _scale * WindowRatio(), 0, nullptr, System::rectShapeVertex2D, Point3DwF(0.f, 0.f, 0.f, 1.f), Point3DwF(0.f, 0.f, 0.f, 1.f), 0.f) {
		scrollBar.visible = false;
	}

	void SetPos(PointF _pos) {
		basePos = _pos;
		nodeShape.SetPos(basePos * WindowRatioPoint(posType));

		rectShape.SetPos(nodeShape.pos);

		scrollBar.SetPos(rectShape.pos + PointF(size.width / 2.f, scrollBarY) * baseScale * WindowRatioPoint(posType));
		PrepareDraw(scrollIndex, nodeShape.nodes.Size());
	}
	void SetX(float _x) {

	}
	void SetY(float _y) {

	}
	void SetScale(PointF _scale) {

	}
	void SetScaleX(float _scaleX) {

	}
	void SetScaleY(float _scaleY) {

	}
	void ReleaseDraw(unsigned _start = 0, unsigned _num = 0) {
		if (_num == 0)_num = nodes.Size();
		else _num = _start + _num;
		for (unsigned i = _start; i < _num; i++) {
			if (nodes[i].isDraw) {
				nodes[i].label->Delete();
				nodes[i].isDraw = false;
			}
		}
	}
	void PrepareDraw(unsigned _start=0, unsigned _num=0) {
		if (_num == 0)_num = nodes.Size();
		else _num = _start + _num;
		for (unsigned i = _start; i < _num; i++) {
			if (!nodes[i].isDraw) {
				nodes[i].label->SizePrepareDraw(WindowRatio());
				nodes[i].isDraw = true;
			}
		}
	}
	void Draw() {
		rectShape.Draw();
		nodeShape.Draw();
		const float xx = -size.width / 2.f + 10.f + 10.f;
		unsigned size = nodeShape.nodes.Size();
		for (unsigned a = 0;a< nodeShape.nodes.Size();a++) {
			const unsigned i = a + scrollIndex;
			nodeImage.frame = nodes[i].label;
			nodeImage.pos = PixelPerfectPoint((PointF(xx, nodeShape.nodes[a].mat.e[7])+basePos) * WindowRatioPoint(posType), nodes[i].label->GetWidth(), nodes[i].label->GetHeight(), CenterPointPos::Left);
			nodeImage.scale.x = (float)nodes[i].label->GetWidth();
			nodeImage.scale.y = (float)nodes[i].label->GetHeight();
			nodeImage.UpdateMatrix();
			nodeImage.Draw();
		}
		scrollBar.Draw();
	}
	bool Update() {
		short scrollingDt = -Input::GetWheelScrolling()/WHEEL_DELTA;
		const PointF rectSize = size * baseScale * WindowRatio();
		const RectF rect(-rectSize.width/2.f + rectShape.pos.x, rectSize.width/2.f + rectShape.pos.x, rectSize.height/2.f + rectShape.pos.y, -rectSize.height/2.f + rectShape.pos.y);
		const PointF mousePos = Input::GetMousePos();

		if (rect.IsPointIn(mousePos)) {
			Scroll(scrollingDt, true);
		}
		const PointF sizeT = PointF(size.width - 20.f, nodeHeight) * baseScale * WindowRatio();
		if (Input::IsLMouseClick()) {
			for (unsigned i = 0; i < nodeShape.nodes.Size(); i++) {
				const PointF posT = PointF(0.f, nodeShape.nodes[i].mat.e[7]) * baseScale * WindowRatio() + nodeShape.pos;
				const RectF rect(posT.x - sizeT.width / 2.f, posT.x + sizeT.width / 2.f, posT.y + sizeT.height / 2.f, posT.y - sizeT.height / 2.f);

				if (rect.IsPointIn(mousePos)) {
					Select(i + scrollIndex);
					break;
				}
			}

			if (!scrolling) {
				const RectF rect(scrollBar.pos.x - scrollBar.scale.width / 2.f, scrollBar.pos.x + scrollBar.scale.width / 2.f,
					scrollBar.pos.y + scrollBar.scale.height / 2.f, scrollBar.pos.y - scrollBar.scale.height / 2.f);
				if (rect.IsPointIn(mousePos)) {
					scrolling = true;
					scrollMouseY = mousePos.y;
					scrollMoveY = scrollBar.pos.y;
				}
			}
		} else if ((Input::IsLMouseClicked() || Input::IsMouseOut()) && scrolling) {
			scrolling = false;
			SetScrollBarPos();
		}
		if (scrolling) {
			scrollBar.pos.y = scrollMoveY + mousePos.y - scrollMouseY;

			if (scrollBar.pos.y - scrollBar.scale.height / 2.f < rectShape.pos.y - rectShape.scale.height / 2.f)
				scrollBar.pos.y = rectShape.pos.y - rectShape.scale.height / 2.f + scrollBar.scale.height / 2.f;

			else if (scrollBar.pos.y + scrollBar.scale.height / 2.f > rectShape.pos.y + rectShape.scale.height / 2.f)
				scrollBar.pos.y = rectShape.pos.y + rectShape.scale.height / 2.f - scrollBar.scale.height / 2.f;

			const float dt = ((rectShape.pos.y + rectShape.scale.height / 2.f)-(scrollBar.pos.y + scrollBar.scale.height/2.f))/(rectShape.scale.height- scrollBar.scale.height);
			Scroll((int)(dt * (float)(nodes.Size()-nodeShape.nodes.Size())) - (int)scrollIndex, false);

			scrollBar.UpdateMatrix();
			scrollVisibleTime = 0;
		}
		if (scrollBar.visible) {
			if (scrollVisibleTime >= (unsigned)(1.f / System::GetDeltaTime() * 2.f)) {
				scrollBar.visible = false;
				scrollVisibleTime = 0;
			} else scrollVisibleTime++;
		}
		return true;
	}
	void Size() {
		for (auto& i : nodes) i.isDraw = false;
		nodeShape.scale = baseScale * WindowRatio();
		nodeShape.pos = basePos * WindowRatioPoint(posType);
		nodeShape.UpdateMatrix();
		for (auto& i : nodeShape.nodes) {
			i.lineWidth = GetLineWidth();
		}
		nodeShape.UpdateInstance();

		rectShape.scale = nodeShape.scale * size;
		rectShape.pos = nodeShape.pos;
		rectShape.UpdateMatrix();
		rectShape.lineWidth = GetLineWidth();

		scrollBar.scale = PointF(7.f, scrollBarHeight) * nodeShape.scale;
		scrollBar.pos = rectShape.pos + PointF(size.width / 2.f, scrollBarY) * baseScale * WindowRatioPoint(posType);
		scrollBar.UpdateMatrix();
		PrepareDraw(scrollIndex, nodeShape.nodes.Size());
	}
	void Select(unsigned _index) {
		if (_index >= scrollIndex)_Select(_index - scrollIndex);
		else _Deselect();
		selectIndex = _index;
	}
	void Deselect() {
		_Deselect();
		selectIndex = UINT_MAX;
	}
	unsigned GetSelectIndex()const { return selectIndex; }
	void UpdateNodes() {
		const unsigned beforeSize = nodeShape.nodes.Size();
		if (nodes.Size() > nodeShape.nodes.Size()) {
			Make(nodeShape.nodes.Size(), nodes.Size() - nodeShape.nodes.Size());
			nodeShape.UpdateInstance(beforeSize, nodeShape.nodes.Size() - beforeSize);
		} else {
			nodeShape.nodes.Resize(nodes.Size());
			nodeShape.UpdateInstance(0, 0);
		}
		if (nodes.Size() == nodeShape.nodes.Size()) {//스크롤 불가능 환경이면
			scrollBar.visible = false;
			scrollVisibleTime = 0;
		}
		if (nodes.Size() - 1 < selectIndex) Deselect();
	}

	void BuildNodes(unsigned _maxNum) {
		nodeShape.nodes.Alloc(_maxNum);
		Make(0,nodes.Size());
		nodeShape.BuildInstance();
	}
};

