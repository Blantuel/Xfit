#pragma once

#include "../system/Error.h"

class CompressError : public Error {
public:
	enum class Code {
		Compress,
		Uncompress,
		NullInput,
		NullOutput,
		ZeroInputSize,
		ZeroOutputSize,
		NullOutOutputSize
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	CompressError(CompressError::Code _code) :code(_code) {}
};
class Compress {
public:
	//_inputLen + _inputLen / 100 + 12
	static void CompressZip(const void* _input, unsigned _inputSize,void* _output, unsigned* _outOutputSize);
	static void UncompressZip(const void* _input, unsigned _inputSize,void* _output, unsigned _outputSize);
};