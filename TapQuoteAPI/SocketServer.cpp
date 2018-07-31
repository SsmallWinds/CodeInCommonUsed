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
	//��ʼ��Socket  
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



	//��ʼ��  
	memset(&servaddr, 0, sizeof(servaddr));
#ifdef WIN
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(m_port);
	servaddr.sin_addr.s_addr = INADDR_ANY;
#else
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(m_port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP��ַ���ó�INADDR_ANY,��ϵͳ�Զ���ȡ������IP��ַ��  
#endif // WIN

												 //�����ص�ַ�󶨵����������׽�����  
	if (::bind(socket_fd, (RJ_SOCKET_ADDR)&servaddr, sizeof(servaddr)) == RJ_SOCKET_ERROR)
	{
		perror("bind error:");
	}
	//��ʼ�����Ƿ��пͻ�������  
	if (listen(socket_fd, 10) == -1) {
		perror("listen error:");
	}
	printf("======waiting for client's request======\n");

	while (true)
	{
		printf("�ȴ�����...\n");
		connect_fd = ::accept(socket_fd, NULL,NULL);
		if (connect_fd == RJ_SOCKET_ERROR)
		{
			printf("accept error !");
			continue;
		}
		printf("���ܵ�һ������ \r\n");

		//��������  
		int ret = ::recv(connect_fd, buff, 255, 0);
		if (ret > 0)
		{
			buff[ret] = 0x00;
			if (strcmp(buff, "authCode") == 0)
			{
				printf("��֤�ɹ���%s \r\n", buff);
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
