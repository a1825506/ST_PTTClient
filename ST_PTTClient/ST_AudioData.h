#pragma once
class CST_AudioData
{
public:
	CST_AudioData(void);
	~CST_AudioData(void);

	int getSize();

	void setSize(int size);

	char* getRealData();

	void setRealData(char realData[]);

public:

	int size;

	char realData[1024];
};

