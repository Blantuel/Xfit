#pragma once

#include "../system/Error.h"

class IP;


class SocketError : public Error {
public:
	enum class Code {
		MakeSocketFailed,
		ConnectError,
		AcceptError,
		SendError,
		RecvError
	};
protected:
	Code code;
public:
	Code GetCode()const { return code; }
	SocketError(Code _code) :code(_code) {}
};

class Socket {
#ifdef _WIN32
	SOCKET handle;
#elif __ANDROID__
	int handle;
#endif

	Socket() {}
public:
	static bool Init();
	static void Release();

	static Socket* New();

	
	~Socket();

	void Connect(IP* _ip, unsigned _portNumber);
	Socket* Accept(unsigned _portNumber);

	void Send(const char* _buf, int _len)const;
	int Recv(char* _buf, int _len);
};

