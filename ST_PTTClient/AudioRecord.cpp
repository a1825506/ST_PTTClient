#include "StdAfx.h"
#include "AudioRecord.h"

#include "CPublic.h"

CAudioRecord::CAudioRecord()
{
	m_hWaveIn = NULL;
	m_bRecord = FALSE;

	memset(&m_waveFormatEx, 0, sizeof(m_waveFormatEx));
	m_waveFormatEx.wFormatTag = WAVE_FORMAT_PCM;	// PCM格式数据
	m_waveFormatEx.nChannels = 1;					// 单通道声音为1,立体声为2
	m_waveFormatEx.nSamplesPerSec = 8000;			// 8k,11.025k,22.05k,44.1k
	m_waveFormatEx.wBitsPerSample = 16;				// 8bit or 16 bit for PCM
	m_waveFormatEx.nAvgBytesPerSec = m_waveFormatEx.nChannels*m_waveFormatEx.nSamplesPerSec*m_waveFormatEx.wBitsPerSample/8/*(byte不是bit)*/;
	m_waveFormatEx.nBlockAlign = m_waveFormatEx.nChannels*m_waveFormatEx.wBitsPerSample/8;	// byte	
	m_waveFormatEx.cbSize = 0;						// 如果不是PCM数据,设置为0

	InitBuffer();
}

CAudioRecord::~CAudioRecord(void)
{
	ReleaseBuffer();
}



void CAudioRecord::InitBuffer()
{
	for (int i = 0; i < MAX_RECORD_BUFFER_NUM; ++i)
	{
		memset(&m_waveHdr[i], 0, sizeof(m_waveHdr[i]));
		m_waveHdr[i].lpData = new char[WAVE_HDR_SIZE];
		m_waveHdr[i].dwBufferLength = WAVE_HDR_SIZE;
	}		
}

void CAudioRecord::ReleaseBuffer()
{
	for (int i = 0; i < MAX_RECORD_BUFFER_NUM; ++i)
	{
		if (m_waveHdr[i].lpData)
		{
			delete []m_waveHdr[i].lpData;
			m_waveHdr[i].lpData = NULL;
		}
	}
}

void CALLBACK CAudioRecord::waveInProc(HWAVEIN hwi,UINT uMsg,DWORD_PTR dwInstance,
	DWORD_PTR dwParam1,DWORD_PTR dwParam2)
{
	CAudioRecord* pInstance = (CAudioRecord*)dwInstance;
	if(NULL == pInstance)
		return ;

	switch (uMsg)
	{
	case WIM_OPEN:
		{

		}
		break;
	case WIM_DATA:
		{
			pInstance->OnReadData(dwParam1, dwParam2);
		}
		break;
	case WIM_CLOSE:
		{

		}
		break;
	}
}

BOOL CAudioRecord::Start()
{
	if(m_bRecord)
		return TRUE;

	MMRESULT mRet = waveInOpen(&m_hWaveIn, WAVE_MAPPER, &m_waveFormatEx, (DWORD_PTR)waveInProc, (DWORD_PTR)this,CALLBACK_FUNCTION);
	if (MMSYSERR_NOERROR != mRet)
		return FALSE;

	for (int i = 0; i < MAX_RECORD_BUFFER_NUM; ++i)
	{
		waveInPrepareHeader(m_hWaveIn, &m_waveHdr[i], sizeof(m_waveHdr[i]));
		waveInAddBuffer(m_hWaveIn, &m_waveHdr[i], sizeof(m_waveHdr[i]));
	}

	if (MMSYSERR_NOERROR != waveInStart(m_hWaveIn))
	{
		waveInClose(m_hWaveIn);
		m_hWaveIn = NULL;
		return FALSE;
	}

	m_bRecord = TRUE;



	return TRUE;
}

void CAudioRecord::Stop()
{
	if(!m_bRecord)
		return ;

	if (m_hWaveIn)
	{
		waveInStop(m_hWaveIn);
		waveInReset(m_hWaveIn);
		waveInClose(m_hWaveIn);
		m_hWaveIn = NULL;
	}

	m_bRecord = FALSE;
}

BOOL CAudioRecord::IsRecord()
{
	return m_bRecord;
}

void CAudioRecord::OnReadData(WPARAM wParam, LPARAM lParam)
{
	LPWAVEHDR lpHdr = (LPWAVEHDR)wParam;
	if (NULL == lpHdr || 0 == lpHdr->dwBytesRecorded)
		return ;

	waveInUnprepareHeader(m_hWaveIn, lpHdr, sizeof(WAVEHDR));
   

	CPublic::getCPTTOpus(lpHdr->lpData,lpHdr->dwBytesRecorded);



	if(m_bRecord)
	{
		// 恢复buffer到准备填充状态
		::waveInPrepareHeader(m_hWaveIn, lpHdr, sizeof(WAVEHDR));
		::waveInAddBuffer(m_hWaveIn, lpHdr, sizeof(WAVEHDR));
	}

	TRACE("OnReadData %d\n", lpHdr->dwBytesRecorded);
}
