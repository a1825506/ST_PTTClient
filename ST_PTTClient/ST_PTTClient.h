
// ST_PTTClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CST_PTTClientApp:
// 有关此类的实现，请参阅 ST_PTTClient.cpp
//

class CST_PTTClientApp : public CWinApp
{
public:
	CST_PTTClientApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CST_PTTClientApp theApp;