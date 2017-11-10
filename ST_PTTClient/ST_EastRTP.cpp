#include "StdAfx.h"
#include "ST_EastRTP.h"


CST_EastRTP::CST_EastRTP(void)
{
}


CST_EastRTP::~CST_EastRTP(void)
{
}

int CST_EastRTP:: createRTPSession()
{
	ipstr="192.168.8.121";

   destip = inet_addr(ipstr);

   destip = ntohl(destip);

   return status;
}
