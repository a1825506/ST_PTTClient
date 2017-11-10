#pragma once
#include "ST_AudioData.h"
#include "ST_Thread.h"
#include <list>   
using namespace std;  
//创建一个list容器的实例LISTINT   
typedef list<CST_AudioData*> LISTRECORD;
class CST_AudioEncoder: public CST_Thread
{
public:
	CST_AudioEncoder(const std::string threadName = "noNamed");

	~CST_AudioEncoder(void);

	void addData(unsigned char* realData,int size);

	bool Start(bool bSuspended/* = false*/);  

	virtual void Run();

public:

	bool isEncoding;

	LISTRECORD recordDataList;
};

