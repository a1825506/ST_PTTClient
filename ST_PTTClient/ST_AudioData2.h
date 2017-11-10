#pragma once
class CST_AudioData2
{
public:
	CST_AudioData2(void);
	~CST_AudioData2(void);

	int getSize();

	void setSize(int size);

	char* getRealData();

	void setRealData(char realData[]);

public:

	int size;

	char realData[320];
};

