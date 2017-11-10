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

	unsigned char* getDataBuff();

	void setDataBuff(unsigned char* data_Buff);

public:

	int size;

	unsigned char *dataBuff;

	char realData[160];
};

