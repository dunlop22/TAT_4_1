#pragma once
#include "Scaner.h"
#include "Semantika.h"

class dias
{
	LEX Operation_Name[14] = { "|", "<", "<=", ">", ">=", "==",
						"!=", "+", "-", "^", "&", "*", "/", "%" };


private:
	Scaner* scan;
	Tree* root;

	void D();		//Описание данных
	void B(DataS* res);		//Имя
	void I();		//Описание класса
	void F();		//Функция
	void K();		//Составной оператор
	void M();		//Оператор
	void H();		//return
	void N();		//if
	void O();		//Присваивание
	void Q(DataS* res);		//Выражение
	void R(DataS* res);		//Равенство
	void U(DataS* res);		//И
	void W(DataS* res);		//Сравнение
	void X(DataS* res);		//Слагаемое
	void Y(DataS* res);		//Множитель
	void Z(DataS* res);		//Со знаком
	void P(DataS* res);		//Вызов функции
	void V(DataS* res);		//Равенство
	int Look_Forward(int n);

public:
	dias(Scaner* s) { scan = s; root = new	Tree(scan); }
	~dias() {}
	void S();		//Программа

	void PrintTree();
};

