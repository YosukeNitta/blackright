
#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include "ISocket.h"

namespace gmlib
{

class CSocket : public ISocket
{
	//	コピーコンストラクタは禁止。
	CSocket(CSocket&);

protected:
	SOCKET				m_socket;
	DWORD				m_timeoutReceiving;		//	データの受信が一定時間以上行われなかった場合、通信障害として切断する。その切断までの時間設定。
	DWORD				m_timeoutConnecting;	//	接続の際のタイムアウト時間。

	DWORD				m_lastTime;	//	タイムアウトを測るための最終更新時間。

	int					m_state;	//	現在の状態。getState()で取得できる。
	std::vector<byte>	m_send;		//	送信バッファ。
	std::vector<byte>	m_recv;		//	受信バッファ。

	void				*m_shared;	//	接続用スレッドとの共有オブジェクトを指すポインタ。	

	/**
	コンストラクタ。
	@param	address	接続先アドレス。IPもしくはドメイン名。
	@param	port	接続先ポート番号。
	@param	timeout	接続要求のタイムアウト時間。0ならば無制限に待ちますが、実際は20秒～30秒程度で失敗するはずです。
	*/
	CSocket(const char *address, WORD port, DWORD timeoutConnecting);

	/**
	update(DWORD)内で、flgにUPDATE_RECVが含まれていた場合に呼び出されるメソッド。
	*/
	virtual void updateRecv();

	/**
	update(DWORD)内で、flgにUPDATE_SENDが含まれていた場合に呼び出されるメソッド。
	*/
	virtual void updateSend();
public:

	/**
	コンストラクタ。
	SOCKETの生成と接続は自前で行い、通信管理のみを行ってもらう場合に利用する。
	@param	socket 接続済みSOCKET。
	*/
	CSocket(SOCKET socket);

	/**
	指定されたアドレスに接続要求をかけているCSocketを生成します。
	@param	address	接続先アドレス。IPもしくはドメイン名。
	@param	port	接続先ポート番号。
	@param	timeout	接続要求のタイムアウト時間。0ならば無制限に待ちますが、実際は20秒～30秒程度で失敗するはずです。
	@return			指定先に接続要求をかけているCSocketインスタンスへのポインタ。
	*/
	static CSocket *connect(const char *address, WORD port, DWORD timeoutConnecting = 0);

	virtual ~CSocket();
	virtual void close();
	virtual void send(const void *p, int len);
	virtual std::vector<byte> &recv();
	virtual const std::vector<byte> &recv()const;
	virtual void update(DWORD flg = UPDATE_SEND | UPDATE_RECV);
	virtual int getState()const;
	virtual SOCKET getSocket()const;
	virtual void setTimeoutReceiving(DWORD timeoutReceiving);
	virtual DWORD getTimeoutReceiving()const;
};

}//namespace gmlib