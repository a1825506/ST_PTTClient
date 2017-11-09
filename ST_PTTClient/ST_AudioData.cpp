#include "StdAfx.h"
#include "ST_AudioData.h"


CST_AudioData::CST_AudioData(void)
{
}


CST_AudioData::~CST_AudioData(void)
{
}

int CST_AudioData::getSize()
{
	return size;
}

void CST_AudioData::setSize(int size)
{
	this->size = size;
}

char* CST_AudioData::getRealData()
{
	return realData;
}

void CST_AudioData::setRealData(char real_Data[])
{
	memcpy(this->realData,real_Data,sizeof(real_Data));
}
