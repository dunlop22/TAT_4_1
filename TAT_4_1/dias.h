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
	void Q();		//���������
	void R();		//���������
	void U();		//�
	void W();		//���������
	void X();		//���������
	void Y();		//���������
	void Z();		//�� ������
	void P();		//����� �������
	void V();		//���������
	int Look_Forward(int n);

public:
	dias(Scaner* s) { scan = s; root = new	Tree(scan); }
	~dias() {}
	void S();		//���������
};

