#include "StdAfx.h"
#include "ST_AudioEncoder.h"
#include "CPublic.h"

CST_AudioEncoder::CST_AudioEncoder(const std::string threadName): CST_Thread(threadName)
{
}


CST_AudioEncoder::~CST_AudioEncoder(void)
{
}

void CST_AudioEncoder::addData(unsigned char* realData , int size)
{
	CST_AudioData* audioData = new CST_AudioData();

	audioData->setDataBuff(realData);

	audioData->setSize(size);

	recordDataList.push_back(audioData);

}

bool CST_AudioEncoder::Start(bool bSuspended/* = false*/)
{
      return CST_Thread::Start(bSuspended);  
}

void CST_AudioEncoder::Run()  
{  
	isEncoding=true;

	while (isEncoding) {

		while (recordDataList.size() > 0) {

			CST_AudioData* encodedData = recordDataList.front();//���ص�һ��Ԫ��

		     CPublic::getCPTTOpus(encodedData->getRealData(),encodedData->getSize());

			 //�������

			recordDataList.pop_front();//ɾ����һ��Ԫ��

		}
	}

}
