#include "Encoder.h"

Encoder::Encoder():encoded(false) {}
bool Encoder::IsEncoded()const { return encoded; }

const void* Encoder::GetInputData()const { 
	return inputData; 
}

unsigned Encoder::GetInputSize()const {
	return inputSize;
}

unsigned Encoder::GetOutputSize()const {
	return outputSize;
}

void* Encoder::GetOutputData()const {
	return outputData;
}