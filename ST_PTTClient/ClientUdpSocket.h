#pragma once
#include "afxsock.h"
#include "PTTOpus.h"
class ClientUdpSocket : public CSocket
{
public:
	ClientUdpSocket(void);
	~ClientUdpSocket(void);
	SOCKADDR_IN ClientAddr;

    
public:
       virtual void OnReceive(int nErrorCode);
	    virtual void OnSend(int nErrorCode);
	   CPTTOpus* pTTOpus;
	    void setSendData(unsigned char *data,int len);
	 
};

