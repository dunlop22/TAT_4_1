#pragma once
#include "defs.h"


class Scaner
{
private:
	IM t;		//Исходный текст
	int uk;		//указатель текущей позиции в исходном тексте

public:
	void PutUK(int i);
	int GetUK();

	void PrintError(string, string, char);
	int FScaner(LEX lex);
	void GetData(string);

	Scaner(string);	//Конструктор
	~Scaner() {}		//Деструктор
};

