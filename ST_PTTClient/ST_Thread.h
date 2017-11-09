
#pragma once
#include <string>  
#include <windows.h>  
/* 
    1. 线程基类, 要创建新的线程类, 只需要继承此类并实现相关接口 
    2. 要开启线程并运行只需要调用Start()函数 
    3. 未完善地方: 应该写个虚函数Stop(), 当线程过程在运行时可以设置下运行标志变量让线程 
        退出循环过程, 再作些清理工作, 避免暴力终止线程。 
*/  
class CST_Thread
{
	

public:  
	 CST_Thread(const std::string threadName = "noNamed");  
	virtual ~CST_Thread();  

	virtual void Run() = 0;                 // 线程执行过程  
	virtual bool Start(bool bSuspended/* = false*/);  

	void Join(int timeout = -1);            // 等待超时时间为负时, 表示无限等待  
	void Resume();                          // 恢复挂起的线程  
	void Suspend();                         // 挂起线程  
	bool Terminate(unsigned long exitCode); // 结束线程  

	unsigned int GetThreadID();             // 获取线程ID  
	std::string GetThreadName();  
	void SetThreadName(std::string threadName);  

private:  
	bool CreateThread(bool bSuspended = false);// 开始运行线程  
	static unsigned int WINAPI StaticThreadFunc(void* arg); // 线程函数  

protected:  
	HANDLE          m_handle;  
	std::string     m_threadName;  
	unsigned int    m_threadID;  
	volatile bool   m_bRun;                 // 表明线程是否已成功创建(实际上与m_handle含义相同了) 
};

