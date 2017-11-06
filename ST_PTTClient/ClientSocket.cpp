// ClientSocket.cpp : 实现文件
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


// ClientSocket 成员函数

void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	char* pData = NULL;
	pData = new char[1024];
	memset(pData, 0, sizeof(char)* 1024);
	int leng = 0;
	int count;

	leng = Receive(pData, 1024, 0);

	 char length[2];

	 vector <int > index;

	// 在编辑框中显示接收到的数据
	
	for(int i=0;i<leng;i++){


		if(pData[i]==104&&pData[i+3]==104){

			memcpy(length, pData+(i+1), 2);

			  int   int_length=0;

			  int_length = length[1] & 0xFF;  

			  int_length |= ((length[0] << 8) & 0xFF00);  

			  if(pData[i+4+int_length+1]==22){
				  //判断接收到的数据中包括多少条命令。由{68,00,00,68，，，，16}确定，在去验证数据的正确性
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
	//生成协议头
	if (Send(lpBuff, nlen) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送错误！\n"));

		return FALSE;
	}
	return  TRUE;
}
