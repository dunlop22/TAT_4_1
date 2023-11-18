#pragma once
#include "defs.h"


class Scaner
{
private:
	IM t;		//Исходный текст
	int uk;		//указатель текущей позиции в исходном тексте

	int line_number;		//строка
	int position;		//позиция

public:
	void PutUK(int i);
	int GetUK();

	void Set_Position(int n);
	void Set_Line_Number(int n);
	void New_Line();
	int Get_Number_Line();
	int Get_Position();

	void PrintError(string, string, char);
	void PrintError(string);
	int FScaner(LEX lex);
	void GetData(string);

	Scaner(string);	//Конструктор
	~Scaner() {}		//Деструктор
};

