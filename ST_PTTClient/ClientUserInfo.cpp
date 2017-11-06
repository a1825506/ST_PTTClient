#include "StdAfx.h"
#include "ClientUserInfo.h"


ClientUserInfo::ClientUserInfo(void)
{

}


ClientUserInfo::~ClientUserInfo(void)
{

}

void ClientUserInfo::setUsername(CString username)
{
   this->username = username;
}

CString ClientUserInfo::getUsername()
{
  return this->username;
}

void ClientUserInfo::setUsertype(CString usertype)
{
	this->usertype = usertype;
}

CString ClientUserInfo::getUsertype()
{
	return this->usertype;
}


void ClientUserInfo::setIsforbidden(int isforbidden)
{
	this->isforbidden = isforbidden;
}

int ClientUserInfo::getIsforbidden()
{
	return this->isforbidden;
}

void ClientUserInfo::setGroup(CString group)
{
	this->group = group;
}

CString ClientUserInfo::getGroup()
{
	return this->group;
}

void ClientUserInfo::setOnline(int online)
{
	this->isOnline = online;
}

int ClientUserInfo::getOnline()
{
	return this->isOnline;
}


void ClientUserInfo::setUserid(int userid)
{
	this->userid = userid;
}

int ClientUserInfo::getUserid()
{
	return this->userid;
}

