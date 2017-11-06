#pragma once
#include <vector>
#include "ClientUserInfo.h"
#include "ClientChannelInfo.h"
#include "ClientSocket.h"
#include "ClientUdpSocket.h"
using namespace std;
/************************************************************************/
/* 公共类，用来定义全局变量                                                                     */
/************************************************************************/
class CPublic
{

public:

	CPublic(void);

	~CPublic(void);

	static void createNotifty();

	static void heartBeat();

	static void tempTalkOrder(CString channel_name, CString channel_list);

	static void uploadChannelid();

	static void sendChannelSwitchOrder();

	static int get_local_ip(int& ip);

	static void setClientSocket(ClientSocket* pSock);

	static void setUdpClientSocket(ClientUdpSocket* pUdpSocket);

	static void setCPTTOpus(CPTTOpus* pTTOpus);

	static void  getCPTTOpus(char* pData, int nLen);




public: 

	static int currentUserID; //全局变量 保存当前用户的id

	static CString currentUserName;

	static int currentChannelID;

	static CString currentChannelName;

	static int switchChannelID;

	static vector <ClientUserInfo> V_ClientUserInfo;

	static vector<ClientChannelInfo> V_ClientChannelInfo;

	static  char LOCAL_IPADDRESS[4];

	static char LOCAL_PORT[4];

	static int LOCAL_RECEVED_STREAM_PORT;

	static int SERVER_DESTPORT;

	static ClientSocket* pSock;

	static ClientUdpSocket* pUdpSocket;

	static CPTTOpus* pTTOpus;

};

