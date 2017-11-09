#include "StdAfx.h"
#include "ST_EasyOpus.h"


CST_EasyOpus::CST_EasyOpus(void)
{

	 bandwidth = OPUS_AUTO;
	 use_vbr = 0;
	 cvbr=1;
	 complexity = 4;
	 packet_loss_perc=0;

}


CST_EasyOpus::~CST_EasyOpus(void)
{

}

bool CST_EasyOpus::Opusopen()
{
	encoder = opus_encoder_create(SAMPLE_RATE, CHANNELS, APPLICATION, &err);

	if (err<0)
	{
		printf("OPUS初始化失败");
		return false;
	}
	err = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(BITRATE));

	if (err<0)
	{
		printf("OPUS初始化失败");
		return false;
	}
	 opus_encoder_ctl(encoder, OPUS_SET_BANDWIDTH(bandwidth));
	 opus_encoder_ctl(encoder, OPUS_SET_VBR(use_vbr));
	 opus_encoder_ctl(encoder, OPUS_SET_VBR_CONSTRAINT(cvbr));
	 opus_encoder_ctl(encoder, OPUS_SET_COMPLEXITY(complexity));
	 opus_encoder_ctl(encoder,   OPUS_SET_PACKET_LOSS_PERC(packet_loss_perc));
	 decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &err);
	 if (err<0)
	 {
		 return false;
	 }
	return true;
}

bool CST_EasyOpus::Opusencode(char* src, int offset, unsigned char* encoded, int size)
{
	if(encoder == NULL){

		 return false;
	}

	///const opus_int16* pcm=&src;



	//opus_encode(encoder,pcm , size, encoded, 480);


	//op

	return true;
}
