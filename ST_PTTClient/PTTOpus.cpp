#include "StdAfx.h"
#include "PTTOpus.h"
#include "CPublic.h"
#include <time.h>

#include "opus_interface.h"
CPTTOpus::CPTTOpus(void)
{
	m_pAudioPlayer = new CAudioPlayExp();

	if(m_pAudioPlayer->Start()){

		printf("SDL2.0��ʼ���ɹ�\n");
	}

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
	 sscanf("decodcchar2.pcm", "%s", outname2);

	 if((fp_output2 = fopen(outname2, "wb+")) == NULL) {

		 printf(" opus: Cannot write file %s.\n", outname2);

		 return -1;
	 }
	 sscanf("decodcchar3.pcm", "%s", outname3);

	 if((fp_output3 = fopen(outname3, "wb+")) == NULL) {

		 printf(" opus: Cannot write file %s.\n", outname3);

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

   	printf("����ǰ�ĳ���%d\n",size);

	short dwParam1[80] ={0};// new short[1024];

	char data_buffrt[160]={0};

	memcpy(data_buffrt,databuffer,size);

	memcpy(dwParam1,data_buffrt,size);

	fwrite(databuffer, sizeof(char), size, fp_output);

	//fwrite(databuffer, sizeof(char), size, fp_output);

	stream_len = WebRtcOpus_Encode(EncInst, dwParam1,  size, 320, stream);

	WebRtcOpus_Encode

	//err = WebRtcOpus_Decode(DecInst, stream, stream_len, Output, &speechType);

	//fwrite(Output, sizeof(short), err/2, fp_output2);

	if(stream_len<0){

			printf("����ʧ��%d\n",stream_len);

			return stream_len;

	}else{

		//�����������ټ���RTPͷ���Զ���ͷ��24�ֽ�

			printf("�����ĳ���%d\n",stream_len);
		//CPublic::getUdpClientSocket(stream,stream_len);

	}
		return stream_len;
}


int  CPTTOpus::Opusdecode(char databuffer1[],int size)
{

	printf("���յ�����%d\n",size);

	short  Output[1024]={0};

	 unsigned char* stream_1;

	memcpy(stream,databuffer1,size);

	err = WebRtcOpus_Decode(DecInst, stream, size, Output, &speechType);

	fwrite(Output, sizeof(short), err, fp_output);

	if(err<0)
	{
		printf("�������%s\n",getTime());
	}
	else
	{

		    dwParam1 = new char[err*2];

	    	memcpy(dwParam1,Output,err*2);

			if(m_pAudioPlayer->Write(dwParam1,err*2)){

				printf("���ڲ���%s\n",getTime());

				//memset(dwParam1, 0, sizeof(char)*err*2);

				//delete dwParam1;

			}else{

				printf("���Ŵ���\n");

			}

			//memset(dwParam1, 0, sizeof(char)*err);

		//	delete dwParam1;

	}

	//memset(stream, 0, sizeof(unsigned char)*size);

	return err;
}
