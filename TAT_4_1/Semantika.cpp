#include "Semantika.h"

Tree* Tree::cur = (Tree*)NULL;
Scaner* Tree::scan = (Scaner*)NULL;

Tree::Tree(Scaner* scan)
{
	this->scan = scan;

	node = new Node();
	node->objType = Empty;
	node->dataType = NO_TYPE;

	parent = NULL;
	left = NULL;
	right = NULL;


	cur = this;
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
		scan->PrintError("Повторное описание идентификатора", a, '\0');
	}

	Tree* v;
	Node n;

	memcpy(n.id, a, strlen(a) + 1);
	n.objType = ot;
	n.dataType = t;

	if (this->node->objType == Empty && this->parent == NULL && this->left == NULL && this->right == NULL)
		memcpy(node, &n, sizeof(Node));
	else
	{
		cur->SetLeft(&n);
		cur = cur->left;
	}

	if (ot == ObjFunct)
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
		scan->PrintError("Отсутствует описание идентификатора", a, '\0');
	}

	if (v->node->objType == ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Неверное использование имени функции", a, '\0');
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
		scan->PrintError("Отсутствует описание функции", a, '\0');
	}

	if (v->node->objType != ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Идентификатор не является именем функции", a, '\0');
	}

	return v;
}

int Tree::DupControl(Tree* addr, LEX a)
{
	if (FindUpOneLevel(addr, a) == NULL) return 0;
	return 1;
}

DATA_TYPE Tree::GetType(int lexType)
{
	if (lexType == TBool)
		return TYPE_BOOL;
	
	if (lexType == TDouble)
		return TYPE_DOUBLE;

	return TYPE_OBJ_CL;
}


