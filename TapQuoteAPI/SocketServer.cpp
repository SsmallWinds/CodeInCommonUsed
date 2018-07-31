#include "SocketServer.h"
#include <string.h>

using std::cout;
using std::endl;

SocketServer::SocketServer(int port):m_port(port)
{
	m_thread = new std::thread(&SocketServer::Init, this);
	m_thread->detach();
}


SocketServer::~SocketServer()
{
	if (m_thread)
		delete m_thread;
}



int SocketServer::Init()
{
#ifdef WIN
	WORD version = MAKEWORD(2, 2);
	WSADATA m_wsa;
	if (WSAStartup(version, &m_wsa) != 0)
	{
		cout << "startup error!" << endl;
	}
#endif // WIN


	RJ_SOCKET socket_fd, connect_fd;
	sockaddr_in     servaddr;
	char    buff[MAX_BUFF];
	int     n;
	//初始化Socket  
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, RJ_TCP_PROTO)) == RJ_SOCKET_ERROR) {
		perror("creat error:");
	}

#ifdef WIN
	BOOL bTrue = TRUE;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&bTrue, sizeof(BOOL)) != 0)
	{
		perror("setsockopt [SO_REUSEADDR] error:");
	}
#else
	int flag = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag)) != 0)
	{
		perror("setsockopt [SO_REUSEADDR] error:");
	}
#endif // WIN



	//初始化  
	memset(&servaddr, 0, sizeof(servaddr));
#ifdef WIN
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(m_port);
	servaddr.sin_addr.s_addr = INADDR_ANY;
#else
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(m_port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。  
#endif // WIN

												 //将本地地址绑定到所创建的套接字上  
	if (::bind(socket_fd, (RJ_SOCKET_ADDR)&servaddr, sizeof(servaddr)) == RJ_SOCKET_ERROR)
	{
		perror("bind error:");
	}
	//开始监听是否有客户端连接  
	if (listen(socket_fd, 10) == -1) {
		perror("listen error:");
	}
	printf("======waiting for client's request======\n");

	while (true)
	{
		printf("等待连接...\n");
		connect_fd = ::accept(socket_fd, NULL,NULL);
		if (connect_fd == RJ_SOCKET_ERROR)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接 \r\n");

		//接收数据  
		int ret = ::recv(connect_fd, buff, 255, 0);
		if (ret > 0)
		{
			buff[ret] = 0x00;
			if (strcmp(buff, "authCode") == 0)
			{
				printf("认证成功：%s \r\n", buff);
				m_mutex.lock();
				m_clients.push_back(connect_fd);
				m_mutex.unlock();
			}
		}
	}

	CLOSE_SOCKET(socket_fd);

#ifdef WIN
	WSACleanup();
#endif // WIN
	return 0;
}

int SocketServer::Send(const char * message, int length)
{
	m_mutex.lock();

	char revData[255];
	auto iter = m_clients.begin();

	for (; iter != m_clients.end();)
	{
		int ret = send(*iter, message, length, 0);
		if (ret < 0)
		{
			RJ_CLOSE(*iter);
			iter = m_clients.erase(iter);
			continue;
		}
		iter++;
		//}
	}

	m_mutex.unlock();
	return 0;
}
