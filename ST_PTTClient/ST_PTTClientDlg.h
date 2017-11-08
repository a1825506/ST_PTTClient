
// ST_PTTClientDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "explorer1.h"
#include "CPublic.h"
#include "AudioRecord.h"

// CST_PTTClientDlg 对话框
class CST_PTTClientDlg : public CDialogEx
{
// 构造
public:
	CST_PTTClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ST_PTTCLIENT_DIALOG };

	protected:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CAudioRecord*		m_pAudioPlayer;
	CRect m_rect;
	void ChangeSize(UINT nID,int x,int y);
	void ReSize(void);  
	POINT old; 
	 afx_msg void OnSize(UINT nType, int cx, int cy);

	 	afx_msg void OnTimer(UINT nIDEvent);
	// 生成的消息映射函数
	 virtual BOOL OnInitDialog();
	afx_msg void OnRclickTreeindicators(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnClkTreeindicators (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint ptMousePos);
		 afx_msg LRESULT DEALWITH_CALLUSER(WPARAM wParam,LPARAM lparam);

	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tc;
	CExplorer1 m_Map;
	CString CurrentChoose;
	CEdit  m_CTalkInfo;
    CStatic m_CallStatus;
	CBitmapButton  m_btnReceive,m_btnHangout;
	afx_msg void OnBnClickedPpttalk();
	afx_msg void OnTvnSelchangedWebTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtntalk();
	afx_msg void OnBnClickedBtnPtttalk();
	afx_msg void OnBnClickCallUser();
	afx_msg void OnBnClickedButhangout();
	afx_msg void OnBnClickedButreceive();
};
