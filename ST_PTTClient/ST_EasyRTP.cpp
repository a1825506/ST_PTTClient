#include "StdAfx.h"
#include "ST_EasyRTP.h"
using namespace jrtplib;  

CST_EasyRTP::CST_EasyRTP(void)
{

}


CST_EasyRTP::~CST_EasyRTP(void)
{

}

bool CST_EasyRTP::InitRTPSession()
{
	uint16_t portbase = CPublic::LOCAL_RECEVED_STREAM_PORT;    

	uint16_t destport = CPublic::SERVER_DESTPORT;  

	destip    = inet_addr("139.224.68.157");

	if (destip == INADDR_NONE)  
	{  
		printf("Bad IP address specified");

		return false;  
	}  


	destip = ntohl(destip);  

	sessparams.SetOwnTimestampUnit(1.0/8000.0); /*����ʱ�����1/8000��ʾ1���Ӳ���8000�Σ���¼��ʱ��8KHz*/        

	sessparams.SetAcceptOwnPackets(true);  

	transparams.SetPortbase(portbase);  /*����ͨѶ�˿�*/

	status = sess.Create(sessparams,&transparams);   

	if (status < 0)  
	{  
		printf(" Create %s",RTPGetErrorString(status));

		 return false;
	}  

	 

	RTPIPv4Address addr(destip,destport);  

	status = sess.AddDestination(addr); //RTP �Ự�����ݷ��͵�destip������ destport �˿ڣ�

	if (status < 0)  
	{  
		printf(" AddDestination %s",RTPGetErrorString(status));

		return false;
	}  

    return true;
}

void CST_EasyRTP::RTPSend(unsigned char* data,int len)
{

	status=sess.SendPacket((void *)data,len,0,false,8000);

	if (status < 0)  
	{  
		printf("%s",RTPGetErrorString(status));
	}    

	RTPTime::Wait(RTPTime(1,0));
}

void CST_EasyRTP::RTPReceive()
{


       sess.BeginDataAccess();

		// check incoming packets
	//	if (sess.GotoFirstSourceWithData())
		//{
			printf("Begin play/n");
			 while (true)
			{
				//printf("Begin play/n");
				RTPPacket *pack;

				while ((pack = sess.GetNextPacket()) != NULL)
				{
				
					printf(" lengh=%d/n",pack->GetPayloadLength());
					// we don't longer need the packet, s
					sess.DeletePacket(pack);

				}
			} 
			//return 0;

	//	}else{
	//			printf("End play/n");
	//	}

		sess.EndDataAccess();

  } 
