// PTTClientControl.cpp : 实现文件
//

#include "stdafx.h"
#include "ST_PTTClient.h"
#include "ST_PTTClientDlg.h"
#include "PTTClientControl.h"
#include "afxdialogex.h"
#include "ClientSocket.h"
#include "ClientUserInfo.h"

#include<vector>
#include <io.h>
#include <fcntl.h>
using namespace std;
// PTTClientControl 对话框

IMPLEMENT_DYNAMIC(PTTClientControl, CDialogEx)

PTTClientControl::PTTClientControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(PTTClientControl::IDD, pParent)
{

}

PTTClientControl::~PTTClientControl()
{
}

void PTTClientControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edit_username);
	DDX_Control(pDX, IDC_EDIT2, edit_password);
}


BEGIN_MESSAGE_MAP(PTTClientControl, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &PTTClientControl::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON2, &PTTClientControl::OnBnClickedButtonExit)
	ON_MESSAGE(WM_CLOSEDIALOGINFO,DEALWITH_CLOSEDIALOG)

	

END_MESSAGE_MAP()


// PTTClientControl 消息处理程序

BOOL PTTClientControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	edit_username.SetWindowText("user2");

	edit_password.SetWindowText("user2");

	edit_password.SetPasswordChar('*');

	InitConsoleWindow();

	m_connect=false;
	
	return TRUE;
}

void PTTClientControl::InitConsoleWindow()
{
	int nCrt = 0;

	FILE* fp;
	AllocConsole();
	nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	fp = _fdopen(nCrt, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}


void PTTClientControl::OnBnClickedButtonLogin()
{

	AfxSocketInit();
	// 启动线程。创建连接
	if (m_connect)                             // 如果已经连接，则断开服务器
	{
		m_connect = false;

		pSock->Close();

		delete pSock;

		pSock = NULL;

		return;

	}else{

		pSock = new ClientSocket();

		if (!pSock->Create())         //创建套接字
		{
			AfxMessageBox(_T("创建套接字失败！"));
			return;
		}
	}
	if (!pSock->Connect(CPublic::serverIP, CPublic::serverPort))    //连接服务器
	{
		AfxMessageBox(_T("连接服务器失败！"));

		return;

	}else{

		CPublic::setClientSocket(pSock);

		m_connect = true;

		UpdateData(false);

		edit_username.GetWindowText(username);  

		edit_password.GetWindowText(password);  

		int str_userLength = username.GetLength();

		int str_passLength = password.GetLength();

		char pData[106] ;

		memset(pData, 0, 106);
	
		char head[]={0x68,0x00,0x64,0x68,0x00,0x00,0x00,0x01};

		memcpy(pData, head, 8);

		memcpy(pData+8, username,str_userLength);

		memcpy(pData+72, password,str_passLength);

		int sum=0;

		for(int i=4;i<104;i++){

			sum+=pData[i];
		}

		pData[104]=(byte)sum;

		pData[105]=0x16;

		pSock->SendMSG(pData,106);
		
	}
}

UINT PTTClientControl:: MyThreadFunction( LPVOID pParam )
{
	//函数体


	

	return 0;
}


LRESULT PTTClientControl::DEALWITH_CLOSEDIALOG(WPARAM wParam,LPARAM lparam)
{
	char * info = (char *)lparam;

	int length = wParam;

	if(info[7]==1){

		if(info[8]==1){

			char userid[4] ;

			memcpy(userid, info+9, 4);

			CPublic::currentUserID=userid[3] + userid[2]*256 +userid[1]*65536 + userid[0]*16777216;

			CPublic::currentUserName = username;

			printf( "当前用户id：%d,%s\n",CPublic::currentUserID,	CPublic::currentUserName );

		}else if(info[8]==0){

			AfxMessageBox("账号未注册");

		}else if(info[8]==2){

			AfxMessageBox("账号被禁止");

		}else if(info[8]==3){

			AfxMessageBox("登录失败");
		}

	}else if(info[7]==4){

			printf( "所有用户消息 %d\n",length);

			char usernum[4];//记录用户个数

			memcpy(usernum, info+8,4);

			int user_num = usernum[3] + usernum[2]*256 +usernum[1]*65536 + usernum[0]*16777216;

			vector <int > index_list;

			vector <int > index_list1;

			char* userinfo = NULL;

			userinfo = new char[length-18];//去掉头尾的用户信息数据

			memcpy(userinfo, info+12,length-18);

			int temp=0;

			for(int i=0;i<user_num;i++){

				ClientUserInfo clientUserInfo;
			
				//找 用户名 后的第一个 "-" 分割符下标
				for(int k=temp;k<length-18;k++){

					if(userinfo[k]==45){

						index_list.push_back(k);

						break;
					}
				}

				char* byte_username = NULL;

				byte_username = new char[index_list[0]-temp];

				memset(byte_username, 0, sizeof(index_list[0]-temp));

				memcpy(byte_username, userinfo+(temp+1),index_list[0]-temp-1);

				byte_username[index_list[0]-temp-1]='\0';

				CString str_username(byte_username);//用户名按字符串处理

				clientUserInfo.setUsername(str_username);

				printf("用户名：%s \n",str_username);

				char byte_userid[4];

				memcpy(byte_userid, userinfo+(index_list[0]+1),4);

				//int int_userid = byte_userid[3] + byte_userid[2]*256 +byte_userid[1]*65536 + byte_userid[0]*16777216;

				int int_userid = byte_userid[3] & 0xFF;  
				int_userid |= ((byte_userid[2] << 8) & 0xFF00);  
				int_userid |= ((byte_userid[1] << 16) & 0xFF0000);  
				int_userid |= ((byte_userid[0] << 24) & 0xFF000000); 

				char str[16];

				sprintf(str,"%X",int_userid);

				  clientUserInfo.setUserid(int_userid);
			
				printf("用户ID%d：\n",int_userid);

				char byte_userbans[4];

				memcpy(byte_userbans, userinfo+(temp+index_list[0]-temp-1+7),4);

				int int_userbans = byte_userbans[3] & 0xFF;  

				int_userbans |= ((byte_userbans[2] << 8) & 0xFF00);  

				int_userbans |= ((byte_userbans[1] << 16) & 0xFF0000);  

				int_userbans |= ((byte_userbans[0] << 24) & 0xFF000000); 


			  clientUserInfo.setIsforbidden(int_userbans);

				printf("用户是否被禁用:%d \n",int_userbans);

				char byte_useronline[4];

				memcpy(byte_useronline, userinfo+(temp+index_list[0]-temp-1+12),4);

				int int_userlogin = byte_useronline[3] & 0xFF;  

				int_userlogin |= ((byte_useronline[2] << 8) & 0xFF00);  

				int_userlogin |= ((byte_useronline[1] << 16) & 0xFF0000);  

				int_userlogin |= ((byte_useronline[0] << 24) & 0xFF000000); 

				clientUserInfo.setOnline(int_userlogin);

				printf("用户是否登录：%d\n",int_userlogin );

				char byte_groupnum[4];

				memcpy(byte_groupnum, userinfo+(temp+index_list[0]-temp-1+17),4);

				int int_groupnum = byte_groupnum[3] & 0xFF;  

				int_groupnum |= ((byte_groupnum[2] << 8) & 0xFF00);  

				int_groupnum |= ((byte_groupnum[1] << 16) & 0xFF0000);  

				int_groupnum |= ((byte_groupnum[0] << 24) & 0xFF000000); 

				printf("用户所属组个数：%d\n",int_groupnum );
				// 找到所属组名后的第一个“#”分割符号
				for(int j=temp+index_list[0]-temp-1+22;j<length-18;j++){

					if(userinfo[j]==35){

						index_list1.push_back(j);

						break;
					}

				}

				if(index_list1.size()==0){
					//最后一组数据，没有分割符

					char  * byte_groupname=NULL;

					byte_groupname = new char[length-(temp+index_list[0]-temp-1+22)+1];

					memcpy(byte_groupname,userinfo+(temp+index_list[0]-temp-1+22),length-(temp+index_list[0]-temp-1+22));

					byte_groupname[length-(temp+index_list[0]-temp-1+22)]='\0';

					CString str_groupnum(byte_groupname);//用户名按字符串处理

					clientUserInfo.setGroup(str_groupnum);
				
					printf("用户所属群组%s\n",str_groupnum);

				}else{

					char * byte_groupname=NULL;

					byte_groupname = new char[index_list1[0]-(temp+index_list[0]-temp-1+22)+1];

					memcpy(byte_groupname,userinfo+(temp+index_list[0]-temp-1+22),index_list1[0]-(temp+index_list[0]-temp-1+22));

					byte_groupname[index_list1[0]-(temp+index_list[0]-temp-1+22)]='\0';

					CString str_groupnum(byte_groupname);//用户名按字符串处理

					clientUserInfo.setGroup(str_groupnum);
				
					printf("用户所属群组%s\n",str_groupnum);
				
					temp=index_list1[0];

				}

				index_list.clear();
				index_list1.clear();
				CPublic::V_ClientUserInfo.push_back(clientUserInfo);
			}
		
	}else if(info[7]==56){

		byte groupnum[4];//记录群组个数

		memcpy(groupnum,info+8,4);

		int group_num = groupnum[3] & 0xFF;  

		group_num |= ((groupnum[2] << 8) & 0xFF00);  

		group_num |= ((groupnum[1] << 16) & 0xFF0000);  

		group_num |= ((groupnum[0] << 24) & 0xFF000000); 

		char* groupinfo = NULL;

		groupinfo = new char[length-14];//去掉头尾的组信息数据

		memcpy(groupinfo, info+12,length-14);

		int temp=0;

		vector <int > index_list;

		vector <int > index_list1;

		for(int i=0;i<group_num;i++){

			ClientChannelInfo clientChannelInfo;

			//遍历组名后的第一个:分割符。组名规定为字母 数字 下划线
			for(int k=temp;k<length-14;k++){

				if(groupinfo[k]==58){

					index_list.push_back(k);

					break;
				}
			}

			char* byte_groupname = NULL;

			byte_groupname = new char[index_list[0]-temp];

			memset(byte_groupname, 0, sizeof(index_list[0]-temp-1));

			memcpy(byte_groupname, groupinfo+(temp+1),index_list[0]-temp-1);

			byte_groupname[index_list[0]-temp-1]='\0';

			CString str_groupname(byte_groupname);//频道名按字符串处理

			clientChannelInfo.setChannelname(str_groupname);

			printf("频道名：%s \n",str_groupname);

			char byte_groupid[4];

			memcpy(byte_groupid, groupinfo+(index_list[0]+1),4);

			int int_groupid = byte_groupid[3] & 0xFF;  

			int_groupid |= ((byte_groupid[2] << 8) & 0xFF00);  

			int_groupid |= ((byte_groupid[1] << 16) & 0xFF0000);  

			int_groupid |= ((byte_groupid[0] << 24) & 0xFF000000); 

			char str[16];

			sprintf(str,"%X",int_groupid);

			clientChannelInfo.setChannelid(int_groupid);

			printf("频道ID :%d\n",int_groupid);

			char byte_grouptype[4];

			memcpy(byte_grouptype, groupinfo+(temp+index_list[0]-temp-1+7),4);

			int int_type = byte_grouptype[3] & 0xFF;  

			int_type |= ((byte_grouptype[2] << 8) & 0xFF00);  

			int_type |= ((byte_grouptype[1] << 16) & 0xFF0000);  

			int_type |= ((byte_grouptype[0] << 24) & 0xFF000000); 

			clientChannelInfo.setChannelidtype(int_type);
			
			printf("频道类型%d：\n",int_type);

			// 找到频道类型后的第一个“#”分割符号
			for(int j=temp+index_list[0]-temp-1+7;j<length-14;j++){

				if(groupinfo[j]==35){

					index_list1.push_back(j);

					break;
				}
			}
			if(index_list1.size()!=0){

				temp=index_list1[0];
			}

			index_list.clear();

			index_list1.clear();

			CPublic::V_ClientChannelInfo.push_back(clientChannelInfo);
			//上报当前频道的ID
		}

		CPublic::uploadChannelid();

	 	this->EndDialog(SW_HIDE); //隐藏父窗体   

		INT_PTR nResponse = dlg.DoModal();//显示子窗体

	}else if(info[7]==8){

	 printf("上班当前ID返回信息\n");

    }else if(info[7]==16){

		 printf("通知帧返回信息\n");

		 char destip[4];

		 char destport[2];

		 memcpy(destip, info+8,4);

		 memcpy(destport, info+12,2);

		 CPublic:: SERVER_DESTPORT = destport[1] & 0xFF;  

		 CPublic:: SERVER_DESTPORT |= ((destport[0] << 8) & 0xFF00);  

		 printf("UDP远程连接端口:%d \n", CPublic:: SERVER_DESTPORT);

		 pUDPSocket = new ClientUdpSocket();

		 bool iscreate=pUDPSocket->Create(CPublic::LOCAL_RECEVED_STREAM_PORT,SOCK_DGRAM);

		 bool isbind=pUDPSocket->Bind(CPublic::LOCAL_RECEVED_STREAM_PORT,"127.0.0.1");

		// easyRTP = new CST_EasyRTP();

		 
		if(iscreate){

			

			CPublic::setUdpClientSocket(pUDPSocket);

			//easyRTP->RTPReceive();

			 printf("RTP连接创建成功\n");

		   /* CWinThread* m_pThread=AfxBeginThread(MyThreadFunction,(LPVOID)easyRTP);

			 if(NULL == m_pThread)
			 {
				 printf("创建新的线程出错！\n");
			}else{
				 printf("创建新的线程成功！\n");
			 }*/

		}else
			printf("UDP连接创建失败\n");

	}else if(info[7]==48){
		

		if (info[8] == 0) {
			//创建失败
		     dlg.SendMessage(WM_CALLUSER,0,NULL);
			//::SendMessage(AfxGetMainWnd()->m_hWnd,WM_CALLUSER,0,NULL);
			printf("创建临时频道失败\n");
			
		} else if (info[8] == 1) {
			//创建成功，发送创建成功广播
		     dlg.SendMessage(WM_CALLUSER,1,NULL);
		  	printf("创建临时频道成功\n");
		}
	}else if(info[7]==7){

		char byte_recTemp[4] = {0};

		 memcpy(byte_recTemp, info+16,4);

		 int int_recTemp = byte_recTemp[3] & 0xFF;  

		 int_recTemp |= ((byte_recTemp[2] << 8) & 0xFF00);  

		 int_recTemp |= ((byte_recTemp[1] << 16) & 0xFF0000);  

		 int_recTemp |= ((byte_recTemp[0] << 24) & 0xFF000000); 

		 char userid[4] = {0};

		  memcpy(userid, info+12,4);

		  int int_userid = userid[3] & 0xFF;  

		  int_userid |= ((userid[2] << 8) & 0xFF00);  

		  int_userid |= ((userid[1] << 16) & 0xFF0000);  

		  int_userid |= ((userid[0] << 24) & 0xFF000000);

		  if (int_recTemp == 1) {
			  //接听
			  if(CPublic::currentUserID!=int_userid){

				    dlg.SendMessage(WM_CALLUSER,2,NULL);

					char str[16];

					sprintf(str,"%X",int_userid);

					printf("接听用户ID%s：\n",str);
			  }

		  }else{

			  dlg.SendMessage(WM_CALLUSER,2,NULL);
		  }

	}else if(info[7] == 57){

		char result[1] ={0} ;

		char channel_id[4] = {0};

		memcpy(result, info+8,1);

		memcpy(channel_id, info+9,4);

		int   int_result=0;

		int_result |= result[0] & 0xFF;  

		int int_channel_id = channel_id[3] & 0xFF;  

		int_channel_id |= ((channel_id[2] << 8) & 0xFF00);  

		int_channel_id |= ((channel_id[1] << 16) & 0xFF0000);  

		int_channel_id |= ((channel_id[0] << 24) & 0xFF000000);

		if(int_result==1){
			//切换成功

			CPublic::currentUserID = int_channel_id;//需要切换频道的ID

			printf("切换频道成功信息\n");

		}else{
			//切换失败

			printf("切换频道失败信息 %d\n",int_result);
		}

	}else if(info[7] == 58){

	     printf("临时频道信息\n");

		 ClientChannelInfo clientChannelInfo;

		 char byte_tempChannelID[4];

		 memcpy(byte_tempChannelID,info+8 ,4);
		 //临时编组的ID，需要在手机客户端保存。每次切换频道的时候需要将该ID告诉服务器。

		 int int_tempChannelID = 0;

		 int_tempChannelID = byte_tempChannelID[3] & 0xFF;  

		 int_tempChannelID |= ((byte_tempChannelID[2] << 8) & 0xFF00);  

		 int_tempChannelID |= ((byte_tempChannelID[1] << 16) & 0xFF0000);  

		 int_tempChannelID |= ((byte_tempChannelID[0] << 24) & 0xFF000000);

		 CPublic::switchChannelID = int_tempChannelID;

		 //获取用户名列表。按字符串处理
		 char byte_username[64];

		  memcpy(byte_username,info+12 ,64);

		  CString str_groupname(byte_username);//频道名按字符串处理

		  //弹出接听 挂断按钮
		   dlg.SendMessage(WM_CALLUSER,3,NULL);
	}else if(info[7] == -111){
		   
		if(info[8] == 0){
			//通道申请失败不可以发送语音对讲
			printf("通道申请失败\n");
		}else if(info[8] == 1){
			//通道申请成功才可以发送语音对讲
			//printf("通道申请成功\n");
		}
		
	}

	delete info;

	return 0;
}




void PTTClientControl::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代码
	this->EndDialog(IDC_BUTTON2);
}

BOOL PTTClientControl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)    
		return TRUE; 
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)    
		return TRUE; 
	return CDialog::PreTranslateMessage(pMsg);
}
