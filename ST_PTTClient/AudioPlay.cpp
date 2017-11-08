#include "StdAfx.h"
#include "AudioPlay.h"
#include <time.h>
extern "C"
{
#include "sdl/SDL.h"
};

static  Uint8  *audio_chunk; 
static  Uint32  audio_len; 
static  Uint8  *audio_pos; 




void fill_audio(void *udata,Uint8 *stream,int len)
{
	SDL_memset(stream, 0, len);
	if(audio_len==0)		/*  Only  play  if  we  have  data  left  */ 
		return; 
	len=(len>audio_len?audio_len:len);	/*  Mix  as  much  data  as  possible  */ 

	SDL_MixAudio(stream,audio_pos,len,SDL_MIX_MAXVOLUME);
	audio_pos += len; 
	audio_len -= len; 
}



BOOL CAudioPlay::Start()  
{  
	 sscanf("decodc1.pcm", "%s", outname);


	 if((fp_output = fopen(outname, "wb+")) == NULL) {

		 printf(" opus: Cannot write file %s.\n", outname);

		 exit(1);
	 }

	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf("Could not initialize SDL - %s\n", SDL_GetError());  
		return FALSE;  
	}  
	//SDL_AudioSpec    
	SDL_AudioSpec wanted_spec;  
	wanted_spec.freq = 8000;
	wanted_spec.format = AUDIO_S16SYS;  
	wanted_spec.channels = 1;  
	wanted_spec.silence = 0;  
	wanted_spec.samples = 640;  
	wanted_spec.callback = fill_audio;  

	if (SDL_OpenAudio(&wanted_spec, NULL) < 0){  
		printf("can't open audio.\n");  
		return FALSE;  
	}  

	// pcm_buffer_size=160;
	// pcm_buffer=(char *)malloc(pcm_buffer_size);
	 //data_count=0;
	return TRUE;  
}  



BOOL CAudioPlay::Write(char *pData, int nLen)
{

	while(true){
		audio_chunk =  (Uint8 *) pData;  
		//Audio buffer length    
		audio_len = nLen;  
		audio_pos = audio_chunk;  
		////Play    
		SDL_PauseAudio(0);  
		while(audio_len>0)//Wait until finish
		{
			printf("≤•∑≈...“Ù∆µ%d\n",nLen);
			fwrite(pData, sizeof(char), nLen, fp_output);
			//SDL_Delay(1); 
		}
		//memset(pcm_buffer, 0,nLen);
	}
	
	//memcpy(pcm_buffer,pData,nLen);
	

	return TRUE;
}

