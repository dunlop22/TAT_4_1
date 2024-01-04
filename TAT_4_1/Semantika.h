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

struct FStart {
	int uk;
	int line;
	int pos;
};

struct Node
{
	OBJ_TYPE objType;	//��� �������
	LEX id;				//������������� �������

	DataS data;
	FStart funcStart; //������ �������
};

class Tree			//������� ������������� �������
{
	LEX DT_Name[3] = { "�����.", "bool", "double" };

private:
	Node* node;		//���������� �� ������� �������
	Tree* parent, * left, * right;		//��������, ����� � ������ �������

public:
	static Tree* lastcur;
	bool flagInterpret = true;
	bool flagReturn = false;

	static Scaner* scan;
	static Tree* cur;		//������� ������� ������

	Tree(Tree* p, Tree* l, Tree* r, Node* data);
	Tree(Scaner* scan);
	Tree(Node* data, Tree* parent);

	//������� ��������� ��������� ������
	void SetLeft(Node* data);
	void SetRight(Node* data);
	void SetStart(int uk, int line, int pos);
	FStart GetStart();

	void SetRightT(Tree* node);
	Tree* GetRight();

	void Back();

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
	Tree* SemInclude(Tree* first);	
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//��������� �������������� a � ������� � ����� t
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t, LEX className);		//��������� �������������� a � ������� � ����� t
	Tree* SemNewLevel();			//����� ������� (��� ���������� ���������)
	Tree* SemGetVar(LEX a);		//����� � ������� ���������� � ������ a � ������� ������ �� �������������� ������� ������
	Tree* SemGetFunct(LEX a);		//����� � ������� ������� � ������ a � ������� ������ �� �������������� ������� ������
	Tree* SemGetClass(LEX a);		//����� � ������� ����� � ������ a � ������� ������ �� �������������� ������� ������
	int DupControl(Tree* addr, LEX a);			//�������� �������������� a �� ��������� �������� ������ �����
	void MakeClassCopy(Tree* clss);			//����������� ���� ������ 
	void MakeChildCopy(Tree* source);

	DATA_TYPE GetTypebyLex(int lexType);
	DATA_TYPE GetType();

	void GetClassName(LEX name);
	DATA_VALUE* GetValue();

	Tree* GetCurrentFunct();

	void TypeCasting(DataS* firstData, DataS secondData, int operation, LEX operationName);
	DataS TypeCastingAssign(DATA_TYPE firstType, DataS second, LEX firstTypeName, LEX secondTypeName);	//���������� ����� ��� ������������

	void CheckTypeBool(DATA_TYPE type);
};

