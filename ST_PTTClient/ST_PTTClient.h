
// ST_PTTClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CST_PTTClientApp:
// �йش����ʵ�֣������ ST_PTTClient.cpp
//

class CST_PTTClientApp : public CWinApp
{
public:
	CST_PTTClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CST_PTTClientApp theApp;