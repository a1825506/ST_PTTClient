#pragma once
#include <MMSystem.h>
#pragma  comment(lib, "winmm.lib")

class CAudioPlayExp
{
public:
	CAudioPlayExp(void);
	~CAudioPlayExp(void);

public:
	BOOL Start();
	void Stop();
	BOOL Write(char* pData, int nLen);

protected:
	static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance,   
		DWORD_PTR dwParam1, DWORD_PTR dwParam2);
	void OnWriteFinish(WPARAM wParam, LPARAM lParam);

protected:
	WAVEFORMATEX	m_waveFormatEx;
	HWAVEOUT		m_hWaveOut;
	BOOL			m_bPlay;
	BOOL			m_bReset;
};

