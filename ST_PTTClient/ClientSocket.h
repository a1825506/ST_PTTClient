#pragma once

// ClientSocket ����Ŀ��

class ClientSocket : public CSocket
{
public:
	ClientSocket();

	virtual ~ClientSocket();

	virtual void OnReceive(int nErrorCode);      

	

	// ��д���պ�����ͨ����������
	BOOL SendMSG(LPSTR lpBuff, int nlen);        

	// ���ͺ��������ڷ������ݸ�������


};


