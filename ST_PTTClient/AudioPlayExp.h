#pragma once
#include <MMSystem.h>
#include "ST_Thread.h"
#include "ST_AudioData.h"
#include <list>   
using namespace std; 
#pragma  comment(lib, "winmm.lib")
using namespace std;  
//创建一个list容器的实例LISTINT   
typedef list<CST_AudioData*> LISTPLAY;   

class CAudioPlayExp:public CST_Thread
{
public:
	CAudioPlayExp(const std::string threadName = "noNamed");
	~CAudioPlayExp(void);

public:

	bool initAudioTrack();

	bool Start(bool bSuspended/* = false*/);  

	virtual void Run();

	void Stop();

	void  addData(char* pData, int nLen);

protected:
	static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance,   
		DWORD_PTR dwParam1, DWORD_PTR dwParam2);
	void OnWriteFinish(WPARAM wParam, LPARAM lParam);

protected:
	WAVEFORMATEX	m_waveFormatEx;
	HWAVEOUT		m_hWaveOut;
	bool  isPlaying;
	LISTPLAY playList;
	char outname[80];
		FILE *fp_output;
};

