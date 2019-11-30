#include "EditBox.h"
#include <text/Font.h>
#include "main.h"

#include <math/Rect.h>
#include <system/Input.h>

EditBox::EditBox(PointF _basePos, PointF _baseScale, unsigned _width, unsigned _textPixelSize) :textImage(_basePos* WindowRatioPoint(posType), PointF(_width* _baseScale.x* WindowRatio(), 0),
	0.f, System::defaultBlend, nullptr, &label, System::defaultVertex2D, System::defaultUV, System::defaultIndex), prevCharsLen(0),
	box(_basePos* WindowRatioPoint(posType), PointF((_width+5)* _baseScale.x*WindowRatio(), 0), 0.f,nullptr,&shapeVertex), posType(PosType::Center),
	cursor((_basePos + PointF(-(float)_width/2.f, 0.f))* WindowRatioPoint(posType), PointF(1 * _baseScale.x * GetLineWidth(1.f), 0), 0.f, nullptr, &shapeVertex, Point3DwF(0.f,0.f,0.f,1.f),Point3DwF(0.f,0.f,0.f,0.f), 0.f), 
	cursorPos(0), dt(0.f), focus(false), basePos(_basePos),baseScale(_baseScale), baseTextWidth(_width){
	cursor.visible = false;

	label.renders = new FontRender[1];
	label.renders[0].type = FontRenderType::Normal;
	label.renders[0].len = 0;
	label.colors = new FontColor[1];
	label.colors[0].color = 0;
	label.colors[0].len = 0;
	label.fonts = new FontContainer[1];
	label.fonts[0].font = font;
	label.fonts[0].len = 0;
	label.sizes = new FontSize[1];
	label.sizes[0].pixelSize = _textPixelSize;
	label.sizes[0].len = 0;
	label.baseSizes = new unsigned[1];
	label.baseSizes[0] = _textPixelSize;
	label.sizeLen = 1;
	label.textWidth = baseTextWidth * WindowRatio();

	shapeVertex.vertices.Alloc(4);
	shapeVertex.vertices.InsertLast(PointF(-0.5f, 0.5f));
	shapeVertex.vertices.InsertLast(PointF(0.5f, 0.5f));
	shapeVertex.vertices.InsertLast(PointF(0.5f, -0.5f));
	shapeVertex.vertices.InsertLast(PointF(-0.5f, -0.5f));
	shapeVertex.Build();

	text = L"a";
	label.text = text.c_str();
	label.SizePrepareDraw(WindowRatio());

	textImage.SetScaleY(label.GetHeight() * baseScale.y);
	box.SetScaleY(label.GetHeight() * baseScale.y);
	cursor.SetScaleY(label.GetHeight() * baseScale.y);

	text = L"";
	label.text = text.c_str();
	label.SizePrepareDraw(WindowRatio());
}
void EditBox::EnterChar(bool* _changePos, bool* _requireDraw) {
	if (Input::GetEnterCharState() == EnterCharState::Making) {
		unsigned textSize = text.size();
		if (cursorPos > 0 && prevCharsLen > 0)text.erase(cursorPos - 1, prevCharsLen);
		text.insert(cursorPos - prevCharsLen, Input::GetChars());
		if (text.size() - textSize > 0) {
			cursorPos += text.size() - textSize;
			label.scrollTextCount += text.size() - textSize;
			*_changePos = true;
		}

		label.text = text.c_str();

		textWidths.resize(text.size());

		label.charWidths = textWidths.data();
		*_requireDraw = true;
		prevCharsLen = Input::GetPrevCharsLen();
	} else if (Input::GetEnterCharState() == EnterCharState::Finish) {
		if (Input::GetChars()[0] == L'\b') {
			if (text.size() > 0 && cursorPos > 0) {
				text.erase(cursorPos - 1, 1);
				cursorPos--;
				label.scrollTextCount--;
				*_changePos = true;
			}
		} else {
			unsigned textSize = text.size();
			if (cursorPos > 0 && prevCharsLen > 0)text.erase(cursorPos - 1, prevCharsLen);
			text.insert(cursorPos - prevCharsLen, Input::GetChars());
			if (text.size() - textSize > 0) {
				cursorPos += text.size() - textSize;
				label.scrollTextCount += text.size() - textSize;
				*_changePos = true;
			}
		}
		label.text = text.c_str();

		textWidths.resize(text.size());

		label.charWidths = textWidths.data();
		*_requireDraw = true;
		prevCharsLen = 0;
	}
}
void EditBox::_Draw(bool* _changePos, bool* _requireDraw) {
	if (*_requireDraw) {
		label.textWidth = baseTextWidth * WindowRatio();
		label.SizePrepareDraw(WindowRatio());

		textImage.SetPos(PixelPerfectPoint(basePos * WindowRatioPoint(posType), label.GetWidth(), label.GetHeight(), CenterPointPos::Center));

		if (*_changePos) {
			unsigned i;
			for (i = 0; i < textWidths.size(); i++) {
				if (textWidths[i] != -1)break;
			}
			int gainWidth = 0;
			for (; i < cursorPos; i++) {
				if (textWidths[i] == -1)break;
				gainWidth += textWidths[i];
			}
			cursor.SetX(box.pos.x + (-(float)label.textWidth / 2.f + gainWidth));
		}
	}
}
bool EditBox::Update() {
	bool requireDraw = false;
	bool changePos = false;

	if (focus) {
		while (dt >= 0.5f) {
			cursor.visible = !cursor.visible;
			dt -= 0.5f;
		}
		EnterChar(&changePos, &requireDraw);
		
		if (Input::IsKeyDown(Input::Key::Left) && cursorPos > 0) {
			unsigned i;
			for (i = 0; i < textWidths.size(); i++) {
				if (textWidths[i] != -1)break;
			}
			if (cursorPos <= i) {
				if (label.scrollTextCount > 0)label.scrollTextCount--;
			}
			cursorPos--;
			requireDraw = true;
			changePos = true;
			cursor.visible = true;
			dt = 0.f;
		}
		if (Input::IsKeyDown(Input::Key::Right)) {
			unsigned i;
			for (i = 0; i < textWidths.size(); i++) {
				if (textWidths[i] != -1)break;
			}
			for (; i < textWidths.size(); i++) {
				if (textWidths[i] == -1)break;
			}
			if (i <= cursorPos) {
				label.scrollTextCount++;
			}
			if (cursorPos < text.size()) {
				cursorPos++;
				changePos = true;
				cursor.visible = true;
				dt = 0.f;
			}
			requireDraw = true;
		}
		if (Input::IsKeyPressing(Input::Key::Control) && Input::IsKeyPress(Input::Key::V)) {
			TCHAR* data = System::GetClipboardData();
			if (data) {
				auto len = lstrlen(data);

				text.insert(cursorPos, data);
				cursorPos += len;
				requireDraw = true;
				changePos = true;

				System::ClipboardClose();

				label.scrollTextCount += len;

				textWidths.resize(text.size());

				label.charWidths = textWidths.data();

				label.text = text.c_str();

				prevCharsLen = 0;
			}
		}
		dt += System::GetDeltaTime();
	}
	
	if (Input::IsLMouseClick()) {
		PointF mousePos = Input::GetMousePos();
		if (RectF(box.pos.x - box.scale.x / 2.f, box.pos.x + box.scale.x / 2.f, box.pos.y + box.scale.y / 2.f, box.pos.y - box.scale.y / 2.f).IsPointIn(mousePos)) {
			focus = true;
			cursor.visible = true;
			dt = 0.f;

			size_t i;
			for (i = 0; i < textWidths.size(); i++) {
				if (textWidths[i] != -1)break;
			}
			if ((textWidths.size() - i) > 0) {//-1 글자 말고 실제로 글자가 있을 경우
				mousePos.x -= box.pos.x - (float)label.textWidth / 2.f;
				int gainWidth = textWidths[i];

				if (mousePos.x < (float)textWidths[i] / 2.f) {
					cursor.SetX(box.pos.x + (-(float)label.textWidth / 2.f));
					cursorPos = i;
				} else {
					size_t j = i, size = textWidths.size() - 1;
					if (size == 0) {
						if (mousePos.x >= (float)textWidths[i] / 2.f) {
							cursor.SetX(box.pos.x + (-(float)label.textWidth / 2.f + (float)textWidths[i]));
							cursorPos = i + 1;
						}
					} else {
						i++;
						for (; i < size; i++) {
							if (textWidths[i+1] == -1)break;
							if ((mousePos.x >= ((float)gainWidth - (float)textWidths[i-1] / 2.f)) &&
								(mousePos.x < ((float)gainWidth + (float)textWidths[i] / 2.f))) {
								cursor.SetX(box.pos.x + (-(float)label.textWidth / 2.f + (float)gainWidth));
								cursorPos = i;
								break;
							}
							gainWidth += textWidths[i];
						}
						if (mousePos.x >= (float)gainWidth && mousePos.x < (((float)gainWidth + (float)textWidths[i] / 2.f) * WindowRatio())) {
							cursor.SetX(box.pos.x + (-(float)label.textWidth / 2.f + (float)gainWidth));
							cursorPos = i;
						} else if (mousePos.x >= (((float)gainWidth + (float)textWidths[i] / 2.f) * WindowRatio()) && mousePos.x < (float)label.textWidth) {
							cursor.SetX(box.pos.x + (-(float)label.textWidth / 2.f + (float)gainWidth + (float)textWidths[i]));
							cursorPos = i + 1;
						}
					}
				}
			}
		} else {
			focus = false;
			cursor.visible = false;
		}
	} else if (!System::IsActivated()) {
		focus = false;
		cursor.visible = false;
	}
	_Draw(&changePos, &requireDraw);
	return true;
}
void EditBox::Draw() {
	box.Draw();
	textImage.Draw();
	cursor.Draw();
}
void EditBox::Size() {
	textImage.scale.x = baseScale.x * baseTextWidth * WindowRatio();

	box.scale.x = baseScale.x * (baseTextWidth + 5) * WindowRatio();
	box.pos = basePos * WindowRatioPoint(posType);
	box.lineWidth = GetLineWidth(1.f);

	const float cursorWidth = (baseScale.x * GetLineWidth(1.f) * WindowRatio());
	if (cursorWidth > 1.f) cursor.scale.x = cursorWidth;
	else cursor.scale.x = 1.f;
	

	if (text == L"") {
		text = L"a";
		label.text = text.c_str();

		label.textWidth = baseTextWidth * WindowRatio();
		label.SizePrepareDraw(WindowRatio());

		unsigned i;
		for (i = 0; i < textWidths.size(); i++) {
			if (textWidths[i] != -1)break;
		}
		int gainWidth = 0;
		for (; i < cursorPos; i++) {
			if (textWidths[i] == -1)break;
			gainWidth += textWidths[i];
		}
		cursor.pos.x = box.pos.x - (float)label.textWidth / 2.f + gainWidth;
		cursor.pos.y = box.pos.y;
		textImage.pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), label.GetWidth(), label.GetHeight(), CenterPointPos::Center);

		textImage.SetScaleY(label.GetHeight() * baseScale.y);
		box.SetScaleY(label.GetHeight() * baseScale.y);
		cursor.SetScaleY(label.GetHeight() * baseScale.y);

		text = L"";
		label.text = text.c_str();
		label.SizePrepareDraw(WindowRatio());
	} else {
		label.textWidth = baseTextWidth * WindowRatio();
		label.SizePrepareDraw(WindowRatio());

		unsigned i;
		for (i = 0; i < textWidths.size(); i++) {
			if (textWidths[i] != -1)break;
		}
		int gainWidth = 0;
		for (; i < cursorPos; i++) {
			if (textWidths[i] == -1)break;
			gainWidth += textWidths[i];
		}
		cursor.pos.x = box.pos.x - (float)label.textWidth / 2.f + gainWidth;
		cursor.pos.y = box.pos.y;
		textImage.pos = PixelPerfectPoint(basePos * WindowRatioPoint(posType), label.GetWidth(), label.GetHeight(), CenterPointPos::Center);

		textImage.SetScaleY(label.GetHeight() * baseScale.y);
		box.SetScaleY(label.GetHeight() * baseScale.y);
		cursor.SetScaleY(label.GetHeight() * baseScale.y);
	}
}
void EditBox::SetPos(PointF _pos) {
	basePos = _pos;
	textImage.SetPos(PixelPerfectPoint(basePos * WindowRatioPoint(posType), label.GetWidth(), label.GetHeight(), CenterPointPos::Center));
	box.SetPos(basePos * WindowRatioPoint(posType));
	unsigned i;
	for (i = 0; i < textWidths.size(); i++) {
		if (textWidths[i] != -1)break;
	}
	int gainWidth = 0;
	for (; i < cursorPos; i++) {
		if (textWidths[i] == -1)break;
		gainWidth += textWidths[i];
	}
	cursor.SetPos(PointF((basePos.x - (float)baseTextWidth / 2.f + gainWidth), basePos.y) * WindowRatioPoint(posType));
}