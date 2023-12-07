#pragma once
#include "Scaner.h"
#include "Semantika.h"

class dias
{
	LEX Operation_Name[14] = { "|", "<", "<=", ">", ">=", "==",
						"!=", "+", "-", "^", "&", "*", "/", "%" };


private:
	Scaner* scan;
	Tree* root;

	void D();		//�������� ������
	void B(Tree** ident);		//���
	void I();		//�������� ������
	void F();		//�������
	void K();		//��������� ��������
	void M();		//��������
	void H();		//return
	void N();		//if
	void O();		//������������
	void Q(DataS* res);		//���������
	void R(DataS* res);		//���������
	void U(DataS* res);		//�
	void W(DataS* res);		//���������
	void X(DataS* res);		//���������
	void Y(DataS* res);		//���������
	void Z(DataS* res);		//�� ������
	void P(DataS* res);		//����� �������
	void V(DataS* res);		//���������
	int Look_Forward(int n);

public:
	bool DEBUG = true;
	dias(Scaner* s) { scan = s; root = new	Tree(scan); }
	~dias() {
		if (DEBUG) {
			printf("������������ ������ - ����� ���������\n");
		}
		root->CleanTree();
		delete root;
	}
	void S();		//���������

	void PrintTree();
};

