#include "StdAfx.h"
#include "ST_Thread.h"




CST_Thread::CST_Thread(const std::string threadName)  
	: m_threadName(threadName), m_threadID(0), m_bRun(false)  
{  
}  


CST_Thread::~CST_Thread(void)
{
}

bool CST_Thread::Start(bool bSuspend)      // 创建线程并运行(默认)或挂起  
{  
	m_bRun = CreateThread(bSuspend);  
	return m_bRun;  
} 

bool CST_Thread::CreateThread(bool bSuspend)   // 创建线程并运行(默认)或挂起  
{  
	if(!m_bRun)  
	{  
		if(bSuspend)  
			m_handle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadFunc, this, CREATE_SUSPENDED, &m_threadID);  
		else  
			m_handle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadFunc, this, 0, &m_threadID);  
		m_bRun = (NULL != m_handle);  
	}  
	return m_bRun;  
}  

void CST_Thread::Join(int timeout/* = -1*/)            // 等待超时时间(毫秒)为负时, 表示无限等待  
{  
	if(m_handle && m_bRun)  
	{  
		if(timeout < 0)  
			timeout = INFINITE;  

		::WaitForSingleObject(m_handle, timeout);  
	}  
} 

void CST_Thread::Resume()                              // 恢复挂起的线程  
{  
	if(m_handle && m_bRun)  
		::ResumeThread(m_handle);  
}  

void CST_Thread::Suspend()                             // 挂起线程  
{  
	if(m_handle && m_bRun)  
		::SuspendThread(m_handle);  
}

bool CST_Thread::Terminate(unsigned long exitCode)     // 结束线程  
{  
	if(m_handle && m_bRun)  
	{  
		if(::TerminateThread(m_handle, exitCode))  
		{  
			::CloseHandle(m_handle);  
			m_handle = NULL;  
			m_bRun = false;  
			return true;  
		}  
	}  
	return false;  
}  

unsigned int CST_Thread::GetThreadID()  
{  
	return m_threadID;  
}  

std::string CST_Thread::GetThreadName()  
{  
	return m_threadName;  
}  

void CST_Thread::SetThreadName(std::string threadName)  
{  
	m_threadName = threadName;  
}  

unsigned int CST_Thread::StaticThreadFunc(void* arg)       // 线程函数  
{  
	CST_Thread* pThread = (CST_Thread*)arg;                   // 取得线程类指针  

	pThread->Run();                                      // 执行线程过程函数  

	return 0;  
} 