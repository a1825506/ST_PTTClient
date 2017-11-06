#include "StdAfx.h"
#include "ClientChannelInfo.h"


ClientChannelInfo::ClientChannelInfo(void)
{
}


ClientChannelInfo::~ClientChannelInfo(void)
{
}

void ClientChannelInfo::setChannelname(CString channelname)
{
     this->channelname=channelname;
}

CString ClientChannelInfo::getChannelname()
{
   return channelname;
}

void ClientChannelInfo::setChannelalias(CString channelalias)
{
	this->channelalias=channelalias;
}

CString ClientChannelInfo::getChannelalias()
{
	return channelalias;
}

void ClientChannelInfo::setChannelid(int channelid)
{
	this->channelid=channelid;
}

int ClientChannelInfo::getChannelid()
{
	return channelid;
}

void ClientChannelInfo::setChannelidtype(int channelidtype)
{
	this->channelidtype=channelidtype;
}

int ClientChannelInfo::getChannelidtype()
{
	return channelidtype;
}