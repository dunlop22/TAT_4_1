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

union DATA_VALUE 
{
	bool DataAsBool;
	double DataAsDouble;
};

struct DataS 
{
	DATA_TYPE dataType;	//тип значения
	DATA_VALUE dataValue; //значение

	LEX className;		//имя класса для объектов классов
};

struct Node
{
	OBJ_TYPE objType;	//тип объекта
	LEX id;				//идентификатор объекта

	DataS data;
};

class Tree			//элемент семантической таблицы
{
	LEX DT_Name[3] = { "неопр.", "bool", "double" };

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

	void SetRightT(Tree* node);
	Tree* GetRight();

	Tree* FindRoot();
	Tree* FindUp(Tree* from, LEX id);
	Tree* FindUp(LEX id);
	Tree* FindUpOneLevel(Tree* from, LEX id);
	Tree* FindRightLeft(Tree* from, LEX id);
	Tree* FindRightLeftVar(LEX id);
	Tree* FindRightLeftFunct(LEX id);

	void CleanTree();
	void CleanChild();

	void Print();

	//Семантичексие подпрограммы
	void SetCur(Tree* a);			//установить текущий узел дерева
	Tree* GetCur();					//получить ссылку на текущий узел дерева
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//занесение идентификатора a в таблицу с типом t
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t, LEX className);		//занесение идентификатора a в таблицу с типом t
	Tree* SemNewLevel();			//новый уровень (для составного оператора)
	Tree* SemGetVar(LEX a);		//найти в таблице переменную с именем a и вернуть ссылку на соответсвующий элемент дерева
	Tree* SemGetFunct(LEX a);		//найти в таблице функцию с именем a и вернуть ссылку на соответсвующий элемент дерева
	Tree* SemGetClass(LEX a);		//найти в таблице класс с именем a и вернуть ссылку на соответсвующий элемент дерева
	int DupControl(Tree* addr, LEX a);			//проверка идентификатора a на повторное описание внутри блока

	DATA_TYPE GetTypebyLex(int lexType);
	DATA_TYPE GetType();

	void GetClassName(LEX name);
	DATA_VALUE* GetValue();

	Tree* GetCurrentFunct();

	DataS TypeCastingAssign(DATA_TYPE firstType, DataS second, LEX firstTypeName, LEX secondTypeName);	//приведение типов при присваивании
	DATA_TYPE TypeCasting(DATA_TYPE firstType, DATA_TYPE secondType, LEX operation);

	void CheckTypeBool(DATA_TYPE type);
};

