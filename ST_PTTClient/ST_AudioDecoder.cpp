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

	char real_data[160]={0};

	memcpy(real_data,realData+12,size-12);


	audioData->setRealData(real_data);

	audioData->setSize(size-12);

	//CPublic::getCPTTOpusDecode(audioData->getRealData(),audioData->getSize());

	dataList.push_back(audioData);

	//delete audioData;

	

}

bool CST_AudioDecoder::Start(bool bSuspended/* = false*/)  
{  
	// todo: �˴������һЩ��ʼ������  

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

		 printf("�����������ɹ�\n");

	}else
		printf("����������ʧ��\n");

	while (isDecoding) {

		while (dataList.size() > 0) {

			 CST_AudioData* encodedData = dataList.front();//���ص�һ��Ԫ��
            
			 char *dwParam=CPublic::getCPTTOpusDecode(encodedData->getRealData(),encodedData->getSize());

		     audioPlay->addData(dwParam,320);

			 dataList.pop_front();//ɾ����һ��Ԫ��

			 
		}
	}

}