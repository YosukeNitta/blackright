
#pragma once
#include <windows.h>
#include <memory>

namespace gmlib
{

/**
スレッドを扱うクラスのインターフェース。
*/
class IThread
{
protected:
	/**
	呼び出し側スレッドを一定時間だけ中断します。
	ただし、stop()及びstopAndWait()が呼ばれた場合は即座に再開します。
	また、stop()及びstopAndWait()が呼ばれた後は、このメソッドは無効化されます。
	@param	milliseconds	中断する時間を指定。0xFFFFFFFFが指定された場合は無期限となります。
	*/
	virtual void sleep(DWORD milliseconds) = 0;

public:
	/**
	デストラクタ。
	スレッドが終了するまで待機します。
	*/
	virtual ~IThread(){}

	/**
	スレッドを起動する。
	以後このスレッドは有効状態となり、isValid()がtrueを返すようになる。
	いかなる場合でも、2度起動することはできません。
	2度目以降のこの関数呼び出しは無視されます。
	*/
	virtual void start() = 0;

	/**
	スレッドが停止するよう命令する。この関数は実際の停止を待たずに処理を終了する。
	以後このスレッドは無効状態となり、isValid()がfalseを返すようになる。
	*/
	virtual void stop() = 0;
	
	/**
	スレッドが停止するよう命令し、実際に停止するまで待つ。
	以後このスレッドは無効状態となり、isValid()がfalseを返すようになる。
	*/
	virtual void stopAndWait() = 0;

	/**
	スレッドが起動している状態かどうかを取得する。
	@return	起動している場合はtrueを返す。
	*/
	virtual bool isRunning()const = 0;

	/**
	スレッドが有効状態かどうかを取得する。
	有効状態なのは、start()が呼ばれてから、stop()が呼ばれるまでの間である。
	@return	有効状態の場合はtrueを返す。
	*/
	virtual bool isValid()const = 0;

	/**
	start()によって起動したスレッドに、実際に処理させる内容です。オーバーライドしてください。
	*/
	virtual void run() = 0;
};

typedef std::tr1::shared_ptr<IThread> SThread;

}//namespace gmlib