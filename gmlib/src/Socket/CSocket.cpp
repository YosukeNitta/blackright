#include "stdafx.h" 
#include "CSocket.h"
#pragma comment(lib, "winmm.lib")

using namespace std;
namespace gmlib
{

/*
接続時の動作イメージ。

通信を確立するためには、名前解決(IPアドレス指定ではなかった場合)と::connect関数による接続が必要です。
しかしこれらは、処理の終了を待つには長すぎる時間ブロックする場合があります。
そこで、接続完了までは別スレッドで行うことにします。

スレッドを使った場合、大きく分けて2つの終了挙動が考えられます。
接続用スレッドが先に終了する場合と、CSocketが先に破棄される場合です。
この際、SOCKETのcloseが問題となります。

	◆接続用スレッドで接続確立→CSocketにSOCKETが格納される→（アプリケーションを終了させるなどで）CSocketの破棄。
	この場合、CSocketのデストラクタでSOCKETがcloseされます。正常な動作です。

	◆CSocketの破棄→接続用スレッドで接続確立
	この場合、CSocketのデストラクタでSOCKETがcloseできません。

よって、接続用スレッドは「CSocketがすでに破棄されていたら、SOCKETは自分がcloseする」機能をつける必要があります。

*/

namespace
{

/**
スレッド間で安全にカウントをするためのクラス。
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
スレッド間で共有したいデータを持つクラス。
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
		//	CSocketが先にclose()を呼び出していた。
		//	SOCKETは必要無くなったので閉じる。
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
	//	イベントの生成に失敗していた場合は、即座に終了。
	if (WSA_INVALID_EVENT == obj->m_hEvent)
	{
		return;
	}

	//	ソケットの生成。
	SOCKET socket;
	socket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == socket)
	{
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	接続先構造体を準備。
	struct sockaddr_in sckAddress;
	::ZeroMemory(&sckAddress, sizeof(sckAddress));
	sckAddress.sin_port = htons(obj->m_port);
	sckAddress.sin_family = AF_INET;
	sckAddress.sin_addr.s_addr = ::inet_addr(obj->m_address.c_str());

	//	数値によるIPアドレスでなかった場合、名前解決をする.
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

	//	呼び出し元のCSocketにて、既にclose()が呼ばれていないかをチェック。
	//	呼び出し元でキャンセルされていた場合、connectする必要がないので終了する。
	if (1 == obj->getCount())
	{
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}


	//	ソケットをイベント型に変更。
	WSANETWORKEVENTS events;
	int ret = ::WSAEventSelect(socket, obj->m_hEvent, FD_CONNECT);
	if (SOCKET_ERROR == ret)
	{
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	接続処理。
	if (SOCKET_ERROR == ::connect(socket, (struct sockaddr *)&sckAddress, sizeof(sckAddress)))
	{
		//	エラー内容がWSAEWOULDBLOCKの時のみ正常動作。
		if(WSAEWOULDBLOCK != ::WSAGetLastError())
		{
			::WSAEventSelect(socket, NULL, 0);
			::shutdown(socket, 2);
			::closesocket(socket);
			return;
		}
	}

	//	イベントが発生するまで待機する。
	//	・接続が接続先から拒否される。
	//	・Winsock側によるタイムアウトが発生した。
	//	・呼び出し元のCSocketでclose()が呼ばれた。
	//	以上の時に待機が解かれる。
	DWORD dw = ::WSAWaitForMultipleEvents(1, &obj->m_hEvent, FALSE, WSA_INFINITE, FALSE);

	//	dwがWSA_WAIT_EVENT_0以外でエラー.
	if (WSA_WAIT_EVENT_0 != dw)
	{
		::WSAEventSelect(socket, NULL, 0);
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	どのイベントが発生したか.
	::ZeroMemory(&events, sizeof(events));
	ret = ::WSAEnumNetworkEvents(socket, obj->m_hEvent, &events);
	if (SOCKET_ERROR == ret)
	{
		::WSAEventSelect(socket, NULL, 0);
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	「FD_CONNECTのイベントが発生し、結果が0」な場合を除きエラー。
	if (!((events.lNetworkEvents & FD_CONNECT) && events.iErrorCode[FD_CONNECT_BIT] == 0))
	{
		::WSAEventSelect(socket, NULL, 0);
		::shutdown(socket, 2);
		::closesocket(socket);
		return;
	}

	//	後始末.
	::WSAEventSelect(socket, NULL, 0);

	//	接続に成功したので、SOCKETを保存して終了。
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
		//	NODELAYモードにする。
		//	一般的にはしないべきだが、ゲームなどでは通信効率よりもレスポンスを優先するべき。
		//	このクラスはゲームで使うのを想定している。
		unsigned long f = TRUE;
		::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (PCSTR)&f, sizeof(f));

		//	非ブロッキングモードにする。
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

		//	スレッド側のイベント待ちを打ち切るようセット。
		::SetEvent(obj->m_hEvent);

		//	解放回数カウントを加算
		if (2 == obj->add())
		{
			//	先に接続用スレッドが終了していたので、objはこちらで解放する
			delete obj;
		}
		m_shared = NULL;
	}

	//	送信バッファをクリアする。
	m_send.clear();

	m_state = STATE_DISCONNECTION;
}
void CSocket::send(const void *p, int len)
{
	if (INVALID_SOCKET == m_socket || len <= 0)
	{
		return;
	}

	//	送信バッファにデータを貯めておく。
	//	実際に送信するのはupdateメソッド内で行う。
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
	//	送信データがあれば、実際に送信する。
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
	//	データを受信する。
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

		//	致命的エラーか、相手から切断された場合はこちらからもclose()する。
		if ((SOCKET_ERROR == readSize && WSAEWOULDBLOCK != WSAGetLastError()) || 0 == readSize)
		{
			close();
		}
		else
		{
			if (0 < ret)
			{
				//	最終受信時間を更新。
				m_lastTime = ::timeGetTime();
			}
			else
			{
				//	受信が行われなかったので、タイムアウトチェックを行う。
				if (0 < m_timeoutReceiving && m_timeoutReceiving <= (::timeGetTime() - m_lastTime))
				{
					//	タイムアウト。
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
		//	現在、接続を確立しようとしています。
		SocketConnectObject *p = (SocketConnectObject*)m_shared;
		if (1 == p->getCount())
		{
			//	解放カウントが加算されているので、つまり接続用スレッドが既に終了している。
			//	SOCKETを受け取った後、共有オブジェクトの後始末。
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
				
				//	NODELAYモードにする。
				//	一般的にはしないべきだが、ゲームなどでは通信効率よりもレスポンスを優先するべき。
				//	このクラスはゲームで使うのを想定している。
				unsigned long f = TRUE;
				::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (PCSTR)&f, sizeof(f));

				//	非ブロッキングモードにする。
				f = 1;
				::ioctlsocket(m_socket, FIONBIO, &f);
			}
		}
		else if ((0 < m_timeoutConnecting) && m_timeoutConnecting <= (::timeGetTime() - m_lastTime))
		{
			//	接続タイムアウト。
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