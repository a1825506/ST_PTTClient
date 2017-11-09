#pragma once
#include "afxwin.h"
#include "ClientSocket.h"
#include "ClientUserInfo.h"
#include "ClientChannelInfo.h"
#include "ClientUdpSocket.h"
#include "CPublic.h"

// PTTClientControl 对话框

class PTTClientControl : public CDialogEx
{
	DECLARE_DYNAMIC(PTTClientControl)

public:
	PTTClientControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PTTClientControl();


// 对话框数据
	enum { IDD = IDD_ST_PTTClient_Login };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CEdit edit_username;
	CEdit edit_password;
	CString username;
	CST_PTTClientDlg dlg;
	CString  password;
	ClientUserInfo clientUserInfo;
	ClientChannelInfo clientChannelInfo;
	bool m_connect;
	
	ClientSocket* pSock;         
	ClientUdpSocket* pUDPSocket;// 客户端套接字指针对象

	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonExit();
	afx_msg LRESULT DEALWITH_CLOSEDIALOG(WPARAM wParam,LPARAM lparam);
		void InitConsoleWindow();
	int bytesToInt(char* bytes,int size);
	virtual BOOL PreTranslateMessage(MSG* pMsg); 
	 static UINT MyThreadFunction( LPVOID pParam );
	//防止按下enter、esc时退出程序
};
