// ClientSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ST_PTTClient.h"
#include "ClientSocket.h"
#include<vector>

#include <Winsock.h>
using namespace std;
#pragma comment (lib, "ws2_32.lib"); 



// ClientSocket

ClientSocket::ClientSocket()
{
	

}

ClientSocket::~ClientSocket()
{

}


// ClientSocket ��Ա����

void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	char* pData = NULL;
	pData = new char[1024];
	memset(pData, 0, sizeof(char)* 1024);
	int leng = 0;
	int count;

	leng = Receive(pData, 1024, 0);

	 char length[2];

	 vector <int > index;

	// �ڱ༭������ʾ���յ�������
	
	for(int i=0;i<leng;i++){


		if(pData[i]==104&&pData[i+3]==104){

			memcpy(length, pData+(i+1), 2);

			  int   int_length=0;

			  int_length = length[1] & 0xFF;  

			  int_length |= ((length[0] << 8) & 0xFF00);  

			  if(pData[i+4+int_length+1]==22){
				  //�жϽ��յ��������а��������������{68,00,00,68��������16}ȷ������ȥ��֤���ݵ���ȷ��
				index.push_back(i);
			  }
		}
	}


	

	for(int i=0;i<index.size();i++){

		int data_length = 0;

		if(i==index.size()-1){

			byte byte_data_length[2];

			memcpy(byte_data_length,pData+(index[i]+1),2);

			data_length = byte_data_length[1] + byte_data_length[0]*256+6;

		}else{

			data_length = index[i+1]-index[i];
		}

		//char* order;

		//
	
		char* order = NULL;

		order = new char[data_length];

		memcpy(order, pData+(index[i]),data_length);

		

		::SendMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSEDIALOGINFO,data_length,(LPARAM)order);

	//	
	}


	

    
	
		
//	delete pData;
//	pData = NULL;
	CSocket::OnReceive(nErrorCode);
}

BOOL ClientSocket::SendMSG(LPSTR lpBuff, int nlen)
{
	//����Э��ͷ
	if (Send(lpBuff, nlen) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("���ʹ���\n"));

		return FALSE;
	}
	return  TRUE;
}
