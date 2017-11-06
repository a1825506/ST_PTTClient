#pragma once
class ClientUserInfo
{
public:
	ClientUserInfo(void);
	~ClientUserInfo(void);

	void setUsername(CString username);
	CString getUsername();

	void setUsertype(CString usertype);
	CString getUsertype();

	void setIsforbidden(int isforbidden);
	int getIsforbidden();

	void setGroup(CString group);
	CString getGroup();

	void setOnline(int online);
	int getOnline();


	void setUserid(int userid);
	int getUserid();

private:
	 CString username;
	 CString useralias;
	 int userid;
	 CString usertype;
	 int isforbidden;
	 CString group;
	 int isOnline;
};

