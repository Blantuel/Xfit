#pragma once

#ifdef _WIN32
#include "../stdafx.h"

class INI {
public:
	//��� �տ� \\ �ٿ���.
	static void Write(const wchar_t* _section, const wchar_t* _key, const wchar_t* _value, const wchar_t* _path);
	//��� �տ� \\ �ٿ���.
	static void Read(const wchar_t* _section, const wchar_t* _key, wchar_t* _out, unsigned _outSize, const wchar_t* _path);
};

#endif