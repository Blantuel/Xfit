#include "IP.h"

void IP::WriteIPInt(int _ip) {
#ifdef _WIN32
    ip.s_addr = (ULONG)_ip;
#elif __ANDROID__
    ip.s_addr = (in_addr_t)_ip;
#endif
}

bool IP::ReadIP(char* _ipText) {
    if (inet_pton(AF_INET, _ipText, &ip.s_addr) != 1) {
        return false;
    }
    return true;
}
void IP::GetIPText(char* _outIP, unsigned _len) {
    inet_ntop(AF_INET, &ip.s_addr, _outIP, _len);
}