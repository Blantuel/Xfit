#pragma once

#include "../system/Error.h"

class DecoderError : public Error {
public:
	enum class Code {
		NoHeader,
		AlreadyDecoded,
		HeaderLoadFail,
		DecodeFail,
		NullData,
		ZeroSize
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	DecoderError(DecoderError::Code _code) :code(_code) {}
};

class Decoder {
protected:
	const void* inputData;
	bool hasHeader;
	unsigned inputSize;
	unsigned outputSize;
	bool decoded;
public:
	Decoder();

	bool HasHeader()const;

	const void* GetInputData()const;
	unsigned GetInputSize()const;
	unsigned GetOutputSize()const;
	bool IsDecoded()const;
};