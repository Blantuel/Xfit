#pragma once

#include "../system/Error.h"

class Encoder {
protected:
	const void* inputData;
	unsigned inputSize;
	unsigned outputSize;
	void* outputData;
	bool encoded;
public:
	Encoder();

	bool IsEncoded()const;
	const void* GetInputData()const;
	unsigned GetInputSize()const;
	unsigned GetOutputSize()const;
	void* GetOutputData()const;
};