#pragma once
#include "defs.h"
#include "Scaner.h"

enum OBJ_TYPE {
	Empty = 0,
	ObjVar,		//простая переменная
	ObjFunct,	//функция
	ObjClass,	//класс как тип
	ObjObjectCl	//объект класса
};

enum DATA_TYPE { NO_TYPE = 0, TYPE_BOOL, TYPE_DOUBLE, TYPE_OBJ_CL};

struct Node
{
	OBJ_TYPE objType;	//тип объекта
	LEX id;				//идентификатор объекта
	DATA_TYPE dataType;	//тип значения
};

class Tree			//элемент семантической таблицы
{
private:
	Node* node;		//информация об объекте таблицы
	Tree* parent, * left, * right;		//родитель, левый и правый потомки

public:
	static Scaner* scan;
	static Tree* cur;		//текущий элемент дерева

	Tree(Tree* p, Tree* l, Tree* r, Node* data);
	Tree(Scaner* scan);

	//Функции обработки бинарного дерева
	void SetLeft(Node* data);
	void SetRight(Node* data);

	Tree* FindRoot();
	Tree* FindUp(Tree* from, LEX id);
	Tree* FindUp(LEX id);
	Tree* FindUpOneLevel(Tree* from, LEX id);

	void Print();

	//Семантичексие подпрограммы
	void SetCur(Tree* a);			//установить текущий узел дерева
	Tree* GetCur();					//получить ссылку на текущий узел дерева
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//занесение идентификатора a в таблицу с типом t
	Tree* SemNewLevel();			//новый уровень (для составного оператора)
	Tree* SemGetVar(LEX a);		//найти в таблице переменную с именем a и вернуть ссылку на соответсвующий элемент дерева
	Tree* SemGetFunct(LEX a);		//найти в таблице переменную с именем a и вернуть ссылку на соответсвующий элемент дерева
	int DupControl(Tree* addr, LEX a);			//проверка идентификатора a на повторное описание внутри блока

	DATA_TYPE GetType(int lexType);
};

