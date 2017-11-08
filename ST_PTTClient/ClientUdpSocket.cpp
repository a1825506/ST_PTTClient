#include "StdAfx.h"
#include "ClientUdpSocket.h"
#include "CPublic.h"
int count=0;




ClientUdpSocket::ClientUdpSocket(void)
{

	pTTOpus = new CPTTOpus();

	int ret=pTTOpus->Opusopen(8);

	if(ret<0){

		printf("OPUS初始化失败\n");

	}else{

		CPublic::setCPTTOpus(pTTOpus);

		printf("OPUS初始化成功\n");

	}
	ClientAddr.sin_family=AF_INET;
	ClientAddr.sin_addr.S_un.S_addr=inet_addr("139.224.68.157");
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


		 char realdata[160]={0};

		memcpy(realdata,recBuf+24,recelen-24);

		  pTTOpus->Opusdecode(realdata,160);
		  
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


	    printf("发送结果 %d \n",err);

		CSocket::OnSend(nErrorCode);
 }

 void ClientUdpSocket::setSendData(unsigned char *data,int len)
 {
	 //char datas[36] = {00,00,00, 00, 00 ,00 ,00 ,00 ,00, 00, 00 ,00 ,00, 00 ,00 ,00 ,00 ,00 ,00 ,00, 00, 00 ,00, 00, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  
	 //int lenw=sizeof(SOCKADDR_IN);
	 //int err= SendTo(data,len,(SOCKADDR *)&ClientAddr,lenw,0);
	 // printf("发送结果 %d \n",lenw);
	// OnSend(1);
	 int lenw=sizeof(SOCKADDR_IN);

	 int err = SendTo(data,len,(SOCKADDR *)&ClientAddr,sizeof(ClientAddr),0);

	 // int err= SendTo(datas,36,20087,"127.0.0.1",0);


	 printf("发送结果 %d \n",err);

 }





