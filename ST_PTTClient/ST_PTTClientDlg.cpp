
// ST_PTTClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ST_PTTClient.h"
#include "ST_PTTClientDlg.h"
#include "PTTClientControl.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CST_PTTClientDlg �Ի���




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


// CST_PTTClientDlg ��Ϣ�������

BOOL CST_PTTClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ��ȡ�Ի����ʼ��С    
	GetClientRect(&m_rect);  //��ȡ�Ի���Ĵ�С
	old.x=m_rect.right-m_rect.left;  
	old.y=m_rect.bottom-m_rect.top;

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pAudioPlayer=NULL;
	//����ָ��ڵ�ľ��
	m_btnReceive.LoadBitmaps(IDB_BITMAP1,NULL);

	m_btnHangout.LoadBitmaps(IDB_BITMAP2,NULL);

	//CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP); // �õ� Picture Control ���  
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
	ImageList4Tree->Create(16,16,0,4,4); //16,16Ϊͼ��ֱ��ʣ�4,4Ϊ��list��������ɵ�ͼ����
	for(int i=0;i<4;i++)
	{
		ImageList4Tree->Add(icon[i]); //����ͼ��
	}
	m_tc.SetImageList(ImageList4Tree,TVSIL_NORMAL);
	//�ڸ��������� Parent2
	hItem = m_tc.InsertItem("����Ƶ������",1,1,TVI_ROOT,hItem);

	

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

	
	//�� Parent2 �����һ���ӽ�� Child2_1

	//�ڸ��������� Parent2
	hItem = m_tc.InsertItem("��ʱƵ������",1,1,TVI_ROOT,hItem);


	hSubItem=m_tc.InsertItem("Ƶ��1",1,1,hItem);


	hSubItem = m_tc.InsertItem("Ƶ��2",1,1,hItem,hSubItem);

	CRect rect;
	m_Map.GetClientRect(&rect);
	m_Map.ScreenToClient(&rect);
	m_Map.SetWindowPos(NULL,0,0,m_rect.Width(),m_rect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	m_Map.Navigate("D:\\tao\\C++\\ST_PTTClient\\ST_PTTClient\\geodemo.html",NULL,NULL,NULL,NULL);

	

	//m_Map.Navigate(L"file:///D:/tao/C++/ST_PTTClient/ST_PTTClient/geodemo.html",NULL,NULL,NULL,NULL);//����Google-earth.htmlҳ��

	CPublic::createNotifty();

	CPublic::heartBeat();

	 SetTimer(1,1000*20,NULL); //20�봥��һ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CST_PTTClientDlg::ReSize(void)  
{  
	float fsp[2];  
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
	CRect recta;      
	GetClientRect(&recta);     //ȡ�ͻ�����С    
	Newp.x=recta.right-recta.left;  
	Newp.y=recta.bottom-recta.top;  
	fsp[0]=(float)Newp.x/old.x;  
	fsp[1]=(float)Newp.y/old.y;  
	CRect Rect;  
	int woc;  
	CPoint OldTLPoint,TLPoint; //���Ͻ�  
	CPoint OldBRPoint,BRPoint; //���½�  
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ�    
	while(hwndChild){      
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID  
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
	if(nType!=SIZE_MINIMIZED )  //�жϴ����ǲ�����С���ˣ���Ϊ������С��֮�� �����ڵĳ��Ϳ����0����ǰһ�α仯��ʱ�ͻ���ֳ���0�Ĵ������
	{
		ReSize();
	}
}

void CST_PTTClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CST_PTTClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CST_PTTClientDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent){
		CPublic::heartBeat();
	}else if(4==nIDEvent){
		
		GetDlgItem(IDC_PPTTALK)->SetWindowText("�ɿ�ֹͣ����");


	}
}
BOOL CST_PTTClientDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == GetDlgItem(IDC_PPTTALK)->m_hWnd){
		//���¶Խ���ť
		SetTimer(4,20,NULL);
		//������Ƶ�ɼ��̡߳�
		m_pAudioPlayer = new CAudioRecord();
		m_pAudioPlayer->Start();
	}
	if(pMsg->message == WM_LBUTTONUP&& pMsg->hwnd == GetDlgItem(IDC_PPTTALK)->m_hWnd){
		//̧��Խ���ť
		KillTimer(4);
		GetDlgItem(IDC_PPTTALK)->SetWindowText("�밴ס����");
		//ֹͣ��Ƶ�ɼ��߳�
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
	GetCursorPos(&point);//����������λ��

	m_tc.ScreenToClient(&point);//ת��Ϊ�ͻ�����

	UINT uFlags;

	HTREEITEM CurrentItem;

	CurrentItem=m_tc.HitTest(point,&uFlags);//��õ�ǰ����ڵ��ITEM
	//��øýڵ�ĸ��ڵ�

	 CurrentChoose=m_tc.GetItemText(CurrentItem) ;//��ýڵ�ֵ
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
	//���ͺ�������
	
}


void CST_PTTClientDlg::OnBnClickedButhangout()
{
	//m_btnReceive.ShowWindow(SW_HIDE);
	m_CallStatus.SetWindowText("�޺���");
	m_btnHangout.ShowWindow(SW_HIDE);
	m_btnReceive.ShowWindow(SW_HIDE);
}

LRESULT CST_PTTClientDlg::DEALWITH_CALLUSER(WPARAM wParam,LPARAM lparam)
{

	int flag= wParam;

	if(flag==1){

		CString calluser;

		calluser.Format("���ں��У�%s",CurrentChoose);

		m_CallStatus.SetWindowText(calluser);

		m_btnHangout.ShowWindow(SW_NORMAL);

	}else if(flag==0){

		AfxMessageBox("������ʱƵ��ʧ��");

	}else if(flag==2){

		CPublic::sendChannelSwitchOrder();//�л�Ƶ��

		m_CallStatus.SetWindowText("ͨ����,��ס�Խ���ť����");


	}else if(flag==WM_UPLOADCHANNELID){
		//�ϱ���ǰƵ����ID��
	}else if(flag==3){

		m_CallStatus.SetWindowText("user1 ���ں�����");

		m_btnHangout.ShowWindow(SW_NORMAL);

		m_btnReceive.ShowWindow(SW_NORMAL);
	
	}



	return 0;
}


void CST_PTTClientDlg::OnBnClickedButreceive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
