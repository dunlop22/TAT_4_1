#pragma once
#include "defs.h"


class Scaner
{
private:
	IM t;		//�������� �����
	int uk;		//��������� ������� ������� � �������� ������

public:
	void PutUK(int i);
	int GetUK();

	void PrintError(string, string);
	int FScaner(LEX lex);
	void GetData(string);

	Scaner(string);	//�����������
	~Scaner() {}		//����������
};

