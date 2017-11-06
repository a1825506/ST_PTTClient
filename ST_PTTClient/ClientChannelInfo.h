#pragma once
/************************************************************************/
/* 保存频道信息的类                                                                     */
/************************************************************************/
class ClientChannelInfo
{
public:
	ClientChannelInfo(void);
	~ClientChannelInfo(void);

	void setChannelname(CString channelname);
	CString getChannelname();

	void setChannelalias(CString channelalias);
	CString getChannelalias();

	void setChannelid(int channelid);
	int getChannelid();

	void setChannelidtype(int channelidtype);
	int getChannelidtype();

public:
	 CString channelname;
	 CString channelalias;
	 int channelid;
	 int channelidtype;
};

