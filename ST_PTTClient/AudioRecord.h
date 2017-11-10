#pragma once
#include "ST_AudioEncoder.h"
#include <MMSystem.h>

#pragma  comment(lib, "winmm.lib")

class CAudioRecord
{
public:
	CAudioRecord(void);
	~CAudioRecord(void);

public:
	BOOL Start();
	void Stop();
	BOOL IsRecord();

protected:
	void InitBuffer();
	void ReleaseBuffer();

protected:
	    static void CALLBACK waveInProc(HWAVEIN hwi,UINT uMsg,DWORD_PTR dwInstance,
		DWORD_PTR dwParam1,DWORD_PTR dwParam2);
	    void OnReadData(WPARAM wParam, LPARAM lParam);
protected:	
	WAVEFORMATEX  m_waveFormatEx;
	WAVEHDR		  m_waveHdr[MAX_RECORD_BUFFER_NUM];
	HWAVEIN		  m_hWaveIn;
	BOOL		  m_bRecord;
	char outname[80];
	FILE *fp_output;
	CST_AudioEncoder* audioEncoder;
};

