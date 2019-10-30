#pragma once

#include "Encoder.h"
#include "../data/ImageType.h"

class ImageEncoder : public Encoder{
protected:
	unsigned width, height;
	ImageType inputType;
public:
	unsigned GetWidth()const;
	unsigned GetHeight()const;
	ImageType GetInputType()const;
};