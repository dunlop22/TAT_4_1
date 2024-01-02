#include "Semantika.h"

Tree* Tree::cur = (Tree*)NULL;
Scaner* Tree::scan = (Scaner*)NULL;

Tree::Tree(Scaner* scan)
{
	this->scan = scan;

	node = new Node();
	node->objType = Empty;
	node->data.dataType = NO_TYPE;

	parent = NULL;
	left = NULL;
	right = NULL;


	cur = this;
}

Tree::Tree(Node* data, Tree* parent)
{
	node = new Node();
	this->parent = parent;

	memcpy(node, data, sizeof(Node));
}

Tree::Tree(Tree* p, Tree* l, Tree* r, Node* data)
{
	node = new Node();

	parent = p;
	left = l;
	right = r;

	memcpy(node, data, sizeof(Node));
}

void Tree::SetLeft(Node* data)
{
	left = new Tree(this, NULL, NULL, data);
}

void Tree::SetRight(Node* data)
{
	right = new Tree(this, NULL, NULL, data);
}

void Tree::SetRightT(Tree* node)
{
	right = node;
}

Tree* Tree::GetRight()
{
	return right;
}

Tree* Tree::FindUp(Tree* from, LEX id)
{
	Tree* i = from;

	while ((i != NULL) && memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) != 0)
		i = i->parent;

	return i;
}

Tree* Tree::FindRoot()
{
	Tree* i = cur;

	while ((i != NULL) && (i->parent != NULL))
		i = i->parent;

	return i;
}

Tree* Tree::FindUp(LEX id)
{
	return FindUp(this, id);
}

Tree* Tree::FindUpOneLevel(Tree* from, LEX id)
{
	Tree* i = from;		//текущая вершина поиска

	while ((i != NULL) && (i->parent == NULL || i->parent->right != i))
	{
		if (memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) == 0)
			return i;	//найден совпадающий идентификатор

		i = i->parent;
	}

	return NULL;
}

Tree* Tree::FindRightLeft(Tree* from, LEX id)
{
	Tree* i = from->right;
	while ((i != NULL) && (memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) != 0))
		i = i->left;
	return i;
}

Tree* Tree::FindRightLeftVar(LEX id)
{
	Tree* v = FindRightLeft(this, id);

	if (v == NULL)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Отсутствует описание идентификатора");
	}

	if (v->node->objType == ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Неверное использование имени функции");
	}

	if (v->node->objType == ObjClass)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Неверное использование имени класса");
	}

	return v;
}

Tree* Tree::FindRightLeftFunct(LEX id)
{
	Tree* v = FindRightLeft(this, id);

	if (v == NULL)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Отсутствует описание метода");
	}

	if (v->node->objType != ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Идентификатор не является именем метода");
	}

	return v;
}

void Tree::CleanTree()
{
	if (left != NULL)
	{
		left->CleanTree();
		delete left;
		left = NULL;
	}

	CleanChild();
}

void Tree::CleanChild()
{
	if (right != NULL)
	{
		right->CleanTree();
		delete right;
		right = NULL;
	}
}

void Tree::Print()
{
	if (node->objType != Empty)
		printf("Вершина с данными %s ------>", node->id);
	else
		printf("Вершина ПУСТАЯ ------>");

	if (left != NULL)
	{
		if (left->node->objType != Empty)
			printf("      слева данные %s", left->node->id);
		else
			printf("      слева ПУСТАЯ вершина");
	}


	if (right != NULL)
	{
		if (right->node->objType != Empty)
			printf("      справа данные %s", right->node->id);
		else
			printf("      справа ПУСТАЯ вершина");
	}

	printf("\n");

	if (left != NULL)
		left->Print();

	if (right != NULL)
	{
		printf("\n\nРОДИТЕЛЬ: ");
		if (node->objType != Empty)
			printf("Вершина %s\n", node->id);
		else
			printf("Вершина ПУСТАЯ\n");
		right->Print();
	}
}

void Tree::SetCur(Tree* a)
{
	cur = a;
}

Tree* Tree::GetCur()
{
	return cur;
}

Tree* Tree::SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t)
{
	if (DupControl(cur, a))
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Повторное описание идентификатора");
	}

	Tree* v;
	Node n;

	memcpy(n.id, a, strlen(a) + 1);
	n.objType = ot;
	n.data.dataType = t;
	memcpy(n.data.className, "", strlen("") + 1);

	if (this->node->objType == Empty && this->parent == NULL && this->left == NULL && this->right == NULL)
		memcpy(node, &n, sizeof(Node));
	else
	{
		cur->SetLeft(&n);
		cur = cur->left;
	}

	if (ot == ObjFunct || ot == ObjClass)
	{
		v = cur;
		memcpy(&n.id, &"", 2);
		n.objType = Empty;

		cur->SetRight(&n);
		cur = cur->right;
		return v;
	}

	return cur;
}

Tree* Tree::SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t, LEX className)
{
	if (DupControl(cur, a))
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Повторное описание идентификатора");
	}

	Tree* v;
	Node n;


	memcpy(n.id, a, strlen(a) + 1);
	n.objType = ot;
	n.data.dataType = t;
	memcpy(n.data.className, className, strlen(className) + 1);

	if (this->node->objType == Empty && this->parent == NULL && this->left == NULL && this->right == NULL)
		memcpy(node, &n, sizeof(Node));
	else
	{
		cur->SetLeft(&n);
		cur = cur->left;
	}

	if (ot == ObjFunct || ot == ObjClass)
	{
		v = cur;
		memcpy(&n.id, &"", 2);
		n.objType = Empty;

		cur->SetRight(&n);
		cur = cur->right;
		return v;
	}

	return cur;
}

Tree* Tree::SemGetVar(LEX a)
{
	Tree* v = FindUp(cur, a);

	if (v == NULL)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Отсутствует описание идентификатора");
	}

	if (v->node->objType == ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Неверное использование имени функции");
	}

	if (v->node->objType == ObjClass)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Неверное использование имени класса");
	}

	return v;
}

Tree* Tree::SemNewLevel()
{
	Node n;

	memcpy(&n.id, &"", 2);
	n.objType = Empty;

	cur->SetLeft(&n);
	cur = cur->left;
	Tree* v = cur;

	cur->SetRight(&n);
	cur = cur->right;

	return v;
}

Tree* Tree::SemGetFunct(LEX a)
{
	Tree* v = FindUp(cur, a);

	if (v == NULL)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Отсутствует описание функции");
	}

	if (v->node->objType != ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Идентификатор не является именем функции");
	}

	return v;
}

Tree* Tree::SemGetClass(LEX a)
{
	Tree* v = FindUp(cur, a);

	if (v == NULL)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Отсутствует описание класса");
	}

	if (v->node->objType != ObjClass)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Идентификатор не является именем класса");
	}

	return v;
}

int Tree::DupControl(Tree* addr, LEX a)
{
	if (FindUpOneLevel(addr, a) == NULL) return 0;
	return 1;
}

void Tree::MakeClassCopy(Tree* clss)
{
	if (clss->right != NULL)
	{
		this->right = new Tree(clss->right->node, this);

		this->right->MakeChildCopy(clss->right);
	}
}

void Tree::MakeChildCopy(Tree* source)
{
	if (source->left != NULL)
	{
		this->left = new Tree(source->left->node, this);

		this->left->MakeChildCopy(source->left);
	}

	if (source->right != NULL)
	{
		this->right = new Tree(source->right->node, this);

		this->right->MakeChildCopy(source->right);
	}
}

DATA_TYPE Tree::GetTypebyLex(int lexType)
{
	if (lexType == TBool)
		return TYPE_BOOL;
	
	if (lexType == TDouble)
		return TYPE_DOUBLE;

	return TYPE_OBJ_CL;
}

DATA_TYPE Tree::GetType()
{
	return node->data.dataType;
}

void Tree::GetClassName(LEX name)
{
	strcpy(name, node->data.className);
}

DATA_VALUE* Tree::GetValue()
{
	return &(node->data.dataValue);
}

Tree* Tree::GetCurrentFunct()
{
	if (parent == NULL)
		return NULL;

	if (node->objType == Empty && parent->node->objType == ObjFunct && parent->right == this)
		return parent;

	return parent->GetCurrentFunct();
}

void Tree::TypeCasting(DataS* firstData, DataS secondData, int operation, LEX operationName)
{
	DATA_TYPE resType = firstData->dataType;

	if (firstData->dataType == TYPE_OBJ_CL || secondData.dataType == TYPE_OBJ_CL)
		scan->PrintError("Объект является экземпляром класса - недопустимо проведение операции", "\0", '\0');

	if (resType != secondData.dataType)
	{
		if (resType == NO_TYPE || secondData.dataType == NO_TYPE)
			resType = NO_TYPE;
		else if (resType == TYPE_DOUBLE || secondData.dataType == TYPE_DOUBLE)
			resType = TYPE_DOUBLE;
		else if (resType == TYPE_BOOL || secondData.dataType == TYPE_BOOL)
			resType = TYPE_BOOL;
		else
			scan->PrintError("Ошибка приведения типов", "\0", '\0');
	}

	printf("\nПриведение типов %s и %s (%s) --> %s ------ строка %d\n", DT_Name[firstData->dataType], DT_Name[secondData.dataType], operationName, DT_Name[resType], scan->Get_Number_Line());

	if (firstData->dataType != resType)
	{
		if (resType == TYPE_DOUBLE)
		{
			if (firstData->dataType == TYPE_BOOL)
			{
				firstData->dataValue.DataAsDouble = firstData->dataValue.DataAsBool;
			}
		}
		else
		{
			if (firstData->dataType == TYPE_DOUBLE)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool;
			}
		}
	}

	firstData->dataType = resType;

	if (resType == TYPE_DOUBLE)
	{
		if (secondData.dataType == TYPE_DOUBLE)
		{
			//L - слагаемое
			if (operation == TPlus)
				firstData->dataValue.DataAsDouble += secondData.dataValue.DataAsDouble;
			else if (operation == TMinus)
				firstData->dataValue.DataAsDouble -= secondData.dataValue.DataAsDouble;

			//M - множитель
			else if (operation == TMult)
				firstData->dataValue.DataAsDouble *= secondData.dataValue.DataAsDouble;
			else if (operation == TDiv)
				firstData->dataValue.DataAsDouble /= secondData.dataValue.DataAsDouble;

			//Z - сравнение
			else if (operation == TLT)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble < secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TGT)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble > secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TLE)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble <= secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TGE)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble >= secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}

			//V - выражение
			else if (operation == TEQ)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble == secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TNEQ)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble != secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
		}
		else if (secondData.dataType == TYPE_BOOL)
		{
			if (operation == TPlus)
				firstData->dataValue.DataAsDouble += secondData.dataValue.DataAsBool;
			else if (operation == TMinus)
				firstData->dataValue.DataAsDouble -= secondData.dataValue.DataAsBool;
			else if (operation == TMult)
				firstData->dataValue.DataAsDouble *= secondData.dataValue.DataAsBool;
			else if (operation == TDiv)
				firstData->dataValue.DataAsDouble /= secondData.dataValue.DataAsBool;

			else if (operation == TLT)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble < secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TGT)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble > secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TLE)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble <= secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TGE)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble >= secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}

			else if (operation == TEQ)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble == secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TNEQ)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsDouble != secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
		}
	}
	else if (resType == TYPE_BOOL)
	{
		if (secondData.dataType == TYPE_DOUBLE)
		{
			if (operation == TPlus)
				firstData->dataValue.DataAsBool += secondData.dataValue.DataAsDouble;
			else if (operation == TMinus)
				firstData->dataValue.DataAsBool -= secondData.dataValue.DataAsDouble;
			else if (operation == TMult)
				firstData->dataValue.DataAsBool *= secondData.dataValue.DataAsDouble;
			else if (operation == TDiv)
				firstData->dataValue.DataAsBool /= secondData.dataValue.DataAsDouble;

			else if (operation == TLT)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool < secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TGT)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool > secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TLE)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool <= secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TGE)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool >= secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}

			else if (operation == TEQ)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool == secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TNEQ)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool != secondData.dataValue.DataAsDouble;
				firstData->dataType = TYPE_BOOL;
			}
		}
		else if (secondData.dataType == TYPE_BOOL)
		{
			if (operation == TPlus)
				firstData->dataValue.DataAsBool += secondData.dataValue.DataAsBool;
			else if (operation == TMinus)
				firstData->dataValue.DataAsBool -= secondData.dataValue.DataAsBool;
			else if (operation == TMult)
				firstData->dataValue.DataAsBool *= secondData.dataValue.DataAsBool;
			else if (operation == TDiv)
				firstData->dataValue.DataAsBool /= secondData.dataValue.DataAsBool;
			else if (operation == TMod)
				firstData->dataValue.DataAsBool %= secondData.dataValue.DataAsBool;
			else if (operation == TAnd)
				firstData->dataValue.DataAsBool &= secondData.dataValue.DataAsBool;
			else if (operation == TOR)
				firstData->dataValue.DataAsBool |= secondData.dataValue.DataAsBool;
			else if (operation == TXOR)
				firstData->dataValue.DataAsBool ^= secondData.dataValue.DataAsBool;

			else if (operation == TLT)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool < secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TGT)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool > secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TLE)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool <= secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TGE)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool >= secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}

			else if (operation == TEQ)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool == secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
			else if (operation == TNEQ)
			{
				firstData->dataValue.DataAsBool = firstData->dataValue.DataAsBool != secondData.dataValue.DataAsBool;
				firstData->dataType = TYPE_BOOL;
			}
		}
	}

}

DataS Tree::TypeCastingAssign(DATA_TYPE firstType, DataS second, LEX firstTypeName, LEX secondTypeName)
{
	if (firstType == TYPE_OBJ_CL)
	{
		if (second.dataType == TYPE_OBJ_CL)
		{
			if (strcmp(firstTypeName, secondTypeName) == 0)
			{
				printf("\nКонтроль приведения типов: Присваивание объекту класса %s объекта класса %s ------ строка %d\n", firstTypeName, secondTypeName, scan->Get_Number_Line());
			}
			else
			{
				scan->PrintError("Попытка присваивания объекту класса \"" + string(firstTypeName) +  "\" объекта класса \"" + string(secondTypeName) + "\"", "\0", '\0');
			}
		}
		else
		{
			scan->PrintError("Попытка присваивания объекту класса \"" + string(firstTypeName) + "\" значения типа \"" + string(DT_Name[second.dataType]) + "\"", "\0", '\0');
		}
	}
	else if (second.dataType == TYPE_OBJ_CL)
	{
		scan->PrintError("\nПопытка присваивания переменной типа \"" + string(DT_Name[firstType]) + "\" объекта класса", "\0", '\0');
	}
	else
	{
		printf("\nКонтроль приведения типов: Приведение типа %s к типу %s (присваивание) --> %s ------ строка %d\n", DT_Name[second.dataType], DT_Name[firstType], DT_Name[firstType], scan->Get_Number_Line());

		if (firstType == TYPE_BOOL)
		{
			if (second.dataType != TYPE_BOOL)
			{
				if (second.dataType == TYPE_DOUBLE)
				{
					second.dataValue.DataAsBool = second.dataValue.DataAsDouble;
				}

				second.dataType = TYPE_BOOL;
			}

			printf("\tТип: bool\tЗначение: %d", second.dataValue.DataAsBool);
		}
		else if (firstType == TYPE_DOUBLE)
		{
			if (second.dataType != TYPE_DOUBLE)
			{
				if (second.dataType == TYPE_BOOL)
				{
					second.dataValue.DataAsDouble = second.dataValue.DataAsBool;
				}

				second.dataType = TYPE_DOUBLE;
			}

			printf("\tТип: double\tЗначение: %f", second.dataValue.DataAsDouble);
		}

		return second;
	}
}

//DATA_TYPE Tree::TypeCasting(DATA_TYPE firstType, DATA_TYPE secondType, LEX operation)
//{
//	if (firstType == TYPE_OBJ_CL || secondType == TYPE_OBJ_CL)
//		scan->PrintError("Объект является экземпляром класса - недопустимо проведение операции", "\0", '\0');
//
//	DATA_TYPE resType = firstType;
//
//	if (firstType != secondType)
//	{
//		if (firstType == NO_TYPE || secondType == NO_TYPE)
//			resType = NO_TYPE;
//		else if (firstType == TYPE_DOUBLE || secondType == TYPE_DOUBLE)
//			resType = TYPE_DOUBLE;
//		else
//			resType = TYPE_BOOL;
//
//	}
//
//	printf("\nПриведение типов %s и %s (%s) --> %s ------ строка %d\n", DT_Name[firstType], DT_Name[secondType], operation, DT_Name[resType], scan->Get_Number_Line());
//
//	return resType;
//}

void Tree::CheckTypeBool(DATA_TYPE type)
{
	if (type != TYPE_BOOL)
	{
		scan->PrintError("Выражение должно относится к целочисленному типу");
	}
}


