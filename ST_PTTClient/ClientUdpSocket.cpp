#include "StdAfx.h"
#include "ClientUdpSocket.h"
#include "CPublic.h"

#include <time.h>
int count=0;


CString getTime1()
{
	time_t timep;
	time (&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
	return tmp;
}

ClientUdpSocket::ClientUdpSocket(void)
{


	pTTOpus = new CPTTOpus();

	sprintf(buf, "AudioDecoder%d",1);  

	audioDecoder = new CST_AudioDecoder(buf);

	if(audioDecoder->Start(TRUE)){

		audioDecoder->Resume();
		audioDecoder->Join(0);
	}else{

		printf("�����߳�����ʧ��");
	}

	int ret=pTTOpus->Opusopen(8);

	if(ret<0){

		printf("OPUS��ʼ��ʧ��\n");

	}else{

		CPublic::setCPTTOpus(pTTOpus);

		printf("OPUS��ʼ���ɹ�\n");

	}
	ClientAddr.sin_family=AF_INET;
	ClientAddr.sin_addr.S_un.S_addr=inet_addr(CPublic::serverIP);
	ClientAddr.sin_port=htons(CPublic::SERVER_DESTPORT);

}


ClientUdpSocket::~ClientUdpSocket(void)
{

}

void ClientUdpSocket::OnReceive(int nErrorCode)
{
    	char recBuf[1024]={0};

	    int len=sizeof(SOCKADDR_IN);

	     int recelen=ReceiveFrom(recBuf,1024,(SOCKADDR*)&ClientAddr,&len,0);

		 audioDecoder->addData(recBuf,recelen);

		//printf("���յ�����%s\n",getTime1());

	//	 char realdata[160]={0};

	//	memcpy(realdata,recBuf+12,recelen-12);

	//	  pTTOpus->Opusdecode(realdata,160);
		  
	     CSocket::OnReceive(nErrorCode);

		
}

 void ClientUdpSocket::OnSend(int nErrorCode) 
{
		    // AfxMessageBox(buff);
		     //SendTo(buff,len,CPublic::SERVER_DESTPORT,"139.224.68.157",0);
	char datas[36] = {00,00,00, 00, 00 ,00 ,00 ,00 ,00, 00, 00 ,00 ,00, 00 ,00 ,00 ,00 ,00 ,00 ,00, 00, 00 ,00, 00, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	int lenw=sizeof(SOCKADDR_IN);

	int err = SendTo(datas,36,(SOCKADDR *)&ClientAddr,sizeof(ClientAddr),0);

	// int err= SendTo(datas,36,20087,"127.0.0.1",0);

		CSocket::OnSend(nErrorCode);
 }

 void ClientUdpSocket::setSendData(unsigned char *data,int len)
 {
	 //char datas[36] = {00,00,00, 00, 00 ,00 ,00 ,00 ,00, 00, 00 ,00 ,00, 00 ,00 ,00 ,00 ,00 ,00 ,00, 00, 00 ,00, 00, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  
	 //int lenw=sizeof(SOCKADDR_IN);
	 //int err= SendTo(data,len,(SOCKADDR *)&ClientAddr,lenw,0);
	 // printf("���ͽ�� %d \n",lenw);
	// OnSend(1);
	 int lenw=sizeof(SOCKADDR_IN);

	 int err = SendTo(data,len,(SOCKADDR *)&ClientAddr,sizeof(ClientAddr),0);

	 // int err= SendTo(datas,36,20087,"127.0.0.1",0);
	 if(err<0){
	    printf("����ʧ�� %d \n",err);
	 }

	 

 }





