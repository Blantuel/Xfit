#pragma once

#ifdef _WIN32
#include "../stdafx.h"

class ResourceManager {
public:
	static unsigned LoadRawData(const Tchar* _resource, const Tchar* _type,const void** _outData) {

	}
	static void LoadText(unsigned _resourceID,Tchar* _outText,int _textNum) {

	}
};
#endif