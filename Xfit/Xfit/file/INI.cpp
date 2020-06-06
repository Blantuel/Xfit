#include "INI.h"

#ifdef _WIN32

void INI::Write(const wchar_t* _section, const wchar_t* _key, const wchar_t* _value, const wchar_t* _path) {
	WritePrivateProfileStringW(_section, _key, _value, _path);
}

void INI::Read(const wchar_t* _section, const wchar_t* _key, wchar_t* _out, unsigned _outSize, const wchar_t* _path) {
	GetPrivateProfileStringW(_section, _key, L"\0", _out, _outSize, _path);
}

#endif