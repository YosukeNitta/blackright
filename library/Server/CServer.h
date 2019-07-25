
#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "IServer.h"

namespace gmlib
{

class CServer : public IServer
{
	SOCKET	m_socket;

	//	コピーコンストラクタは禁止。
	CServer(CServer&);
public:

	CServer();
	virtual ~CServer();

	virtual LRESULT start(WORD port);
	virtual void stop();
	virtual SOCKET accept(sockaddr_in *addr = NULL);


};

}//namespace gmlib