#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#ifdef WIN
#pragma comment(lib,"ws2_32.lib")//显示连接套接字库
#endif // WIN

#ifdef WIN
#define CLOSE_SOCKET(socket) closesocket(socket)
#else
#define CLOSE_SOCKET(socket) close(socket)
#endif // WIN

#ifdef WIN
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif // WIN

#ifdef WIN
#define RJ_SOCKET SOCKET 
#define RJ_SOCKET_ADDR LPSOCKADDR
#define RJ_TCP_PROTO IPPROTO_TCP
#define RJ_CLOSE ::closesocket
#else
#define RJ_SOCKET int
#define RJ_SOCKET_ADDR const struct sockaddr *
#define RJ_TCP_PROTO IPPROTO_TCP
#define RJ_CLOSE ::close
#endif // WIN

#define RJ_SOCKET_ERROR -1


#define RJ_SOCKET_OK 0

#define LOCAL_HOST "127.0.0.1"

#define DEFAULT_PORT 8000  
#define MAX_BUFF 4096

class SocketServer
{
public:
	SocketServer(int port);
	~SocketServer();

public:
	int Init();
	int Send(const char *message,int length);

private:
	int m_port;
	std::mutex m_mutex;
	std::thread *m_thread;
	std::vector<RJ_SOCKET> m_clients;
};

