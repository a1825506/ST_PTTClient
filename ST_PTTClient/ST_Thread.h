
#pragma once
#include <string>  
#include <windows.h>  
/* 
    1. �̻߳���, Ҫ�����µ��߳���, ֻ��Ҫ�̳д��ಢʵ����ؽӿ� 
    2. Ҫ�����̲߳�����ֻ��Ҫ����Start()���� 
    3. δ���Ƶط�: Ӧ��д���麯��Stop(), ���̹߳���������ʱ�������������б�־�������߳� 
        �˳�ѭ������, ����Щ������, ���Ⱪ����ֹ�̡߳� 
*/  
class CST_Thread
{
	

public:  
	 CST_Thread(const std::string threadName = "noNamed");  
	virtual ~CST_Thread();  

	virtual void Run() = 0;                 // �߳�ִ�й���  
	virtual bool Start(bool bSuspended/* = false*/);  

	void Join(int timeout = -1);            // �ȴ���ʱʱ��Ϊ��ʱ, ��ʾ���޵ȴ�  
	void Resume();                          // �ָ�������߳�  
	void Suspend();                         // �����߳�  
	bool Terminate(unsigned long exitCode); // �����߳�  

	unsigned int GetThreadID();             // ��ȡ�߳�ID  
	std::string GetThreadName();  
	void SetThreadName(std::string threadName);  

private:  
	bool CreateThread(bool bSuspended = false);// ��ʼ�����߳�  
	static unsigned int WINAPI StaticThreadFunc(void* arg); // �̺߳���  

protected:  
	HANDLE          m_handle;  
	std::string     m_threadName;  
	unsigned int    m_threadID;  
	volatile bool   m_bRun;                 // �����߳��Ƿ��ѳɹ�����(ʵ������m_handle������ͬ��) 
};

