#pragma once
#include "defs.h"
#include "Scaner.h"

enum OBJ_TYPE {
	Empty = 0,
	ObjVar,		//������� ����������
	ObjFunct,	//�������
	ObjClass,	//����� ��� ���
	ObjObjectCl	//������ ������
};

enum DATA_TYPE { NO_TYPE = 0, TYPE_BOOL, TYPE_DOUBLE, TYPE_OBJ_CL};

union DATA_VALUE 
{
	bool DataAsBool;
	double DataAsDouble;
};

struct DataS 
{
	DATA_TYPE dataType;	//��� ��������
	DATA_VALUE dataValue; //��������

	LEX className;		//��� ������ ��� �������� �������
};

struct Node
{
	OBJ_TYPE objType;	//��� �������
	LEX id;				//������������� �������

	DataS data;
};

class Tree			//������� ������������� �������
{
	LEX DT_Name[3] = { "�����.", "bool", "double" };

private:
	Node* node;		//���������� �� ������� �������
	Tree* parent, * left, * right;		//��������, ����� � ������ �������

public:
	static Scaner* scan;
	static Tree* cur;		//������� ������� ������

	Tree(Tree* p, Tree* l, Tree* r, Node* data);
	Tree(Scaner* scan);

	//������� ��������� ��������� ������
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

	//������������� ������������
	void SetCur(Tree* a);			//���������� ������� ���� ������
	Tree* GetCur();					//�������� ������ �� ������� ���� ������
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//��������� �������������� a � ������� � ����� t
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t, LEX className);		//��������� �������������� a � ������� � ����� t
	Tree* SemNewLevel();			//����� ������� (��� ���������� ���������)
	Tree* SemGetVar(LEX a);		//����� � ������� ���������� � ������ a � ������� ������ �� �������������� ������� ������
	Tree* SemGetFunct(LEX a);		//����� � ������� ������� � ������ a � ������� ������ �� �������������� ������� ������
	Tree* SemGetClass(LEX a);		//����� � ������� ����� � ������ a � ������� ������ �� �������������� ������� ������
	int DupControl(Tree* addr, LEX a);			//�������� �������������� a �� ��������� �������� ������ �����

	DATA_TYPE GetTypebyLex(int lexType);
	DATA_TYPE GetType();

	void GetClassName(LEX name);
	DATA_VALUE* GetValue();

	Tree* GetCurrentFunct();

	DataS TypeCastingAssign(DATA_TYPE firstType, DataS second, LEX firstTypeName, LEX secondTypeName);	//���������� ����� ��� ������������
	DATA_TYPE TypeCasting(DATA_TYPE firstType, DATA_TYPE secondType, LEX operation);

	void CheckTypeBool(DATA_TYPE type);
};

