#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "AudioPlay.h"

#include "opus_interface.h"
#include "opus.h"

//#define FRAME_SIZE 160
//#define SAMPLE_RATE 8000
//#define CHANNELS 1
//#define APPLICATION OPUS_APPLICATION_AUDIO
////��������ѹ������������7000 ������������,ѹ����160short ѹ����18short
//#define BITRATE 8000
//
//#define MAX_FRAME_SIZE 6*960
//#define MAX_PACKET_SIZE (3*1276)





class CPTTOpus
{
    public:
	CPTTOpus(void);
	~CPTTOpus(void);
	int Opusopen(int compression);

	//ѹ��
	int Opusencode(char* databuff,int size);

	//����
	int  Opusdecode(char databuff[],int size,char decodebuff[]);



	void Opusclose();

public:

// CAudioPlay*		m_pAudioPlayer;
	short Output[1024];
	unsigned char *stream;
	char *dwParam1;
	short stream_len ;
	short speechType;
	int err;
	int fs;
	int ch;
	int rate;
	int complexity;
	short length_encoded_buffer ;
	int FrameLen;
	char outname[80],outname2[80],outname3[80];
	FILE *fp_output;
	FILE *fp_output2;
	FILE *fp_output3;
	OpusEncInst *EncInst;
	OpusDecInst *DecInst;
};
