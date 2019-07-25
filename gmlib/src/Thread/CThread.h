
#pragma once

#include <windows.h>
#include <process.h>
#include "IThread.h"

namespace gmlib
{

class CThread : public IThread
{
protected:
	HANDLE	m_hThread;
	HANDLE	m_hEvent;
	volatile bool m_stopFlg;

	static unsigned int __stdcall ThreadCallBack(void *p);

	virtual void sleep(DWORD milliseconds);
public:
	CThread();
	virtual ~CThread();

	virtual void start();
	virtual void stop();
	virtual void stopAndWait();
	virtual bool isRunning()const;
	virtual bool isValid()const;
};

}//namespace gmlib