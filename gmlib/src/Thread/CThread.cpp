#include "stdafx.h" 
#include "CThread.h"

namespace gmlib
{

CThread::CThread()
:m_hThread(NULL)
,m_hEvent(NULL)
,m_stopFlg(false)
{
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CThread::~CThread()
{
	stopAndWait();
	::CloseHandle(m_hEvent);
	::CloseHandle(m_hThread);
}

void CThread::start()
{
	if (NULL != m_hThread || m_stopFlg)
	{
		return;
	}

	m_hThread = (HANDLE)::_beginthreadex(NULL, 0, ThreadCallBack, (void*)this, 0, NULL);
}

void CThread::stop()
{
	m_stopFlg = true;
	::SetEvent(m_hEvent);
}
void CThread::stopAndWait()
{
	stop();
	if (NULL != m_hThread)
	{
		::WaitForSingleObject(m_hThread, INFINITE);
	}
	return;
}

bool CThread::isRunning()const
{
	return (WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, 0));
}

bool CThread::isValid()const
{
	if (NULL == m_hThread || isRunning())
	{
		return !m_stopFlg;
	}
	return false;
}

void CThread::sleep(DWORD milliseconds)
{
	if (!m_stopFlg)
	{
		::WaitForSingleObject(m_hEvent, milliseconds);
	}
}

unsigned int __stdcall CThread::ThreadCallBack(void *p)
{
	CThread *pThread = (CThread*)p;
	pThread->run();
	return 0;
}

}//namespace gmlib.