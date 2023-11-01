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
	void Q();		//Выражение
	void R();		//Равенство
	void U();		//И
	void W();		//Сравнение
	void X();		//Слагаемое
	void Y();		//Множитель
	void Z();		//Со знаком
	void P();		//Вызов функции
	void V();		//Равенство
	int Look_Forward(int n);

public:
	dias(Scaner* s) { scan = s; root = new	Tree(scan); }
	~dias() {}
	void S();		//Программа
};

