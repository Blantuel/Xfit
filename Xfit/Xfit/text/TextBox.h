#pragma once

#include "Font.h"


class TextBox : public Label{
public:
	Frame* boxFrame;
	bool editable;
	bool Draw(unsigned index,unsigned viewIndex);
};