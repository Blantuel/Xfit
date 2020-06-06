#include "EditBox.h"
#include "../text/Font.h"

#include "../math/Rect.h"
#include "../system/Input.h"
#include "../_system/_Android.h"

#ifdef __ANDROID__
extern "C"
JNIEXPORT void JNICALL
Java_com_willygames_certainwarrior_XfitActivity_ApplyTextEditBox(JNIEnv *env, jobject thiz, jstring string) {
    const char* str = env->GetStringUTFChars(string, nullptr);

    EditBox::staticText = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);

    env->ReleaseStringUTFChars(string, str);

    EditBox::isFinish = true;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_willygames_certainwarrior_XfitActivity_EnterTextEditBox(JNIEnv *env, jobject thiz,
															  jstring string) {
	const char* str = env->GetStringUTFChars(string, nullptr);

	EditBox::staticText = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);

	env->ReleaseStringUTFChars(string, str);

	EditBox::isEnter = true;
}

#endif

EditBox::EditBox(FontColor* _fontColor, FontContainer* _fontContainer, ScaleImage* _box, ScaleImage* _cursor, PointF _basePos, unsigned _width, unsigned _textPixelSize) :
	focus(false), basePos(_basePos), baseTextWidth(_width), box(_box) {

#ifndef __ANDROID__
	dt = 0;
	cursorPos=0;
	cursor = _cursor;
	prevCharsLen = 0;
#endif

	label = new SizeLabel;
	label->colors = _fontColor;
	label->fonts = _fontContainer;
	label->sizes = new FontSize[1];
	label->sizes[0].pixelSize = _textPixelSize;
	label->sizes[0].len = 0;
	label->baseSizes = new unsigned[1];
	label->baseSizes[0] = _textPixelSize;
	label->sizeLen = 1;
	label->textWidth = (unsigned int)((float)baseTextWidth * WindowRatio());

	textImage = new Image(_basePos * WindowRatio(), PointF((float)baseTextWidth * WindowRatio(), 0),
		0.f, System::defaultBlend, System::pointSampler, label, System::defaultVertex2D, System::defaultUV, System::defaultIndex);

	box->basePos = basePos;

	box->baseScale = PointF((float)(baseTextWidth + 5) / (float)box->frame->GetWidth(), (float)_textPixelSize / (float)box->frame->GetHeight());

	box->Size();

#ifndef __ANDROID__
	cursor->basePos = basePos + PointF(-(float)baseTextWidth / 2.f, 0.f);
	cursor->baseScale = PointF(GetLineWidth(1.f) / (float)cursor->frame->GetWidth() / WindowRatio(), (float)_textPixelSize  / (float)cursor->frame->GetHeight());

	cursor->Size();

	cursor->visible = false;
#endif
}

void EditBox::SetWidth(unsigned _width) {
	baseTextWidth = _width;
	Size();
}
void EditBox::SetTextPx(unsigned _px) {
	label->baseSizes[0] = _px;
	label->sizes[0].pixelSize = _px;
	Size();
}

unsigned EditBox::GetTextPx()const {
	return label->baseSizes[0];
}
unsigned EditBox::GetWidth()const {
	return baseTextWidth;
}

#ifndef __ANDROID__
void EditBox::EnterChar(bool* _changePos, bool* _requireDraw) {
	if (Input::GetEnterCharState() == EnterCharState::Making) {
		unsigned textSize = text.size();
		if (cursorPos > 0 && prevCharsLen > 0)text.erase(cursorPos - 1, prevCharsLen);
		text.insert(cursorPos - prevCharsLen, Input::GetChars());
		if (text.size() - textSize > 0) {
			cursorPos += text.size() - textSize;
			label->scrollTextCount += text.size() - textSize;
			*_changePos = true;
		}

		label->text = text.c_str();

		textWidths.resize(text.size());

		label->charWidths = textWidths.data();
		*_requireDraw = true;
		prevCharsLen = Input::GetPrevCharsLen();
	} else if (Input::GetEnterCharState() == EnterCharState::Finish) {
		if (Input::GetChars()[0] != L'\n' && Input::GetChars()[0] != L'\r') {
			if (Input::GetChars()[0] == L'\b') {
				if (text.size() > 0 && cursorPos > 0) {
					text.erase(cursorPos - 1, 1);
					cursorPos--;
					label->scrollTextCount--;
					*_changePos = true;
				}
			} else {
				unsigned textSize = text.size();
				if (cursorPos > 0 && prevCharsLen > 0)text.erase(cursorPos - 1, prevCharsLen);
				text.insert(cursorPos - prevCharsLen, Input::GetChars());
				if (text.size() - textSize > 0) {
					cursorPos += text.size() - textSize;
					label->scrollTextCount += text.size() - textSize;
					*_changePos = true;
				}
			}
			label->text = text.c_str();

			textWidths.resize(text.size());

			label->charWidths = textWidths.data();
			*_requireDraw = true;
		}
		prevCharsLen = 0;
	}
}
#endif

void EditBox::_Draw(bool* _changePos, bool* _requireDraw) {
	if (*_requireDraw) {
		label->textWidth = baseTextWidth * WindowRatio();
		label->SizePrepareDraw(WindowRatio());
		textImage->SetScaleY(label->GetHeight());

		textImage->SetPos(PixelPerfectPoint(basePos * WindowRatio(), label->GetWidth(), label->GetHeight(), CenterPointPos::Center));

#ifndef __ANDROID__
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
			cursor->SetX(box->basePos.x + (-(float)label->textWidth / 2.f + gainWidth) / WindowRatio());
		}
#endif
	}
}

ScaleImage* EditBox::GetBox()const {
	return box;
}
ScaleImage* EditBox::GetCursor()const {
#ifndef __ANDROID__
	return cursor;
#else
	return nullptr;
#endif
}

EditBox::~EditBox() {
	delete[] label->sizes;
	delete[] label->baseSizes;
	delete label;
	delete textImage;
}

void EditBox::Draw() {
	box->Draw();
	if (label->text && label->text[0] != 0) {
		textImage->Draw();
	}
#ifndef __ANDROID__
	cursor->Draw();
#endif
}

void EditBox::Size() {
#ifdef __ANDROID__
	if (text == L"") {
		text = L"a";
		label->text = text.c_str();

		label->textWidth = baseTextWidth * WindowRatio();
		label->SizePrepareDraw(WindowRatio());

		box->SetTransform(basePos, PointF((baseTextWidth + 5) / (float)box->frame->GetWidth(), label->sizes[0].pixelSize / (float)box->frame->GetHeight()), 0.f);

		textImage->pos = PixelPerfectPoint(basePos * WindowRatio(), label->GetWidth(), label->GetHeight(), CenterPointPos::Center);
		textImage->SetScale(PointF(baseTextWidth * WindowRatio(), label->GetHeight()));

		text = L"";
		label->text = text.c_str();

		label->SizePrepareDraw(WindowRatio());
	} else {
		label->text = text.c_str();
		label->textWidth = baseTextWidth * WindowRatio();

		label->SizePrepareDraw(WindowRatio());

		box->SetTransform(basePos, PointF((baseTextWidth + 5) / (float)box->frame->GetWidth(), label->sizes[0].pixelSize / (float)box->frame->GetHeight()), 0.f);

		textImage->pos = PixelPerfectPoint(basePos * WindowRatio(), label->GetWidth(), label->GetHeight(), CenterPointPos::Center);
		textImage->SetScale(PointF(baseTextWidth * WindowRatio(), label->GetHeight()));
	}
#else
	const float cursorWidth = (GetLineWidth(1.f) * WindowRatio());
	if (cursorWidth > 1.f) cursor->scale.x = cursorWidth;
	else cursor->scale.x = 1.f;

	if (text == L"") {
		cursorPos = 0;
		text = L"a";
		label->text = text.c_str();

		label->textWidth = baseTextWidth * WindowRatio();
		label->SizePrepareDraw(WindowRatio());

		box->SetTransform(basePos, PointF((baseTextWidth + 5) / (float)box->frame->GetWidth(), label->sizes[0].pixelSize / (float)box->frame->GetHeight()), 0.f);
		cursor->SetTransform(PointF(basePos.x - ((float)label->textWidth / 2.f) / WindowRatio(), basePos.y),
							 PointF(GetLineWidth(1.f) / (float)cursor->frame->GetWidth() / WindowRatio(), label->sizes[0].pixelSize / (float)cursor->frame->GetHeight()), 0.f);

		textImage->pos = PixelPerfectPoint(basePos * WindowRatio(), label->GetWidth(), label->GetHeight(), CenterPointPos::Center);
		textImage->SetScale(PointF(baseTextWidth * WindowRatio(), label->GetHeight()));

		text = L"";
		label->text = text.c_str();

		textWidths.resize(0);
		label->charWidths = textWidths.data();

		label->SizePrepareDraw(WindowRatio());
	} else {
		label->text = text.c_str();
		if (text.size() < cursorPos)cursorPos = text.size();
		label->textWidth = baseTextWidth * WindowRatio();

		textWidths.resize(text.size());
		label->charWidths = textWidths.data();

		label->SizePrepareDraw(WindowRatio());

		unsigned i;
		for (i = 0; i < textWidths.size(); i++) {
			if (textWidths[i] != -1)break;
		}
		int gainWidth = 0;
		for (; i < cursorPos; i++) {
			if (textWidths[i] == -1)break;
			gainWidth += textWidths[i];
		}
		box->SetTransform(basePos, PointF((baseTextWidth + 5) / (float)box->frame->GetWidth(), label->sizes[0].pixelSize / (float)box->frame->GetHeight()), 0.f);
		cursor->SetTransform(PointF(basePos.x + (-(float)label->textWidth / 2.f + gainWidth) / WindowRatio(), basePos.y),
							 PointF(GetLineWidth(1.f) / (float)cursor->frame->GetWidth() / WindowRatio(), label->sizes[0].pixelSize / (float)cursor->frame->GetHeight()), 0.f);

		textImage->pos = PixelPerfectPoint(basePos * WindowRatio(), label->GetWidth(), label->GetHeight(), CenterPointPos::Center);
		textImage->SetScale(PointF(baseTextWidth * WindowRatio(), label->GetHeight()));
	}
#endif

}


void EditBox::SetPos(PointF _pos) {
	basePos = _pos;
	textImage->SetPos(PixelPerfectPoint(basePos * WindowRatio(), label->GetWidth(), label->GetHeight(), CenterPointPos::Center));
	box->SetPos(basePos);
#ifndef __ANDROID__
	unsigned i;
	for (i = 0; i < textWidths.size(); i++) {
		if (textWidths[i] != -1)break;
	}
	int gainWidth = 0;
	for (; i < cursorPos; i++) {
		if (textWidths[i] == -1)break;
		gainWidth += textWidths[i];
	}
	cursor->SetPos(PointF(basePos.x - (float)baseTextWidth / 2.f + gainWidth / WindowRatio(), basePos.y));
#endif
}

void EditBox::SetX(float _x) {
	SetPos(PointF(_x, basePos.y));
}
void EditBox::SetY(float _y) {
	SetPos(PointF(basePos.x, _y));
}
PointF EditBox::GetPos()const {
	return basePos;
}
float EditBox::GetX()const {
	return basePos.x;
}
float EditBox::GetY()const {
	return basePos.y;
}

bool EditBox::Update() {
	bool requireDraw = false;
	bool changePos = false;

#ifdef __ANDROID__
	if(thisEditBox == this) {
		if(isFinish) {
			isFinish = false;
			thisEditBox = nullptr;
			text = staticText;
			staticText = L"";
			Size();
			focus = false;
			Input::IMEFinish();
		} else if(isEnter) {
			isEnter = false;
			text = staticText;
			staticText = L"";
			Size();
		}
	}

	if (Input::IsLMouseClick()) {
		PointF mousePos = Input::GetMousePos();
		if (RectF(box->pos.x - box->scale.x / 2.f, box->pos.x + box->scale.x / 2.f, box->pos.y + box->scale.y / 2.f, box->pos.y - box->scale.y / 2.f).IsPointIn(mousePos)) {
			focus = true;
			thisEditBox = this;
			Input::ShowKeyboard(true, text);
			Input::IMEStart();
		} else if(focus) {
			focus = false;
			thisEditBox = nullptr;
			Input::ShowKeyboard(false, L"");
			Input::IMEFinish();
		}
	} else if (!System::IsActivated() && focus) {
		focus = false;
		thisEditBox = nullptr;
		Input::ShowKeyboard(false, L"");
		Input::IMEFinish();
	}
	_Draw(&changePos, &requireDraw);
	return true;
#else
	if (focus) {
		while (dt >= 0.5f) {
			cursor->visible = !cursor->visible;
			dt -= 0.5f;
		}
		EnterChar(&changePos, &requireDraw);

		if (Input::IsKeyDown(Input::Key::Left) && cursorPos > 0) {
			unsigned i;
			for (i = 0; i < textWidths.size(); i++) {
				if (textWidths[i] != -1)break;
			}
			if (cursorPos <= i) {
				if (label->scrollTextCount > 0)label->scrollTextCount--;
			}
			cursorPos--;
			requireDraw = true;
			changePos = true;
			cursor->visible = true;
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
				label->scrollTextCount++;
			}
			if (cursorPos < text.size()) {
				cursorPos++;
				changePos = true;
				cursor->visible = true;
				dt = 0.f;
			}
			requireDraw = true;
		}
#ifdef _WIN32
		if (Input::IsKeyPressing(Input::Key::Control) && Input::IsKeyPress(Input::Key::V)) {
			TCHAR* data = System::GetClipboardData();
			if (data) {
				auto len = lstrlen(data);

				text.insert(cursorPos, data);
				cursorPos += len;
				requireDraw = true;
				changePos = true;

				System::ClipboardClose();

				label->scrollTextCount += len;

				textWidths.resize(text.size());

				label->charWidths = textWidths.data();

				label->text = text.c_str();

				prevCharsLen = 0;
			}
		}
		dt += System::GetDeltaTime();
#endif

	}

	if (Input::IsLMouseClick()) {
		PointF mousePos = Input::GetMousePos();
		if (RectF(box->pos.x - box->scale.x / 2.f, box->pos.x + box->scale.x / 2.f, box->pos.y + box->scale.y / 2.f, box->pos.y - box->scale.y / 2.f).IsPointIn(mousePos)) {
			focus = true;
			Input::IMEStart();
			cursor->visible = true;
			dt = 0.f;

			size_t i;
			for (i = 0; i < textWidths.size(); i++) {
				if (textWidths[i] != -1)break;
			}
			if ((textWidths.size() - i) > 0) {
				mousePos.x -= box->pos.x - (float)label->textWidth / 2.f;
				int gainWidth = textWidths[i];

				if (mousePos.x < (float)textWidths[i] / 2.f) {
					cursor->SetX(box->basePos.x + (-(float)label->textWidth / 2.f) / WindowRatio());
					cursorPos = i;
				} else {
					size_t j = i, size = textWidths.size();
					if (size == 0) {
						if (mousePos.x >= (float)textWidths[i] / 2.f) {
							cursor->SetX(box->basePos.x + (-(float)label->textWidth / 2.f + (float)textWidths[i]) / WindowRatio());
							cursorPos = i + 1;
						}
					} else {
						i++;
						if (i == size) {
							cursor->SetX(box->basePos.x + (-(float)label->textWidth / 2.f + (float)gainWidth) / WindowRatio());
							cursorPos = i;
						} else {
							for (; i < size; i++) {
								if (i < (size-1) && textWidths[i + 1] == -1) {
									cursor->SetX(box->basePos.x + (-(float)label->textWidth / 2.f + (float)gainWidth) / WindowRatio());
									cursorPos = i;
									break;
								}
								if ((mousePos.x >= ((float)gainWidth - (float)textWidths[i - 1] / 2.f)) &&
									(mousePos.x < ((float)gainWidth + (float)textWidths[i] / 2.f))) {
									cursor->SetX(box->basePos.x + (-(float)label->textWidth / 2.f + (float)gainWidth) / WindowRatio());
									cursorPos = i;
									break;
								}
								gainWidth += textWidths[i];
							}
							if (i == size) {
								cursor->SetX(box->basePos.x + (-(float)label->textWidth / 2.f + (float)gainWidth) / WindowRatio());
								cursorPos = i;
							}
						}
					}
				}
			}
		} else {
			focus = false;
			Input::IMEFinish();
			cursor->visible = false;
		}
	} else if (!System::IsActivated()) {
		focus = false;
        Input::IMEFinish();
		cursor->visible = false;
	}
	_Draw(&changePos, &requireDraw);
	return true;
#endif
}

void EditBox::SetFocus(bool _focus) {
	focus = _focus;
#ifdef __ANDROID__
	if (focus) {
		thisEditBox = this;
		Input::ShowKeyboard(true, text);
		Input::IMEStart();
	} else {
		Input::IMEFinish();
		thisEditBox = nullptr;
		Input::ShowKeyboard(false, L"");
	}
#else
	if (focus) {
		Input::IMEStart();
		cursor->visible = true;
		dt = 0.f;
	} else {
		Input::IMEFinish();
		cursor->visible = false;
	}
#endif
}
