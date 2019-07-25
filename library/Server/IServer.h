
#pragma once
#include <winsock2.h>
#include <windows.h>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

namespace gmlib
{

class IServer
{

public:
	enum
	{
		SE_CREATE = 1,	//	SOCKETの生成に失敗しました。リソースが逼迫している可能性があります。
		SE_BIND,		//	bindに失敗しました。指定したポート番号が既に使われている可能性があります。
		SE_LISTEN,		//	listenに失敗しました。リソースが逼迫している可能性があります。
	};

public:
	virtual ~IServer(){}

	/**
	接続待ち受けを開始します。
	@param	port	待ち受けポート番号
	@return			エラーコード。SE_XXXX
	*/
	virtual LRESULT start(WORD port) = 0;

	/**
	接続待ち受けを停止します。
	*/
	virtual void stop() = 0;

	/**
	接続が確立されたSOCKETを取得します。
	@param	*addr	接続先の情報を格納する構造体へのポインタ。NULLを指定した場合は取得しない。
	@return			接続が確立されたSOCKET。
	*/
	virtual SOCKET accept(sockaddr_in *addr = NULL) = 0;
};


typedef std::tr1::shared_ptr<IServer> SServer;

}//namespace gmlib