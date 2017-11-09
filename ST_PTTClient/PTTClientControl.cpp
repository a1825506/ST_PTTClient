// PTTClientControl.cpp : ʵ���ļ�
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
// PTTClientControl �Ի���

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


// PTTClientControl ��Ϣ�������

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
	// �����̡߳���������
	if (m_connect)                             // ����Ѿ����ӣ���Ͽ�������
	{
		m_connect = false;

		pSock->Close();

		delete pSock;

		pSock = NULL;

		return;

	}else{

		pSock = new ClientSocket();

		if (!pSock->Create())         //�����׽���
		{
			AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
			return;
		}
	}
	if (!pSock->Connect(CPublic::serverIP, CPublic::serverPort))    //���ӷ�����
	{
		AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));

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
	//������


	

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

			printf( "��ǰ�û�id��%d,%s\n",CPublic::currentUserID,	CPublic::currentUserName );

		}else if(info[8]==0){

			AfxMessageBox("�˺�δע��");

		}else if(info[8]==2){

			AfxMessageBox("�˺ű���ֹ");

		}else if(info[8]==3){

			AfxMessageBox("��¼ʧ��");
		}

	}else if(info[7]==4){

			printf( "�����û���Ϣ %d\n",length);

			char usernum[4];//��¼�û�����

			memcpy(usernum, info+8,4);

			int user_num = usernum[3] + usernum[2]*256 +usernum[1]*65536 + usernum[0]*16777216;

			vector <int > index_list;

			vector <int > index_list1;

			char* userinfo = NULL;

			userinfo = new char[length-18];//ȥ��ͷβ���û���Ϣ����

			memcpy(userinfo, info+12,length-18);

			int temp=0;

			for(int i=0;i<user_num;i++){

				ClientUserInfo clientUserInfo;
			
				//�� �û��� ��ĵ�һ�� "-" �ָ���±�
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

				CString str_username(byte_username);//�û������ַ�������

				clientUserInfo.setUsername(str_username);

				printf("�û�����%s \n",str_username);

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
			
				printf("�û�ID%d��\n",int_userid);

				char byte_userbans[4];

				memcpy(byte_userbans, userinfo+(temp+index_list[0]-temp-1+7),4);

				int int_userbans = byte_userbans[3] & 0xFF;  

				int_userbans |= ((byte_userbans[2] << 8) & 0xFF00);  

				int_userbans |= ((byte_userbans[1] << 16) & 0xFF0000);  

				int_userbans |= ((byte_userbans[0] << 24) & 0xFF000000); 


			  clientUserInfo.setIsforbidden(int_userbans);

				printf("�û��Ƿ񱻽���:%d \n",int_userbans);

				char byte_useronline[4];

				memcpy(byte_useronline, userinfo+(temp+index_list[0]-temp-1+12),4);

				int int_userlogin = byte_useronline[3] & 0xFF;  

				int_userlogin |= ((byte_useronline[2] << 8) & 0xFF00);  

				int_userlogin |= ((byte_useronline[1] << 16) & 0xFF0000);  

				int_userlogin |= ((byte_useronline[0] << 24) & 0xFF000000); 

				clientUserInfo.setOnline(int_userlogin);

				printf("�û��Ƿ��¼��%d\n",int_userlogin );

				char byte_groupnum[4];

				memcpy(byte_groupnum, userinfo+(temp+index_list[0]-temp-1+17),4);

				int int_groupnum = byte_groupnum[3] & 0xFF;  

				int_groupnum |= ((byte_groupnum[2] << 8) & 0xFF00);  

				int_groupnum |= ((byte_groupnum[1] << 16) & 0xFF0000);  

				int_groupnum |= ((byte_groupnum[0] << 24) & 0xFF000000); 

				printf("�û������������%d\n",int_groupnum );
				// �ҵ�����������ĵ�һ����#���ָ����
				for(int j=temp+index_list[0]-temp-1+22;j<length-18;j++){

					if(userinfo[j]==35){

						index_list1.push_back(j);

						break;
					}

				}

				if(index_list1.size()==0){
					//���һ�����ݣ�û�зָ��

					char  * byte_groupname=NULL;

					byte_groupname = new char[length-(temp+index_list[0]-temp-1+22)+1];

					memcpy(byte_groupname,userinfo+(temp+index_list[0]-temp-1+22),length-(temp+index_list[0]-temp-1+22));

					byte_groupname[length-(temp+index_list[0]-temp-1+22)]='\0';

					CString str_groupnum(byte_groupname);//�û������ַ�������

					clientUserInfo.setGroup(str_groupnum);
				
					printf("�û�����Ⱥ��%s\n",str_groupnum);

				}else{

					char * byte_groupname=NULL;

					byte_groupname = new char[index_list1[0]-(temp+index_list[0]-temp-1+22)+1];

					memcpy(byte_groupname,userinfo+(temp+index_list[0]-temp-1+22),index_list1[0]-(temp+index_list[0]-temp-1+22));

					byte_groupname[index_list1[0]-(temp+index_list[0]-temp-1+22)]='\0';

					CString str_groupnum(byte_groupname);//�û������ַ�������

					clientUserInfo.setGroup(str_groupnum);
				
					printf("�û�����Ⱥ��%s\n",str_groupnum);
				
					temp=index_list1[0];

				}

				index_list.clear();
				index_list1.clear();
				CPublic::V_ClientUserInfo.push_back(clientUserInfo);
			}
		
	}else if(info[7]==56){

		byte groupnum[4];//��¼Ⱥ�����

		memcpy(groupnum,info+8,4);

		int group_num = groupnum[3] & 0xFF;  

		group_num |= ((groupnum[2] << 8) & 0xFF00);  

		group_num |= ((groupnum[1] << 16) & 0xFF0000);  

		group_num |= ((groupnum[0] << 24) & 0xFF000000); 

		char* groupinfo = NULL;

		groupinfo = new char[length-14];//ȥ��ͷβ������Ϣ����

		memcpy(groupinfo, info+12,length-14);

		int temp=0;

		vector <int > index_list;

		vector <int > index_list1;

		for(int i=0;i<group_num;i++){

			ClientChannelInfo clientChannelInfo;

			//����������ĵ�һ��:�ָ���������涨Ϊ��ĸ ���� �»���
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

			CString str_groupname(byte_groupname);//Ƶ�������ַ�������

			clientChannelInfo.setChannelname(str_groupname);

			printf("Ƶ������%s \n",str_groupname);

			char byte_groupid[4];

			memcpy(byte_groupid, groupinfo+(index_list[0]+1),4);

			int int_groupid = byte_groupid[3] & 0xFF;  

			int_groupid |= ((byte_groupid[2] << 8) & 0xFF00);  

			int_groupid |= ((byte_groupid[1] << 16) & 0xFF0000);  

			int_groupid |= ((byte_groupid[0] << 24) & 0xFF000000); 

			char str[16];

			sprintf(str,"%X",int_groupid);

			clientChannelInfo.setChannelid(int_groupid);

			printf("Ƶ��ID :%d\n",int_groupid);

			char byte_grouptype[4];

			memcpy(byte_grouptype, groupinfo+(temp+index_list[0]-temp-1+7),4);

			int int_type = byte_grouptype[3] & 0xFF;  

			int_type |= ((byte_grouptype[2] << 8) & 0xFF00);  

			int_type |= ((byte_grouptype[1] << 16) & 0xFF0000);  

			int_type |= ((byte_grouptype[0] << 24) & 0xFF000000); 

			clientChannelInfo.setChannelidtype(int_type);
			
			printf("Ƶ������%d��\n",int_type);

			// �ҵ�Ƶ�����ͺ�ĵ�һ����#���ָ����
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
			//�ϱ���ǰƵ����ID
		}

		CPublic::uploadChannelid();

	 	this->EndDialog(SW_HIDE); //���ظ�����   

		INT_PTR nResponse = dlg.DoModal();//��ʾ�Ӵ���

	}else if(info[7]==8){

	 printf("�ϰ൱ǰID������Ϣ\n");

    }else if(info[7]==16){

		 printf("֪ͨ֡������Ϣ\n");

		 char destip[4];

		 char destport[2];

		 memcpy(destip, info+8,4);

		 memcpy(destport, info+12,2);

		 CPublic:: SERVER_DESTPORT = destport[1] & 0xFF;  

		 CPublic:: SERVER_DESTPORT |= ((destport[0] << 8) & 0xFF00);  

		 printf("UDPԶ�����Ӷ˿�:%d \n", CPublic:: SERVER_DESTPORT);

		 pUDPSocket = new ClientUdpSocket();

		 bool iscreate=pUDPSocket->Create(CPublic::LOCAL_RECEVED_STREAM_PORT,SOCK_DGRAM);

		 bool isbind=pUDPSocket->Bind(CPublic::LOCAL_RECEVED_STREAM_PORT,"127.0.0.1");

		// easyRTP = new CST_EasyRTP();

		 
		if(iscreate){

			

			CPublic::setUdpClientSocket(pUDPSocket);

			//easyRTP->RTPReceive();

			 printf("RTP���Ӵ����ɹ�\n");

		   /* CWinThread* m_pThread=AfxBeginThread(MyThreadFunction,(LPVOID)easyRTP);

			 if(NULL == m_pThread)
			 {
				 printf("�����µ��̳߳���\n");
			}else{
				 printf("�����µ��̳߳ɹ���\n");
			 }*/

		}else
			printf("UDP���Ӵ���ʧ��\n");

	}else if(info[7]==48){
		

		if (info[8] == 0) {
			//����ʧ��
		     dlg.SendMessage(WM_CALLUSER,0,NULL);
			//::SendMessage(AfxGetMainWnd()->m_hWnd,WM_CALLUSER,0,NULL);
			printf("������ʱƵ��ʧ��\n");
			
		} else if (info[8] == 1) {
			//�����ɹ������ʹ����ɹ��㲥
		     dlg.SendMessage(WM_CALLUSER,1,NULL);
		  	printf("������ʱƵ���ɹ�\n");
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
			  //����
			  if(CPublic::currentUserID!=int_userid){

				    dlg.SendMessage(WM_CALLUSER,2,NULL);

					char str[16];

					sprintf(str,"%X",int_userid);

					printf("�����û�ID%s��\n",str);
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
			//�л��ɹ�

			CPublic::currentUserID = int_channel_id;//��Ҫ�л�Ƶ����ID

			printf("�л�Ƶ���ɹ���Ϣ\n");

		}else{
			//�л�ʧ��

			printf("�л�Ƶ��ʧ����Ϣ %d\n",int_result);
		}

	}else if(info[7] == 58){

	     printf("��ʱƵ����Ϣ\n");

		 ClientChannelInfo clientChannelInfo;

		 char byte_tempChannelID[4];

		 memcpy(byte_tempChannelID,info+8 ,4);
		 //��ʱ�����ID����Ҫ���ֻ��ͻ��˱��档ÿ���л�Ƶ����ʱ����Ҫ����ID���߷�������

		 int int_tempChannelID = 0;

		 int_tempChannelID = byte_tempChannelID[3] & 0xFF;  

		 int_tempChannelID |= ((byte_tempChannelID[2] << 8) & 0xFF00);  

		 int_tempChannelID |= ((byte_tempChannelID[1] << 16) & 0xFF0000);  

		 int_tempChannelID |= ((byte_tempChannelID[0] << 24) & 0xFF000000);

		 CPublic::switchChannelID = int_tempChannelID;

		 //��ȡ�û����б����ַ�������
		 char byte_username[64];

		  memcpy(byte_username,info+12 ,64);

		  CString str_groupname(byte_username);//Ƶ�������ַ�������

		  //�������� �Ҷϰ�ť
		   dlg.SendMessage(WM_CALLUSER,3,NULL);
	}else if(info[7] == -111){
		   
		if(info[8] == 0){
			//ͨ������ʧ�ܲ����Է��������Խ�
			printf("ͨ������ʧ��\n");
		}else if(info[8] == 1){
			//ͨ������ɹ��ſ��Է��������Խ�
			//printf("ͨ������ɹ�\n");
		}
		
	}

	delete info;

	return 0;
}




void PTTClientControl::OnBnClickedButtonExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
