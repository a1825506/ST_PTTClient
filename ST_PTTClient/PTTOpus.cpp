#include "StdAfx.h"
#include "PTTOpus.h"
#include "CPublic.h"
#include <time.h>

#include "opus_interface.h"
CPTTOpus::CPTTOpus(void)
{


}
CString getTime()
{
	time_t timep;
	time (&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
	return tmp;
}

CPTTOpus::~CPTTOpus(void)
{

}


int CPTTOpus::Opusopen(int compression)
{
	short stream_len = 0;

	int err;

	int fs=8000;

	int ch=1;

	int rate=8000;

	int complexity=8;

	short length_encoded_buffer = 160;

	stream = (unsigned char *)malloc(sizeof(char)*1024);

	memset(stream, 0, sizeof(unsigned char)*1024);

	FrameLen = fs/50;

	 sscanf("decodcchar.pcm", "%s", outname);

	 if((fp_output = fopen(outname, "wb+")) == NULL) {

		 printf(" opus: Cannot write file %s.\n", outname);

		 return -1;
	 }


	WebRtcOpus_EncoderCreate(&EncInst, ch, fs);

	WebRtcOpus_SetBitRate(EncInst, rate);

	WebRtcOpus_SetPacketLossRate(EncInst, 0);

	WebRtcOpus_SetMaxPlaybackRate(EncInst, fs);

	WebRtcOpus_SetComplexity(EncInst, complexity);

	WebRtcOpus_DecoderCreate(&DecInst, ch, fs);

	WebRtcOpus_DecoderInit(DecInst);

	return 1;
}



int  CPTTOpus::Opusencode(char* databuffer,int size)
{

   //	printf("编码前的长度%d\n",size);

	short dwParam1[80] ={0};// new short[1024];

	char data_buffrt[160]={0};

	memcpy(data_buffrt,databuffer,size);

	memcpy(dwParam1,data_buffrt,size);

	//fwrite(databuffer, sizeof(char), size, fp_output);

	stream_len = WebRtcOpus_Encode(EncInst, dwParam1,  size/2, 160, stream);

	if(stream_len<0){

			printf("编码失败%d\n",stream_len);

			return stream_len;

	}else{

		//编码后的数据再加上RTP头和自定义头共24字节

			//printf("解码后的长度%d\n",stream_len);
		CPublic::getUdpClientSocket(stream,stream_len);

	}
		return stream_len;
}


int  CPTTOpus::Opusdecode(char databuffer1[],int size,char *dwParam1)
{
	short  Output[1024]={0};

	 unsigned char* stream_1;

	memcpy(stream,databuffer1,size);

	err = WebRtcOpus_Decode(DecInst, stream, size, Output, &speechType);

	
		fwrite(Output, sizeof(short), err/2, fp_output);
	if(err<0)
	{
		printf("解码错误%s\n",getTime());
	}
	else
	{

		    dwParam1 = new char[err*2];

	    	memcpy(dwParam1,Output,err*2);

		


	return err*2;
	}
}
