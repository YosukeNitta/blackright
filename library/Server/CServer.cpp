#include "pch.h" 
#include "CServer.h"

using namespace std;
namespace gmlib
{

CServer::CServer()
:m_socket(INVALID_SOCKET)
{
}

CServer::~CServer()
{
	stop();
}

LRESULT CServer::start(WORD port)
{
	//	Ç‹Ç∏ÇÕí‚é~ÅB
	stop();

	SOCKET socket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == socket)
	{
		return IServer::SE_CREATE;
	}
	
	struct sockaddr_in addr;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;

	if (0 != ::bind(socket, (struct sockaddr *)&addr, sizeof(addr)))
	{
		::closesocket(socket);
		return IServer::SE_BIND;
	}

	if (SOCKET_ERROR == ::listen(socket, 5))
	{
		::closesocket(socket);
		return IServer::SE_LISTEN;
	}
	
	unsigned long f = 1;
	::ioctlsocket(socket, FIONBIO, &f);

	m_socket = socket;
	return 0;
}

void CServer::stop()
{
	if (INVALID_SOCKET != m_socket)
	{
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

SOCKET CServer::accept(sockaddr_in *addr)
{
	sockaddr_in clientAddr;
	int len = sizeof(clientAddr);	
	SOCKET socket = ::accept(m_socket, (sockaddr*)&clientAddr, &len);
	if (INVALID_SOCKET == socket)
	{
		return NULL;
	}

	if (NULL != addr)
	{
		*addr = clientAddr;
	}
	return socket;
}

}//namespace gmlib.