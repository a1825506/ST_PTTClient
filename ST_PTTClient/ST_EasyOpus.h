#pragma once
#include "opus_multistream.h"
#include "opus.h"

#define FRAME_SIZE 160
#define SAMPLE_RATE 8000
#define CHANNELS 1
#define APPLICATION OPUS_APPLICATION_AUDIO
//��������ѹ������������7000 ������������,ѹ����160short ѹ����18short
#define BITRATE 8000
class CST_EasyOpus
{
public:
	CST_EasyOpus(void);
	~CST_EasyOpus(void);
	bool Opusopen();
	bool Opusencode(char* src, int offset,unsigned char* encoded, int size);

public:
	OpusEncoder *encoder;
	OpusDecoder *decoder;
	int err;
	int bandwidth ;
	int use_vbr ;
	int cvbr;
	int complexity ;
	int packet_loss_perc;
	
};

