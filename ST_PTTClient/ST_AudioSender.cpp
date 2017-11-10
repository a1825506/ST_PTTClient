#include "StdAfx.h"
#include "ST_AudioSender.h"
#include "CPublic.h"

CST_AudioSender::CST_AudioSender(const std::string threadName): CST_Thread(threadName)
{

}


CST_AudioSender::~CST_AudioSender(void)
{

}

void CST_AudioSender::addData(unsigned char* realData , int size)
{
	CST_AudioData* audioData = new CST_AudioData();

	audioData->setDataBuff(realData);

	audioData->setSize(size);

	senderDataList.push_back(audioData);

}

bool CST_AudioSender::Start(bool bSuspended/* = false*/)
{
	return CST_Thread::Start(bSuspended);  
}

void CST_AudioSender::Run()  
{  
	isSendering=true;

	while (isSendering) {

		while (senderDataList.size() > 0) {

			CST_AudioData* encodedData = senderDataList.front();//返回第一个元素

			CPublic::getUdpClientSocket(encodedData->getDataBuff(),encodedData->getSize());

			//编码后发送

			senderDataList.pop_front();//删除第一个元素

		}
	}

}
