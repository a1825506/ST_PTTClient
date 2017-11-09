#pragma once
#include "ST_Thread.h"
#include "ST_AudioData.h"
#include "AudioPlayExp.h"
#include <list>   
using namespace std;  
//创建一个list容器的实例LISTINT   
typedef list<CST_AudioData*> LISTINT;   
class CST_AudioDecoder : public CST_Thread
{
public:
	CST_AudioDecoder(const std::string threadName = "noNamed");  

	~CST_AudioDecoder(void);

	void addData(char realData[],int size);

	bool Start(bool bSuspended/* = false*/);  

	virtual void Run();

public:

	bool isDecoding;

	 LISTINT dataList; 

	 CAudioPlayExp* audioPlay;

	 char outname[80];

	 FILE *fp_output;
};

