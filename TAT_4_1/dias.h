#pragma once
#include "Scaner.h"
#include "Semantika.h"

class dias
{
private:
	Scaner* scan;
	Tree* root;

	void D();		//�������� ������
	void B();		//���
	void I();		//�������� ������
	void F();		//�������
	void K();		//��������� ��������
	void M();		//��������
	void H();		//return
	void N();		//if
	void O();		//������������
	void Q(DATA_TYPE* resType, LEX* resTypeName);		//���������
	void R(DATA_TYPE* resType, LEX* resTypeName;		//���������
	void U(DATA_TYPE* resType, LEX* resTypeName);		//�
	void W(DATA_TYPE* resType, LEX* resTypeName);		//���������
	void X(DATA_TYPE* resType, LEX* resTypeName);		//���������
	void Y(DATA_TYPE* resType, LEX* resTypeName);		//���������
	void Z(DATA_TYPE* resType, LEX* resTypeName);		//�� ������
	void P(DATA_TYPE* resType, LEX* resTypeName);		//����� �������
	void V(DATA_TYPE* resType, LEX* resTypeName);		//���������
	int Look_Forward(int n);

public:
	dias(Scaner* s) { scan = s; root = new	Tree(scan); }
	~dias() {}
	void S();		//���������

	void PrintTree();
};

