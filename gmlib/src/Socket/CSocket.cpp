#include "stdafx.h" 
#include "CSocket.h"
#pragma comment(lib, "winmm.lib")

using namespace std;
namespace gmlib
{

/*
�ڑ����̓���C���[�W�B

�ʐM���m�����邽�߂ɂ́A���O����(IP�A�h���X�w��ł͂Ȃ������ꍇ)��::connect�֐��ɂ��ڑ����K�v�ł��B
�����������́A�����̏I����҂ɂ͒������鎞�ԃu���b�N����ꍇ������܂��B
�����ŁA�ڑ������܂ł͕ʃX���b�h�ōs�����Ƃɂ��܂��B

�X���b�h���g�����ꍇ�A�傫��������2�̏I���������l�����܂��B
�ڑ��p�X���b�h����ɏI������ꍇ�ƁACSocket����ɔj�������ꍇ�ł��B
���̍ہASOCKET��close�����ƂȂ�܂��B

	���ڑ��p�X���b�h�Őڑ��m����CSocket��SOCKET���i�[����遨�i�A�v���P�[�V�������I��������ȂǂŁjCSocket�̔j���B
	���̏ꍇ�ACSocket�̃f�X�g���N�^��SOCKET��close����܂��B����ȓ���ł��B

	��CSocket�̔j�����ڑ��p�X���b�h�Őڑ��m��
	���̏ꍇ�ACSocket�̃f�X�g���N�^��SOCKET��close�ł��܂���B

����āA�ڑ��p�X���b�h�́uCSocket�����łɔj������Ă�����ASOCKET�͎�����close����v�@�\������K�v������܂��B

*/

namespace
{

/**
�X���b�h�Ԃň��S�ɃJ�E���g�����邽�߂̃N���X�B
*/
class ThreadSafeCounter
{
	CRITICAL_SECTION m_cs;
	int m_cnt;
public:

	ThreadSafeCounter()
		:m_cnt(0)
	{
		::InitializeCriticalSection(&m_cs);
	}
	virtual ~ThreadSafeCounter()
	{
		::DeleteCriticalSection(&m_cs);
	}
	int add()
	{
		::EnterCriticalSection(&m_cs);
		++m_cnt;
		int n = m_cnt;
		::LeaveCriticalSection(&m_cs);
		return n;
	}
	int getCount()
	{
		return m_cnt;
	}
};

/**
�X���b�h�Ԃŋ��L�������f�[�^�����N���X�B
*/
class SocketConnectObject : public ThreadSafeCounter
{
public:
	SocketConnectObject(string address, WORD port)
		:m_address(address)
		,m_port(port)
		,m_socket(INVALID_SOCKET)
		,m_hEvent(::WSACreateEvent())
	{}

	virtual ~SocketConnectObject()
	{
		::WSACloseEvent(m_hEvent);
	}

	SOCKET m_socket;
	string m_address;
	WORD m_port;
	WSAEVENT m_hEvent;
};

extern void ConnectFunc(SocketConnectObject *obj);
void ThreadFunc(void *p)
{
	SocketConnectObject *obj = (SocketConnectObject*)p;
	ConnectFunc(obj);

	if (2 == obj->add())
	{
		//	CSocket�����close()���Ăяo���Ă����B
		//	SOCKET�͕K�v�����Ȃ����̂ŕ���B
		if (INVALID_SOCKET != obj->m_socket)
		{
			::shutdown(obj->m_socket, 2);
			::closesocket(obj->m_socket);
		}
		delete obj;
	}
}

void ConnectFunc(SocketConnectObject *obj)
{
	//	�C�x���g�̐����Ɏ��s���Ă����ꍇ�́A�����ɏI���B
	if (WSA_INVALID_EVENT == obj->m_hEvent)
	{
		return;
	}

	//	�\�P�b�g�̐����B
	SOCKET socket;
	socket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == socket)
	{
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	�ڑ���\���̂������B
	struct sockaddr_in sckAddress;
	::ZeroMemory(&sckAddress, sizeof(sckAddress));
	sckAddress.sin_port = htons(obj->m_port);
	sckAddress.sin_family = AF_INET;
	sckAddress.sin_addr.s_addr = ::inet_addr(obj->m_address.c_str());

	//	���l�ɂ��IP�A�h���X�łȂ������ꍇ�A���O����������.
	if (INADDR_NONE == sckAddress.sin_addr.S_un.S_addr)
	{
		struct addrinfo hints;
		struct addrinfo *res;
		::ZeroMemory(&hints, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_INET;
		if (0 != ::getaddrinfo(obj->m_address.c_str(), NULL, &hints, &res))
		{
			::shutdown(socket, 2);
			::closesocket(socket);
			return;
		}
		sckAddress.sin_addr.S_un = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.S_un;
		::freeaddrinfo(res);
	}

	//	�Ăяo������CSocket�ɂāA����close()���Ă΂�Ă��Ȃ������`�F�b�N�B
	//	�Ăяo�����ŃL�����Z������Ă����ꍇ�Aconnect����K�v���Ȃ��̂ŏI������B
	if (1 == obj->getCount())
	{
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}


	//	�\�P�b�g���C�x���g�^�ɕύX�B
	WSANETWORKEVENTS events;
	int ret = ::WSAEventSelect(socket, obj->m_hEvent, FD_CONNECT);
	if (SOCKET_ERROR == ret)
	{
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	�ڑ������B
	if (SOCKET_ERROR == ::connect(socket, (struct sockaddr *)&sckAddress, sizeof(sckAddress)))
	{
		//	�G���[���e��WSAEWOULDBLOCK�̎��̂ݐ��퓮��B
		if(WSAEWOULDBLOCK != ::WSAGetLastError())
		{
			::WSAEventSelect(socket, NULL, 0);
			::shutdown(socket, 2);
			::closesocket(socket);
			return;
		}
	}

	//	�C�x���g����������܂őҋ@����B
	//	�E�ڑ����ڑ��悩�狑�ۂ����B
	//	�EWinsock���ɂ��^�C���A�E�g�����������B
	//	�E�Ăяo������CSocket��close()���Ă΂ꂽ�B
	//	�ȏ�̎��ɑҋ@���������B
	DWORD dw = ::WSAWaitForMultipleEvents(1, &obj->m_hEvent, FALSE, WSA_INFINITE, FALSE);

	//	dw��WSA_WAIT_EVENT_0�ȊO�ŃG���[.
	if (WSA_WAIT_EVENT_0 != dw)
	{
		::WSAEventSelect(socket, NULL, 0);
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	�ǂ̃C�x���g������������.
	::ZeroMemory(&events, sizeof(events));
	ret = ::WSAEnumNetworkEvents(socket, obj->m_hEvent, &events);
	if (SOCKET_ERROR == ret)
	{
		::WSAEventSelect(socket, NULL, 0);
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	�uFD_CONNECT�̃C�x���g���������A���ʂ�0�v�ȏꍇ�������G���[�B
	if (!((events.lNetworkEvents & FD_CONNECT) && events.iErrorCode[FD_CONNECT_BIT] == 0))
	{
		::WSAEventSelect(socket, NULL, 0);
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	��n��.
	::WSAEventSelect(socket, NULL, 0);

	//	�ڑ��ɐ��������̂ŁASOCKET��ۑ����ďI���B
	obj->m_socket = socket;
	return;
}

}//namespace

CSocket *CSocket::connect(const char *address, WORD port, DWORD timeoutConnecting)
{
	CSocket *s = new CSocket(address, port, timeoutConnecting);
	return s;
}

CSocket::CSocket(const char *address, WORD port, DWORD timeoutConnecting)
:m_socket(INVALID_SOCKET)
,m_state(STATE_TRY_CONNECTING)
,m_timeoutReceiving(0)
,m_timeoutConnecting(timeoutConnecting)
,m_lastTime(::timeGetTime())
,m_shared(NULL)
{
	SocketConnectObject *p = new SocketConnectObject(address, port);
	m_shared = p;
	::_beginthread(ThreadFunc, 0, m_shared);
}

CSocket::CSocket(SOCKET socket)
:m_socket(socket)
,m_state(STATE_CONNECTED)
,m_timeoutReceiving(0)
,m_timeoutConnecting(0)
,m_lastTime(::timeGetTime())
,m_shared(NULL)
{
	if (INVALID_SOCKET == socket)
	{
		m_state = STATE_DISCONNECTION;
	}
	else
	{
		//	NODELAY���[�h�ɂ���B
		//	��ʓI�ɂ͂��Ȃ��ׂ������A�Q�[���Ȃǂł͒ʐM�����������X�|���X��D�悷��ׂ��B
		//	���̃N���X�̓Q�[���Ŏg���̂�z�肵�Ă���B
		unsigned long f = TRUE;
		::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (PCSTR)&f, sizeof(f));

		//	��u���b�L���O���[�h�ɂ���B
		f = 1;
		::ioctlsocket(m_socket, FIONBIO, &f);
	}
}

CSocket::~CSocket()
{
	close();
}

void CSocket::close()
{
	if (INVALID_SOCKET != m_socket)
	{
		::shutdown(m_socket, 2);
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}

	if (NULL != m_shared)
	{
		SocketConnectObject *obj = (SocketConnectObject*)m_shared;

		//	�X���b�h���̃C�x���g�҂���ł��؂�悤�Z�b�g�B
		::SetEvent(obj->m_hEvent);

		//	����񐔃J�E���g�����Z
		if (2 == obj->add())
		{
			//	��ɐڑ��p�X���b�h���I�����Ă����̂ŁAobj�͂�����ŉ������
			delete obj;
		}
		m_shared = NULL;
	}

	//	���M�o�b�t�@���N���A����B
	m_send.clear();

	m_state = STATE_DISCONNECTION;
}
void CSocket::send(const void *p, int len)
{
	if (INVALID_SOCKET == m_socket || len <= 0)
	{
		return;
	}

	//	���M�o�b�t�@�Ƀf�[�^�𒙂߂Ă����B
	//	���ۂɑ��M����̂�update���\�b�h���ōs���B
	DWORD size = m_send.size();
	m_send.resize(size + len);
	::memcpy(&m_send[size], p, len);
}
vector<byte> &CSocket::recv()
{
	return m_recv;
}
const vector<byte> &CSocket::recv()const
{
	return m_recv;
}

void CSocket::updateSend()
{
	//	���M�f�[�^������΁A���ۂɑ��M����B
	if (INVALID_SOCKET != m_socket && !m_send.empty())
	{
		int n = ::send(m_socket, (const char *)&m_send[0], m_send.size(), 0);
		if (0 < n)
		{
			m_send.erase(m_send.begin(), m_send.begin() + n);
		}
	}
}

void CSocket::updateRecv()
{
	//	�f�[�^����M����B
	if (INVALID_SOCKET != m_socket)
	{
		int ret = 0;
		byte tmp[1024];
		int readSize;
		while(0 < (readSize = ::recv(m_socket, (char*)tmp, 1024, 0)))
		{
			ret += readSize;
			DWORD size = m_recv.size();
			m_recv.resize(size + (DWORD)readSize);
			::memcpy(&m_recv[size], tmp, readSize);
		}

		//	�v���I�G���[���A���肩��ؒf���ꂽ�ꍇ�͂����炩���close()����B
		if ((SOCKET_ERROR == readSize && WSAEWOULDBLOCK != WSAGetLastError()) || 0 == readSize)
		{
			close();
		}
		else
		{
			if (0 < ret)
			{
				//	�ŏI��M���Ԃ��X�V�B
				m_lastTime = ::timeGetTime();
			}
			else
			{
				//	��M���s���Ȃ������̂ŁA�^�C���A�E�g�`�F�b�N���s���B
				if (0 < m_timeoutReceiving && m_timeoutReceiving <= (::timeGetTime() - m_lastTime))
				{
					//	�^�C���A�E�g�B
					close();
					m_state = STATE_RECEIVE_TIMED_OUT;
				}
			}
		}
	}
}

void CSocket::update(DWORD flg)
{
	if (NULL != m_shared)
	{
		//	���݁A�ڑ����m�����悤�Ƃ��Ă��܂��B
		SocketConnectObject *p = (SocketConnectObject*)m_shared;
		if (1 == p->getCount())
		{
			//	����J�E���g�����Z����Ă���̂ŁA�܂�ڑ��p�X���b�h�����ɏI�����Ă���B
			//	SOCKET���󂯎������A���L�I�u�W�F�N�g�̌�n���B
			m_socket = p->m_socket;
			delete p;
			p = NULL;
			m_shared = NULL;

			if (INVALID_SOCKET == m_socket)
			{
				m_state = STATE_CONNECT_ERROR;
			}
			else
			{
				m_state = STATE_CONNECTED;
				
				//	NODELAY���[�h�ɂ���B
				//	��ʓI�ɂ͂��Ȃ��ׂ������A�Q�[���Ȃǂł͒ʐM�����������X�|���X��D�悷��ׂ��B
				//	���̃N���X�̓Q�[���Ŏg���̂�z�肵�Ă���B
				unsigned long f = TRUE;
				::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (PCSTR)&f, sizeof(f));

				//	��u���b�L���O���[�h�ɂ���B
				f = 1;
				::ioctlsocket(m_socket, FIONBIO, &f);
			}
		}
		else if ((0 < m_timeoutConnecting) && m_timeoutConnecting <= (::timeGetTime() - m_lastTime))
		{
			//	�ڑ��^�C���A�E�g�B
			close();
			m_state = STATE_CONNECTION_TIMED_OUT;
			return;
		}
	}

	if (UPDATE_SEND & flg)
	{
		updateSend();
	}

	if (UPDATE_RECV & flg)
	{
		updateRecv();
	}
}

int CSocket::getState()const
{
	return m_state;
}
SOCKET CSocket::getSocket()const
{
	return m_socket;
}
void CSocket::setTimeoutReceiving(DWORD timeoutReceiving)
{
	m_timeoutReceiving = timeoutReceiving;
}
DWORD CSocket::getTimeoutReceiving()const
{
	return m_timeoutReceiving;
}

}//namespace gmlib.