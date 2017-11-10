#include "StdAfx.h"
#include "ST_AudioData2.h"


CST_AudioData2::CST_AudioData2(void)
{
}


CST_AudioData2::~CST_AudioData2(void)
{
}

int CST_AudioData2::getSize()
{
	return size;
}

void CST_AudioData2::setSize(int size)
{
	this->size = size;
}

char* CST_AudioData2::getRealData()
{
	return realData;
}

void CST_AudioData2::setRealData(char real_Data[])
{
	char realData[320]={0};
	memcpy(this->realData,real_Data,320);
}
