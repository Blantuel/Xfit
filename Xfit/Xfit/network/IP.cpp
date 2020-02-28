#include "IP.h"

#ifdef _WIN32
bool IP::ReadIP(char* _ipText) {
	if (inet_pton(AF_INET, _ipText, &ip.s_addr) != 1) {
		return false;
	}
	return true;
}
#elif __ANDROID__
bool IP::ReadIP(char* _ipText) {
    if (inet_pton(AF_INET, _ipText, &ip.s_addr) != 1) {
        return false;
    }
    return true;
}

#endif