#pragma once

#include "../stdafx.h"

class IP {
	friend class Socket;
#ifdef _WIN32
	IN_ADDR ip;
#elif __ANDROID__
	in_addr ip;
#endif
public:
	void WriteIPInt(int _ip);
	void GetIPText(char* _outIP, unsigned _len);
	bool ReadIP(char* _ipText);
};