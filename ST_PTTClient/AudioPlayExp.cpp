#include "StdAfx.h"
#include "AudioPlayExp.h"

CAudioPlayExp::CAudioPlayExp(void)
{
	m_hWaveOut = NULL;
	m_bPlay = FALSE;     
	m_bReset = FALSE;

	memset(&m_waveFormatEx, 0, sizeof(m_waveFormatEx));
	m_waveFormatEx.wFormatTag = WAVE_FORMAT_PCM;	// PCM格式数据
	m_waveFormatEx.nChannels = 1;					// 单通道声音为1,立体声为2
	m_waveFormatEx.nSamplesPerSec = 8000;			// 8k,11.025k,22.05k,44.1k
	m_waveFormatEx.wBitsPerSample = 16;				// 8bit or 16 bit for PCM
	m_waveFormatEx.nAvgBytesPerSec = m_waveFormatEx.nChannels*m_waveFormatEx.nSamplesPerSec*m_waveFormatEx.wBitsPerSample/8/*(byte不是bit)*/;
	m_waveFormatEx.nBlockAlign = m_waveFormatEx.nChannels*m_waveFormatEx.wBitsPerSample/8;	// byte	
	m_waveFormatEx.cbSize = 0;	
}

CAudioPlayExp::~CAudioPlayExp(void)
{

}

void CALLBACK CAudioPlayExp::waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance,   
	DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	CAudioPlayExp* pInstance = (CAudioPlayExp*)dwInstance;
	if (NULL == pInstance)
		return;

	switch (uMsg)
	{
	case WOM_OPEN:
		{

		}
		break;
	case WOM_DONE:
		{
			pInstance->OnWriteFinish(dwParam1, dwParam2);
		}
		break;
	case WOM_CLOSE:
		{

		}
		break;
	}
}

BOOL CAudioPlayExp::Start()
{
	if(m_bPlay)
		return TRUE;

	MMRESULT mRet = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_waveFormatEx, 
		(DWORD_PTR)waveOutProc, (DWORD_PTR)this, 
		CALLBACK_FUNCTION);
	if (MMSYSERR_NOERROR != mRet)
		return FALSE;

	DWORD volume = 0xffffffff;
	waveOutSetVolume(m_hWaveOut, volume);

	m_bPlay = TRUE;
	return TRUE;
}

void CAudioPlayExp::Stop()
{
	if(!m_bPlay)
		return;

	if (m_hWaveOut)
	{
		waveOutReset(m_hWaveOut);
		waveOutClose(m_hWaveOut);
		m_hWaveOut = NULL;
	}
	m_bPlay = FALSE;
}

BOOL CAudioPlayExp::Write(char* pData, int nLen)
{
	if(!m_bPlay)
	{
		return FALSE;
	}

	WAVEHDR* pWaveHdr = new WAVEHDR;
	memset(pWaveHdr, 0, sizeof(WAVEHDR));
	pWaveHdr->lpData = pData;
	pWaveHdr->dwBufferLength = nLen;
	waveOutPrepareHeader(m_hWaveOut, pWaveHdr, sizeof(WAVEHDR));
	waveOutWrite(m_hWaveOut, pWaveHdr, sizeof(WAVEHDR));

	return TRUE;
}

void CAudioPlayExp::OnWriteFinish(WPARAM wParam, LPARAM lParam)
{
	WAVEHDR* pWaveHdr = (WAVEHDR*)wParam;
	if (pWaveHdr && m_hWaveOut)
	{
		::waveOutUnprepareHeader(m_hWaveOut, pWaveHdr, sizeof(WAVEHDR));
	}
	delete pWaveHdr;
	pWaveHdr = NULL;
}
