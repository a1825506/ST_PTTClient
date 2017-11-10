#pragma once
#include "ST_AudioData.h"
#include "ST_Thread.h"
#include <list>   
using namespace std;  
//����һ��list������ʵ��LISTINT   
typedef list<CST_AudioData*> LISTSENDER;
class CST_AudioSender: public CST_Thread
{
public:
	CST_AudioSender(const std::string threadName = "noNamed");

	~CST_AudioSender(void);

	void addData(unsigned char* realData,int size);

	bool Start(bool bSuspended/* = false*/);  

	virtual void Run();

public:

	bool isSendering;

	LISTSENDER senderDataList;
};

