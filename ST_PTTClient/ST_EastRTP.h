#pragma once
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#ifndef WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif // WIN32
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace jrtplib;
class CST_EastRTP
{
public:
	CST_EastRTP(void);
	~CST_EastRTP(void);

	RTPSession sess;

	uint16_t portbase,destport;

	uint32_t destip;

	CString ipstr;

	int status;

	int createRTPSession();
};

