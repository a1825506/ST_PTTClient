#include "StdAfx.h"
#include "CPublic.h"
#include <vector>
using namespace std;

int CPublic::currentUserID=0;

CString CPublic::currentUserName;

 int CPublic::currentChannelID=0;

 int CPublic::switchChannelID=0;

 CString CPublic::currentChannelName="default";

vector<ClientUserInfo> CPublic::V_ClientUserInfo;

vector<ClientChannelInfo> CPublic::V_ClientChannelInfo;

char CPublic::LOCAL_IPADDRESS[4];

char CPublic::LOCAL_PORT[4];


ClientUdpSocket* CPublic:: pUdpSocket;

ClientSocket* CPublic:: pSock;

 CPTTOpus*CPublic:: pTTOpus;

 int CPublic::LOCAL_RECEVED_STREAM_PORT=11254;

 int CPublic::SERVER_DESTPORT=0;


CPublic::CPublic(void)
{

}


CPublic::~CPublic(void)
{

}


void CPublic::setClientSocket(ClientSocket* pSock)
{
  CPublic::pSock=pSock;
}

void CPublic::setUdpClientSocket(ClientUdpSocket* pUdpSocket)
{
	CPublic::pUdpSocket=pUdpSocket;
}


void CPublic::setCPTTOpus(CPTTOpus* pTTOpus)
{
	CPublic::pTTOpus=pTTOpus;
}

void CPublic::getCPTTOpus(char* buffer,int len)
{



	if(CPublic::pTTOpus){
	
			 CPublic::pTTOpus->Opusencode(buffer,len);
	}else{
			printf("编码数据失败%d\n",len);

	}




}



/************************************************************************/
/* 发送通知帧                                                                     */
/************************************************************************/
void CPublic::createNotifty()
{
	char order[20];

	char notifty[12] = {0x68,0x00,0x0e,0x68,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00};

	//将用户ID复制到notifty数组中
	char userid[4];

	userid[0]  = CPublic::currentUserID >> 24;     

	userid[1] = CPublic::currentUserID >> 16;

	userid[2] = CPublic::currentUserID >> 8;

	userid[3] = CPublic::currentUserID;

	memcpy(notifty+8, userid,4);

	memcpy(order, notifty,12);

	int ip;

	int ret = get_local_ip(ip);

	if (ret == 0) {

		CPublic::LOCAL_IPADDRESS[0]  = ip >> 24;     

		CPublic::LOCAL_IPADDRESS[1] = ip >> 16;

		CPublic::LOCAL_IPADDRESS[2] = ip >> 8;

		CPublic::LOCAL_IPADDRESS[3] = ip;
	} 

		memcpy(order+12, CPublic::LOCAL_IPADDRESS,4);

		CPublic::LOCAL_PORT[0]  = CPublic::LOCAL_RECEVED_STREAM_PORT >> 24;     

		CPublic::LOCAL_PORT[1] = CPublic::LOCAL_RECEVED_STREAM_PORT >> 16;

		CPublic::LOCAL_PORT[2] = CPublic::LOCAL_RECEVED_STREAM_PORT >> 8;

		CPublic::LOCAL_PORT[3] = CPublic::LOCAL_RECEVED_STREAM_PORT;

		memcpy(order+16, CPublic::LOCAL_PORT,4);

		int sum=0;

		for(int i=4;i<18;i++){

			sum+=order[i];
		}

		order[18]=(byte)sum;

		order[19]=0x16;

		CPublic::pSock->SendMSG(order,20);

	//	System.arraycopy(Config.LOCAL_PORT, 0, order, notifty.length+Config.LOCAL_IPADDRESS.length, Config.LOCAL_PORT.length);

}
/************************************************************************/
/* 发送心跳包                                                                     */
/************************************************************************/
void CPublic::heartBeat()
{
	 char head[10] ={0x68,0x00,0x04,0x68,0x00,0x00,0x00,0x20,0x20,0x16};

	 CPublic::pSock->SendMSG(head,10);
}
//创建临时会话
void CPublic::tempTalkOrder(CString channel_name, CString channel_list)
{
    char head[8]={0x68,0x00,0x00,0x68,0x00,0x00,0x00,0x30};

	char byte_channel_name[64]={0};////编组名为固定64字节，暂时定为谁发起的就叫什么

	memcpy(byte_channel_name, channel_list,channel_list.GetLength());//将帧头部分复制到数据帧数组中

	char channel_type[4] = {0x00,0x00,0x00,0x00};//编组类型只能是临时编组

	char isbans[4] = {0x00,0x00,0x00,0x00};//是否被禁用

	char discrip[128]={0};//128字节的描述部分

	 int group_name_length = channel_name.GetLength();//用户名列表长度

	 char byte_group_name_length[4];

	 byte_group_name_length[0]  = group_name_length >> 24;     

	 byte_group_name_length[1] = group_name_length >> 16;

	 byte_group_name_length[2] = group_name_length >> 8;

	 byte_group_name_length[3] = group_name_length;

	  int datalength = 24+64+128+2+group_name_length;//对照命令格式

	  char *order =new char[datalength];//整个命令帧的长度为用户数据区长度16+group_name.length+channel_name.length+6定义帧长度

	  char data_length[2];//用户数据区长度

	  data_length[0]  = (datalength-6) >> 8;     

	  data_length[1] = (datalength-6) >> 0;

	  memcpy(head+1, data_length,2);

	  memcpy(order, head,8);//将帧头部分复制到数据帧数组中

	   memcpy(order+8, byte_channel_name,64);//将编组名复制到数据帧数组中

	   memcpy(order+72, channel_type,4);//将编组类型复制到数据帧数组中

	   memcpy(order+76, byte_group_name_length,4);//将用户名列表长度复制到数据帧数组中

	   memcpy(order+80, channel_name,group_name_length);//将用户名复制到数据帧数组中

	   memcpy(order+80+group_name_length, channel_type,4);//编组类型复制到数据帧数组中

	   memcpy(order+84+group_name_length, isbans,4);

	   memcpy(order+88+group_name_length, discrip,128);

	   int sum=0;

	   for(int i=4;i<datalength-2;i++){

		   sum+=order[i];

	   }

	   order[datalength-2]=(byte)sum;

	   order[datalength-1]=0x16;

	   CPublic::pSock->SendMSG(order,datalength);
}

//上报当前频道的ID
void CPublic::uploadChannelid()
{
	printf("上报当前频道ID\n");

	ClientChannelInfo clientChannelInfo;

	size_t channel_len = CPublic::V_ClientChannelInfo.size();

	for (size_t i =0; i < channel_len; i ++) {

		clientChannelInfo = CPublic::V_ClientChannelInfo[i];

			CString user_channel = clientChannelInfo.getChannelname();

			if(user_channel==CPublic::currentChannelName){

				CPublic::currentChannelName==user_channel;

				CPublic::currentChannelID = clientChannelInfo.getChannelid();
			}
	}
	 char head[14]={0x68,0x00,0x08,0x68,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x08,0x00,0x16};

	char byte_channelid[4];

	byte_channelid[0]  = currentChannelID >> 24;     

	byte_channelid[1] = currentChannelID >> 16;

	byte_channelid[2] = currentChannelID >> 8;

	byte_channelid[3] = currentChannelID;

	memcpy(head+8, byte_channelid,4);

	int sum=0;

	for(int i=4;i<12;i++){

		sum+=head[i];
	}

	head[12]=(byte)sum;

	CPublic::pSock->SendMSG(head,14);
}

//切换编组命令

void CPublic::sendChannelSwitchOrder()
{
	
	char srcChannelid[4];//当前频道ID

	srcChannelid[0]  = CPublic::currentChannelID >> 24;     

	srcChannelid[1] = CPublic::currentChannelID >> 16;

	srcChannelid[2] = CPublic::currentChannelID >> 8;

	srcChannelid[3] = CPublic::currentChannelID;


	char desChannelid[4];//需要切换的频道ID

	desChannelid[0]  = CPublic::switchChannelID >> 24;     

	desChannelid[1] = CPublic::switchChannelID >> 16;

	desChannelid[2] = CPublic::switchChannelID >> 8;

	desChannelid[3] = CPublic::switchChannelID;

	char userid[4];

	userid[0]  = CPublic::currentUserID >> 24;     

	userid[1] = CPublic::currentUserID >> 16;

	userid[2] = CPublic::currentUserID >> 8;

	userid[3] = CPublic::currentUserID;

	char head[22] = {0x68,0x00,0x10,0x68,0x00,0x00,0x00,0x39,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	memcpy(head+8, srcChannelid,4);

	memcpy(head+12, desChannelid,4);
	
	memcpy(head+16, userid,4);
	
	int sum=0;
	
	for(int i=4;i<20;i++){
		
		sum+=head[i];
	}
	
	head[20]=(byte)sum;

	head[21]=0x16;

	CPublic::pSock->SendMSG(head,22);
}

int CPublic::get_local_ip(int& ip) {
	char hostname[128];
	int ret = gethostname(hostname, sizeof(hostname));
	if (ret == -1){
		return -1;
	}
	struct hostent *hent;
	hent = gethostbyname(hostname);
	if (NULL == hent) {
		return -1;
	}
	//直接取h_addr_list列表中的第一个地址h_addr
	ip = ntohl(((struct in_addr*)hent->h_addr)->s_addr);
	
	return 0;
}
