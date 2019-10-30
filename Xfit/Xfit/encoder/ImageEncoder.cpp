#include "ImageEncoder.h"

unsigned ImageEncoder::GetWidth()const {
	return width;
}
unsigned ImageEncoder::GetHeight()const {
	return height;
}
ImageType ImageEncoder::GetInputType()const {
	return inputType;
}