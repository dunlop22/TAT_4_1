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
	Tree* i = from;		//òåêóùàÿ âåğøèíà ïîèñêà

	while ((i != NULL) && (i->parent == NULL || i->parent->right != i))
	{
		if (memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) == 0)
			return i;	//íàéäåí ñîâïàäàşùèé èäåíòèôèêàòîğ

		i = i->parent;
	}

	return NULL;
}

void Tree::Print()
{
	if (node->objType != Empty)
		printf("Âåğøèíà ñ äàííûìè %s ------>", node->id);
	else
		printf("Âåğøèíà ÏÓÑÒÀß ------>");

	if (left != NULL)
	{
		if (left->node->objType != Empty)
			printf("      ñëåâà äàííûå %s", left->node->id);
		else
			printf("      ñëåâà ÏÓÑÒÀß âåğøèíà");
	}


	if (right != NULL)
	{
		if (right->node->objType != Empty)
			printf("      ñïğàâà äàííûå %s", right->node->id);
		else
			printf("      ñïğàâà ÏÓÑÒÀß âåğøèíà");
	}

	printf("\n");

	if (left != NULL)
		left->Print();

	if (right != NULL)
	{
		printf("\n\nĞÎÄÈÒÅËÜ: ");
		if (node->objType != Empty)
			printf("Âåğøèíà %s\n", node->id);
		else
			printf("Âåğøèíà ÏÓÑÒÀß\n");
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
		scan->PrintError("Ïîâòîğíîå îïèñàíèå èäåíòèôèêàòîğà");
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
		scan->PrintError("Îòñóòñòâóåò îïèñàíèå èäåíòèôèêàòîğà");
	}

	if (v->node->objType == ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Íåâåğíîå èñïîëüçîâàíèå èìåíè ôóíêöèè");
	}

	if (v->node->objType == ObjClass)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Íåâåğíîå èñïîëüçîâàíèå èìåíè êëàññà");
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
		scan->PrintError("Îòñóòñòâóåò îïèñàíèå ôóíêöèè");
	}

	if (v->node->objType != ObjFunct)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Èäåíòèôèêàòîğ íå ÿâëÿåòñÿ èìåíåì ôóíêöèè");
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
		scan->PrintError("Îòñóòñòâóåò îïèñàíèå êëàññà");
	}

	if (v->node->objType != ObjClass)
	{
		Tree* root = FindRoot();
		root->Print();
		scan->PrintError("Èäåíòèôèêàòîğ íå ÿâëÿåòñÿ èìåíåì êëàññà");
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

void Tree::TypeCastingAssign(DATA_TYPE firstType, DATA_TYPE secondType, LEX firstTypeName, LEX secondTypeName)
{
	if (firstType == TYPE_OBJ_CL)
	{
		if (secondType == TYPE_OBJ_CL)
		{
			if (firstTypeName == secondTypeName)
			{
				printf("\nÊîíòğîëü ïğèâåäåíèÿ òèïîâ: Ïğèñâàèâàíèå îáúåêòó êëàññà %s îáúåêòà êëàññà %s ------ ñòğîêà %d\n", firstTypeName, secondTypeName, scan->Get_Number_Line());
			}
			else
			{
				scan->PrintError("Ïîïûòêà ïğèñâàèâàíèÿ îáúåêòó êëàññà \"" + string(firstTypeName) +  "\" îáúåêòà êëàññà \"" + string(secondTypeName) + "\"", '\0', '\0');
			}
		}
		else
		{
			scan->PrintError("Ïîïûòêà ïğèñâàèâàíèÿ îáúåêòó êëàññà \"" + string(firstTypeName) + "\" çíà÷åíèÿ òèïà \"" + string(DT_Name[secondType]) + "\"", '\0', '\0');
		}
	}
	else if (secondType == TYPE_OBJ_CL)
	{
		scan->PrintError("Ïîïûòêà ïğèñâàèâàíèÿ ïåğåìåííîé òèïà \"" + string(DT_Name[secondType]) + "\" îáúåêòà êëàññà", '\0', '\0');
	}
	else
	{
		printf("\nÊîíòğîëü ïğèâåäåíèÿ òèïîâ: Ïğèâåäåíèå òèïà %s ê òèïó %s (ïğèñâàèâàíèå) --> %s ------ ñòğîêà %d\n", scan->Get_Number_Line());
	}
}


