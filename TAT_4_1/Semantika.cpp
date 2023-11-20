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
	Tree* i = from;		//������� ������� ������

	while ((i != NULL) && (i->parent == NULL || i->parent->right != i))
	{
		if (memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) == 0)
			return i;	//������ ����������� �������������

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
		scan->PrintError("����������� �������� ��������������");
	}

	if (v->node->objType == ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("�������� ������������� ����� �������");
	}

	if (v->node->objType == ObjClass)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("�������� ������������� ����� ������");
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
		scan->PrintError("����������� �������� ������");
	}

	if (v->node->objType != ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("������������� �� �������� ������ ������");
	}

	return v;
}

void Tree::Print()
{
	if (node->objType != Empty)
		printf("������� � ������� %s ------>", node->id);
	else
		printf("������� ������ ------>");

	if (left != NULL)
	{
		if (left->node->objType != Empty)
			printf("      ����� ������ %s", left->node->id);
		else
			printf("      ����� ������ �������");
	}


	if (right != NULL)
	{
		if (right->node->objType != Empty)
			printf("      ������ ������ %s", right->node->id);
		else
			printf("      ������ ������ �������");
	}

	printf("\n");

	if (left != NULL)
		left->Print();

	if (right != NULL)
	{
		printf("\n\n��������: ");
		if (node->objType != Empty)
			printf("������� %s\n", node->id);
		else
			printf("������� ������\n");
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
		scan->PrintError("��������� �������� ��������������");
	}

	Tree* v;
	Node n;

	memcpy(n.id, a, strlen(a) + 1);
	n.objType = ot;
	n.dataType = t;
	memcpy(n.className, "", strlen("") + 1);

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
		scan->PrintError("��������� �������� ��������������");
	}

	Tree* v;
	Node n;

	memcpy(n.id, a, strlen(a) + 1);
	n.objType = ot;
	n.dataType = t;
	memcpy(n.className, className, strlen(className) + 1);

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
		scan->PrintError("����������� �������� ��������������");
	}

	if (v->node->objType == ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("�������� ������������� ����� �������");
	}

	if (v->node->objType == ObjClass)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("�������� ������������� ����� ������");
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
		scan->PrintError("����������� �������� �������");
	}

	if (v->node->objType != ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("������������� �� �������� ������ �������");
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
		scan->PrintError("����������� �������� ������");
	}

	if (v->node->objType != ObjClass)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("������������� �� �������� ������ ������");
	}

	return v;
}

int Tree::DupControl(Tree* addr, LEX a)
{
	if (FindUpOneLevel(addr, a) == NULL) return 0;
	return 1;
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
	return node->dataType;
}

void Tree::GetClassName(LEX name)
{
	strcpy(name, node->className);
}

Tree* Tree::GetCurrentFunct()
{
	if (parent == NULL)
		return NULL;

	if (node->objType == Empty && parent->node->objType == ObjFunct && parent->right == this)
		return parent;

	return parent->GetCurrentFunct();
}

void Tree::TypeCastingAssign(DATA_TYPE firstType, DATA_TYPE secondType, LEX firstTypeName, LEX secondTypeName)
{
	if (firstType == TYPE_OBJ_CL)
	{
		if (secondType == TYPE_OBJ_CL)
		{
			if (strcmp(firstTypeName, secondTypeName) == 0)
			{
				printf("\n�������� ���������� �����: ������������ ������� ������ %s ������� ������ %s ------ ������ %d\n", firstTypeName, secondTypeName, scan->Get_Number_Line());
			}
			else
			{
				scan->PrintError("������� ������������ ������� ������ \"" + string(firstTypeName) +  "\" ������� ������ \"" + string(secondTypeName) + "\"", "\0", '\0');
			}
		}
		else
		{
			scan->PrintError("������� ������������ ������� ������ \"" + string(firstTypeName) + "\" �������� ���� \"" + string(DT_Name[secondType]) + "\"", "\0", '\0');
		}
	}
	else if (secondType == TYPE_OBJ_CL)
	{
		scan->PrintError("\n������� ������������ ���������� ���� \"" + string(DT_Name[secondType]) + "\" ������� ������", "\0", '\0');
	}
	else
	{
		printf("\n�������� ���������� �����: ���������� ���� %s � ���� %s --> %s ------ ������ %d\n", DT_Name[secondType], DT_Name[firstType], DT_Name[firstType], scan->Get_Number_Line());
	}
}

DATA_TYPE Tree::TypeCasting(DATA_TYPE firstType, DATA_TYPE secondType, LEX operation)
{
	if (firstType == TYPE_OBJ_CL || secondType == TYPE_OBJ_CL)
		scan->PrintError("������ �������� ����������� ������ - ����������� ���������� ��������", "\0", '\0');

	DATA_TYPE resType = firstType;

	if (firstType != secondType)
	{
		if (firstType == NO_TYPE || secondType == NO_TYPE)
			resType = NO_TYPE;
		else if (firstType == TYPE_DOUBLE || secondType == TYPE_DOUBLE)
			resType = TYPE_DOUBLE;
		else
			resType = TYPE_BOOL;

	}

	printf("\n���������� ����� %s � %s (%s) --> %s ------ ������ %d\n", DT_Name[firstType], DT_Name[secondType], operation, DT_Name[resType], scan->Get_Number_Line());

	return resType;
}

void Tree::CheckTypeBool(DATA_TYPE type)
{
	if (type != TYPE_BOOL)
	{
		scan->PrintError("��������� ������ ��������� � �������������� ����");
	}
}


