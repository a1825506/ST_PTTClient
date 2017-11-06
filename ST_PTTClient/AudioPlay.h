#pragma once
extern "C"
{
#include "sdl/SDL.h"
};

class CAudioPlay
{


public:
	BOOL Start();
	void Stop();
	BOOL Write(char *pData, int nLen);
	int pcm_buffer_size;
	char *pcm_buffer;
	FILE *fp_output;
	char outname[80];
	int data_count;
};