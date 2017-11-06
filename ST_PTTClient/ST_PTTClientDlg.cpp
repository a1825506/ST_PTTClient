
// ST_PTTClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ST_PTTClient.h"
#include "ST_PTTClientDlg.h"
#include "PTTClientControl.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CST_PTTClientDlg 对话框




CST_PTTClientDlg::CST_PTTClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CST_PTTClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CST_PTTClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEB_TREE, m_tc);
	DDX_Control(pDX, IDC_EXPLORER1, m_Map);
	DDX_Control(pDX,IDC_BUTRECEIVE,m_btnReceive);
	DDX_Control(pDX,IDC_BUTHANGOUT,m_btnHangout);
	DDX_Control(pDX,IDC_CALLSTATUS,m_CallStatus);
	
}

BEGIN_MESSAGE_MAP(CST_PTTClientDlg, CDialogEx)
	  ON_WM_PAINT()
	  ON_WM_QUERYDRAGICON()
	  ON_WM_SIZE()
	  ON_NOTIFY(NM_RCLICK, IDC_WEB_TREE,OnRclickTreeindicators)
	   ON_NOTIFY(NM_CLICK, IDC_WEB_TREE, OnClkTreeindicators)
	   ON_BN_CLICKED(ID_CALLUSER,OnBnClickCallUser)
	   	ON_MESSAGE(WM_CALLUSER,DEALWITH_CALLUSER)
	  ON_WM_CONTEXTMENU()
	  ON_WM_TIMER()
	  ON_BN_CLICKED(IDC_BTNTALK, &CST_PTTClientDlg::OnBnClickedBtntalk)
	  ON_BN_CLICKED(IDC_BTN_PTTTALK, &CST_PTTClientDlg::OnBnClickedBtnPtttalk)
	  ON_BN_CLICKED(IDC_BUTHANGOUT, &CST_PTTClientDlg::OnBnClickedButhangout)

	  ON_BN_CLICKED(IDC_BUTRECEIVE, &CST_PTTClientDlg::OnBnClickedButreceive)
END_MESSAGE_MAP()


// CST_PTTClientDlg 消息处理程序

BOOL CST_PTTClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 获取对话框初始大小    
	GetClientRect(&m_rect);  //获取对话框的大小
	old.x=m_rect.right-m_rect.left;  
	old.y=m_rect.bottom-m_rect.top;

	// TODO: 在此添加额外的初始化代码
	m_pAudioPlayer=NULL;
	//声明指向节点的句柄
	m_btnReceive.LoadBitmaps(IDB_BITMAP1,NULL);

	m_btnHangout.LoadBitmaps(IDB_BITMAP2,NULL);

	//CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP); // 得到 Picture Control 句柄  
	//CImage image;  
	//image.Load("map_pressed.png");  
	//HBITMAP hBmp = image.Detach();  
	//pWnd->SetBitmap(hBmp);  


	HTREEITEM hItem,hSubItem,hThirdItem;

	HICON  icon[4];
	icon[0]=AfxGetApp()->LoadIcon(IDI_ICON1);
	icon[1]=AfxGetApp()->LoadIcon(IDI_ICON2);
	icon[2]=AfxGetApp()->LoadIcon(IDI_ICON3);
	CImageList *ImageList4Tree = new CImageList;
	ImageList4Tree->Create(16,16,0,4,4); //16,16为图标分辩率，4,4为该list最多能容纳的图标数
	for(int i=0;i<4;i++)
	{
		ImageList4Tree->Add(icon[i]); //读入图标
	}
	m_tc.SetImageList(ImageList4Tree,TVSIL_NORMAL);
	//在根结点上添加 Parent2
	hItem = m_tc.InsertItem("常规频道管理",1,1,TVI_ROOT,hItem);

	

	ClientChannelInfo clientChannelInfo;

	size_t channel_len = CPublic::V_ClientChannelInfo.size();

	ClientUserInfo clientUserInfo;

	size_t len = CPublic::V_ClientUserInfo.size();


	for (size_t i =0; i < channel_len; i ++) {

		clientChannelInfo = CPublic::V_ClientChannelInfo[i];

		if(clientChannelInfo.getChannelidtype()==1){

			hSubItem = m_tc.InsertItem(clientChannelInfo.getChannelname(),1, 1,hItem);

			for (size_t j =0; j < len; j ++) {

				

				clientUserInfo = CPublic::V_ClientUserInfo[j];

				CString user_channel = clientUserInfo.getGroup();

				if(clientUserInfo.getUserid()!=CPublic::currentUserID){

					if(user_channel.Find(clientChannelInfo.getChannelname())<0){

					}else{


						if(clientUserInfo.getOnline()==1){

							hThirdItem = m_tc.InsertItem(clientUserInfo.getUsername(), 0, 0, hSubItem);
						}else 
							hThirdItem = m_tc.InsertItem(clientUserInfo.getUsername(), 2, 2, hSubItem);

					}
				}
			}

		}
	}

	
	//在 Parent2 上添加一个子结点 Child2_1

	//在根结点上添加 Parent2
	hItem = m_tc.InsertItem("临时频道管理",1,1,TVI_ROOT,hItem);


	hSubItem=m_tc.InsertItem("频道1",1,1,hItem);


	hSubItem = m_tc.InsertItem("频道2",1,1,hItem,hSubItem);

	CRect rect;
	m_Map.GetClientRect(&rect);
	m_Map.ScreenToClient(&rect);
	m_Map.SetWindowPos(NULL,0,0,m_rect.Width(),m_rect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	m_Map.Navigate("D:\\tao\\C++\\ST_PTTClient\\ST_PTTClient\\geodemo.html",NULL,NULL,NULL,NULL);

	

	//m_Map.Navigate(L"file:///D:/tao/C++/ST_PTTClient/ST_PTTClient/geodemo.html",NULL,NULL,NULL,NULL);//加载Google-earth.html页面

	CPublic::createNotifty();

	CPublic::heartBeat();

	 SetTimer(1,1000*20,NULL); //20秒触发一次

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CST_PTTClientDlg::ReSize(void)  
{  
	float fsp[2];  
	POINT Newp; //获取现在对话框的大小  
	CRect recta;      
	GetClientRect(&recta);     //取客户区大小    
	Newp.x=recta.right-recta.left;  
	Newp.y=recta.bottom-recta.top;  
	fsp[0]=(float)Newp.x/old.x;  
	fsp[1]=(float)Newp.y/old.y;  
	CRect Rect;  
	int woc;  
	CPoint OldTLPoint,TLPoint; //左上角  
	CPoint OldBRPoint,BRPoint; //右下角  
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件    
	while(hwndChild){      
		woc=::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(Rect);    
		ScreenToClient(Rect);    
		OldTLPoint = Rect.TopLeft();    
		TLPoint.x = long(OldTLPoint.x*fsp[0]);    
		TLPoint.y = long(OldTLPoint.y*fsp[1]);    
		OldBRPoint = Rect.BottomRight();    
		BRPoint.x = long(OldBRPoint.x *fsp[0]);    
		BRPoint.y = long(OldBRPoint.y *fsp[1]);    
		Rect.SetRect(TLPoint,BRPoint);    
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);  
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);      
	}
	old=Newp; 

}

void CST_PTTClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType,cx,cy);
	if(nType!=SIZE_MINIMIZED )  //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	{
		ReSize();
	}
}

void CST_PTTClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CST_PTTClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CST_PTTClientDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent){
		CPublic::heartBeat();
	}else if(4==nIDEvent){
		
		GetDlgItem(IDC_PPTTALK)->SetWindowText("松开停止讲话");


	}
}
BOOL CST_PTTClientDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_PPTTALK)->m_hWnd){
		//按下对讲按钮
		SetTimer(4,20,NULL);
		//开启音频采集线程。
		m_pAudioPlayer = new CAudioRecord();
		m_pAudioPlayer->Start();
	}
	if(pMsg->message == WM_LBUTTONUP&& pMsg->hwnd == GetDlgItem(IDC_PPTTALK)->m_hWnd){
		//抬起对讲按钮
		KillTimer(4);
		GetDlgItem(IDC_PPTTALK)->SetWindowText("请按住讲话");
		//停止音频采集线程
		m_pAudioPlayer->Stop();
		//char sendBuf[2]={0x05,0xA9};
		//send(sockClient,sendBuf,2,0);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CST_PTTClientDlg::OnRclickTreeindicators(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW *pNmList = (NMLISTVIEW*)pNMHDR;



	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CMenu* subMenu = menu.GetSubMenu(0);
		CPoint pt;
		GetCursorPos(&pt);
		subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}
}

void CST_PTTClientDlg::OnContextMenu(CWnd* pWnd, CPoint ptMousePos) 
{
	
}

void CST_PTTClientDlg::OnClkTreeindicators(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	GetCursorPos(&point);//获得鼠标点击的位置

	m_tc.ScreenToClient(&point);//转化为客户坐标

	UINT uFlags;

	HTREEITEM CurrentItem;

	CurrentItem=m_tc.HitTest(point,&uFlags);//获得当前点击节点的ITEM
	//获得该节点的父节点

	 CurrentChoose=m_tc.GetItemText(CurrentItem) ;//获得节点值
}




void CST_PTTClientDlg::OnBnClickedBtntalk()
{
	m_CTalkInfo.EnableWindow(TRUE);
	//GetDlgItem(IDC_TALK_STATUS)->EnableWindow(FALSE);
	
}


void CST_PTTClientDlg::OnBnClickedBtnPtttalk()
{
	m_CTalkInfo.EnableWindow(FALSE);
	//GetDlgItem(IDC_TALK_STATUS)->EnableWindow(TR);
	
}

void CST_PTTClientDlg::OnBnClickCallUser()
{
	
//	m_btnReceive.ShowWindow(SW_NORMAL);


	CPublic::tempTalkOrder(CurrentChoose+":"+CPublic::currentUserName,CurrentChoose+"-"+CPublic::currentUserName);
	//
	//发送呼叫命令
	
}


void CST_PTTClientDlg::OnBnClickedButhangout()
{
	//m_btnReceive.ShowWindow(SW_HIDE);
	m_CallStatus.SetWindowText("无呼叫");
	m_btnHangout.ShowWindow(SW_HIDE);
	m_btnReceive.ShowWindow(SW_HIDE);
}

LRESULT CST_PTTClientDlg::DEALWITH_CALLUSER(WPARAM wParam,LPARAM lparam)
{

	int flag= wParam;

	if(flag==1){

		CString calluser;

		calluser.Format("正在呼叫：%s",CurrentChoose);

		m_CallStatus.SetWindowText(calluser);

		m_btnHangout.ShowWindow(SW_NORMAL);

	}else if(flag==0){

		AfxMessageBox("创建临时频道失败");

	}else if(flag==2){

		CPublic::sendChannelSwitchOrder();//切换频道

		m_CallStatus.SetWindowText("通话中,按住对讲按钮讲话");


	}else if(flag==WM_UPLOADCHANNELID){
		//上报当前频道的ID。
	}else if(flag==3){

		m_CallStatus.SetWindowText("user1 正在呼叫您");

		m_btnHangout.ShowWindow(SW_NORMAL);

		m_btnReceive.ShowWindow(SW_NORMAL);
	
	}



	return 0;
}


void CST_PTTClientDlg::OnBnClickedButreceive()
{
	// TODO: 在此添加控件通知处理程序代码
}
