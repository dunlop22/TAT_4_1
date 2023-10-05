#pragma once

#include <iostream>
#include <fstream>		//������ � �������

using namespace std;

#define MAX_TEXT 100000     //������������ ����� ������ ��
#define MAX_LEX 100         //������������ ����� �������
#define MAX_KEYW 8			//����� �������� ����

typedef char IM[MAX_TEXT];   //����� ��
typedef char LEX[MAX_LEX];   //�������

//�������� �����
#define TBool		1
#define TDouble     2
#define TIf			3
#define TElse		4
#define TMain		5
#define TReturn		6
#define TTrue		7
#define TFalse		8

//��������������
#define TIdent      10

//���������
#define TConstInt		20
#define TConstFloat		21

//����������� �����
#define TTochka     30	//.
#define TZapya		31	//,
#define TTochkaZap  32	//;
#define TLS         33	//(
#define TRS			34	//)
#define TFLS        35	//{
#define TFRS        36	//}

//����� ��������
#define TOR			40	//|
#define TLT         41	//<
#define TLE         42	//<=
#define TGT         43	//>
#define TGE         44	//>=
#define TEQ         45	//==
#define TNEQ        46	//!=
#define TPlus       47	//+
#define TMinus      48	//-
#define TXOR	    49	//^
#define TAnd	    50	//&
#define TSave       51	//=
#define TMult       52	//*
#define TDiv        53	///
#define TMod        54	//%

//����� ��������� ������
#define TEnd        70

//��������� ������
#define TError      71