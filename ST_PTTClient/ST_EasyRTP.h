#pragma once
#include <jrtplib/rtpsession.h>  
#include <jrtplib/rtpudpv4transmitter.h>  
#include <jrtplib/rtpipv4address.h>  
#include <jrtplib/rtpsessionparams.h>  
#include <jrtplib/rtperrors.h>  
#include <jrtplib/rtplibraryversion.h>  
#include <jrtplib/rtppacket.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <iostream>  
#include <string> 
#include"CPublic.h"
using namespace jrtplib;  

class CST_EasyRTP
{
public:
	CST_EasyRTP(void);
	~CST_EasyRTP(void);

public:
	RTPSession sess;  
	uint16_t portbase ;    
	uint16_t destport ;  
	uint32_t destip;     
	RTPUDPv4TransmissionParams transparams;  
	RTPSessionParams sessparams; 
	int status; 


public:

	bool  InitRTPSession();

	void RTPSend(unsigned char* data,int len);

	void RTPReceive();

	bool done;

};

