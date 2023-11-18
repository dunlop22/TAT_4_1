#pragma once
#include "Scaner.h"
#include "Semantika.h"

class dias
{
private:
	Scaner* scan;
	Tree* root;

	void D();		//Описание данных
	void B();		//Имя
	void I();		//Описание класса
	void F();		//Функция
	void K();		//Составной оператор
	void M();		//Оператор
	void H();		//return
	void N();		//if
	void O();		//Присваивание
	void Q(DATA_TYPE* resType, LEX* resTypeName);		//Выражение
	void R(DATA_TYPE* resType, LEX* resTypeName;		//Равенство
	void U(DATA_TYPE* resType, LEX* resTypeName);		//И
	void W(DATA_TYPE* resType, LEX* resTypeName);		//Сравнение
	void X(DATA_TYPE* resType, LEX* resTypeName);		//Слагаемое
	void Y(DATA_TYPE* resType, LEX* resTypeName);		//Множитель
	void Z(DATA_TYPE* resType, LEX* resTypeName);		//Со знаком
	void P(DATA_TYPE* resType, LEX* resTypeName);		//Вызов функции
	void V(DATA_TYPE* resType, LEX* resTypeName);		//Равенство
	int Look_Forward(int n);

public:
	dias(Scaner* s) { scan = s; root = new	Tree(scan); }
	~dias() {}
	void S();		//Программа

	void PrintTree();
};

