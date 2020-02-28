#include "Socket.h"
#include "IP.h"

#ifdef _WIN32

bool Socket::Init() {
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

void Socket::Release() {
	WSACleanup();
}

Socket* Socket::New() {
	Socket* socket = new Socket;
	socket->handle = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket->handle == INVALID_SOCKET) {
		throw SocketError(SocketError::Code::MakeSocketFailed);
	}
	return socket;
}

Socket::~Socket() {
	if (handle)closesocket(handle);
}

void Socket::Connect(IP* _ip, unsigned _portNumber) {
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = _ip->ip.s_addr;
	addr.sin_port = htons(_portNumber);
	if (connect(handle, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		throw SocketError(SocketError::Code::ConnectError);
	}
}
Socket* Socket::Accept(unsigned _portNumber) {
	SOCKADDR_IN addr;
	SOCKADDR_IN addr2;
	int addrLen2;

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;

	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(_portNumber);

	if (bind(handle, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		throw SocketError(SocketError::Code::AcceptError);
	}

	if (listen(handle, 1) == SOCKET_ERROR) {
		throw SocketError(SocketError::Code::AcceptError);
	}
	addrLen2 = sizeof(addr2);

	Socket* socket = new Socket;
	socket->handle = accept(handle, (SOCKADDR*)&addr2, &addrLen2);
	if (socket->handle == INVALID_SOCKET) {
		delete socket;
		throw SocketError(SocketError::Code::AcceptError);
	}
	return socket;
}

void Socket::Send(const char* _buf, int _len)const {
	if (send(handle, _buf, _len, 0) == SOCKET_ERROR) {
		throw SocketError(SocketError::Code::SendError);
	}
}
int Socket::Recv(char* _buf, int _len) {
	int left = _len;
	while (left > 0) {
		int res = recv(handle, _buf, _len, 0);
		if (res == SOCKET_ERROR || res == 0) {
			throw SocketError(SocketError::Code::RecvError);
		}
		left -= res;
		_buf += res;
	}
	return _len - left;
}

#elif __ANDROID__


bool Socket::Init() {
    return true;
}

void Socket::Release() {
}

Socket::~Socket() {
    if (handle)close(handle);
}
Socket* Socket::New() {
    Socket* socket = new Socket;
    socket->handle = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket->handle == -1) {
        throw SocketError(SocketError::Code::MakeSocketFailed);
    }
    return socket;
}
void Socket::Connect(IP* _ip, unsigned _portNumber) {
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = _ip->ip.s_addr;
    addr.sin_port = htons(_portNumber);
    if (connect(handle, (sockaddr*)&addr, sizeof(addr)) == -1) {
        throw SocketError(SocketError::Code::ConnectError);
    }
}
Socket* Socket::Accept(unsigned _portNumber) {
    sockaddr_in addr;
    sockaddr_in addr2;
    socklen_t addrLen2;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(_portNumber);

    if(bind(handle, (sockaddr*)&addr, sizeof(addr)) == -1) {
        throw SocketError(SocketError::Code::AcceptError);
    }


    if (listen(handle, 1) == -1) {
        throw SocketError(SocketError::Code::AcceptError);
    }
    addrLen2 = sizeof(addr2);

    Socket* socket = new Socket;
    socket->handle = accept(handle, (sockaddr*)&addr2, &addrLen2);
    if (socket->handle == -1) {
        delete socket;
        throw SocketError(SocketError::Code::AcceptError);
    }
    return socket;
}

void Socket::Send(const char* _buf, int _len)const {
    if (send(handle, _buf, _len, 0) == -1) {
        throw SocketError(SocketError::Code::SendError);
    }
}
int Socket::Recv(char* _buf, int _len) {
    int left = _len;
    while (left > 0) {
        int res = recv(handle, _buf, _len, 0);
        if (res == -1 || res == 0) {
            throw SocketError(SocketError::Code::RecvError);
        }
        left -= res;
        _buf += res;
    }
    return _len - left;
}

#endif
