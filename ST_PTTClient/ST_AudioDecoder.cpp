#include "StdAfx.h"
#include "ST_AudioDecoder.h"
#include "ST_AudioData.h"
#include "CPublic.h"

CST_AudioDecoder::CST_AudioDecoder(const std::string threadName): CST_Thread(threadName)  
{  

} 


CST_AudioDecoder::~CST_AudioDecoder(void)
{
}

void CST_AudioDecoder::addData(char realData[] , int size)
{
	CST_AudioData* audioData = new CST_AudioData();

	audioData->setSize(size);

	char realdata[160]={0};

	memcpy(realdata,realData+12,size-12);

	audioData->setRealData(realdata);

	dataList.push_back(audioData);

}

bool CST_AudioDecoder::Start(bool bSuspended/* = false*/)  
{  
	// todo: 此处可添加一些初始化代码  

	sscanf("decodcchar.pcm", "%s", outname);

	if((fp_output = fopen(outname, "wb+")) == NULL) {

		printf(" opus: Cannot write file %s.\n", outname);

		return -1;
	}
	
	return CST_Thread::Start(bSuspended);  
}

void CST_AudioDecoder::Run()  
{  
	char buf[20]={0};

	sprintf(buf, "AudioPlay%d",1);  

	isDecoding=true;

	audioPlay = new CAudioPlayExp(buf);

	if(audioPlay->Start(true)){

		audioPlay->initAudioTrack();

		audioPlay->Resume();

		audioPlay->Join(0);

	}

	while (isDecoding) {

		while (dataList.size() > 0) {

			 CST_AudioData* encodedData = dataList.front();//返回第一个元素

			 char * decodebuff;

			 int err=CPublic::getCPTTOpusDecode(encodedData->getRealData(),encodedData->getSize(),decodebuff);

			// audioPlay->addData(decodebuff,err);

			 printf("解码后的长度%d\n",err);

			

			 dataList.pop_front();//删除第一个元素

			 delete encodedData;
		}
	}

}