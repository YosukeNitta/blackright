
#pragma once
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <memory>
#include <ws2tcpip.h>
#include <wspiapi.h>

#pragma comment(lib, "ws2_32.lib")

namespace gmlib
{

/**
ソケットを扱うクラスのインターフェース。
全てのメソッドは、単一のスレッドから呼び出されることが想定されています。
例えば、複数のスレッドから同時にsendが呼ばれた場合の動作は不定です。
複数のスレッドから使いたい場合、きちんとCRITICAL_SECTIONなどで保護してから利用してください。
*/
class ISocket
{
public:
	enum
	{
		STATE_TRY_CONNECTING = 0,	//	接続要求をしている状態。
		STATE_CONNECTED,			//	接続要求が成功し、送受信が可能な状態。
		STATE_CONNECT_ERROR,		//	接続要求が失敗した状態。
		STATE_CONNECTION_TIMED_OUT,	//	接続要求に時間がかかりすぎたため、あきらめた状態。
		STATE_RECEIVE_TIMED_OUT,	//	最後にデータを受信した時間から、指定時間以上経過したので切断した状態。
		STATE_DISCONNECTION,		//	正しく切断された後の状態。
	};

	enum
	{
		UPDATE_SEND		= (1 << 0),	//	sendメソッドによって送信バッファに登録したデータを、実際に送信します。
		UPDATE_RECV		= (1 << 1),	//	実際に受信を行い、recv()で受信データを参照できるようにします。
	};

public:
	/**
	デストラクタ。
	*/
	virtual ~ISocket(){}

	/**
	接続を終了します。受信したデータは破棄されません。
	*/
	virtual void close() = 0;
	
	/**
	接続先に送信するデータを、送信バッファに登録します。
	実際に送信するのはupdate(UPDATE_SEND)が呼ばれた時です。
	@param	p	登録データへのポインタ。
	@param	len	登録データのサイズ。
	*/
	virtual void send(const void *p, int len) = 0;

	/**
	受信したデータを保持している受信バッファへの参照を取得します。
	@return	受信バッファへの参照。
	*/
	virtual std::vector<byte> &recv() = 0;
	
	/**
	受信したデータを保持している受信バッファへの参照を取得します。
	@return	受信バッファへの参照。
	*/
	virtual const std::vector<byte> &recv()const = 0;

	/**
	状態を更新します。
	定期的(例えばFPS60のゲームならば1/60秒ごと)に呼び出してください。
	@param	flg		UPDATE_XXXX定数をORで組み合わせたものを指定します。
	*/
	virtual void update(DWORD flg = UPDATE_SEND | UPDATE_RECV) = 0;

	/**
	現在の状態を取得します。
	@return	現在の状態。STATE_XXXX定数。
	*/
	virtual int getState()const = 0;

	/**
	SOCKETを取得する。
	@return	SOCKET
	*/
	virtual SOCKET getSocket()const = 0;

	/**
	受信に関するタイムアウト時間を設定します。
	@param	timeoutReceiving	指定された時間以上の時間受信が行われなかった場合、回線を切断した上で STATE_RECEIVE_TIMED_OUT な状態になります。
								0を指定した場合はタイムアウトは起こりません。
	*/
	virtual void setTimeoutReceiving(DWORD timeoutReceiving) = 0;

	/**
	受信に関するタイムアウト時間を取得します。
	@return	受信に関するタイムアウト時間。
	*/
	virtual DWORD getTimeoutReceiving()const = 0;
};

typedef std::tr1::shared_ptr<ISocket> SSocket;

}//namespace gmlib