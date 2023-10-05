#pragma once

#include <iostream>
#include <fstream>		//Работа с файлами

using namespace std;

#define MAX_TEXT 100000     //макисмальная длина текста ИМ
#define MAX_LEX 100         //максимальная длина лексемы
#define MAX_KEYW 8			//число ключевых слов

typedef char IM[MAX_TEXT];   //текст ИМ
typedef char LEX[MAX_LEX];   //лексема

//ключевые слова
#define TBool		1
#define TDouble     2
#define TIf			3
#define TElse		4
#define TMain		5
#define TReturn		6
#define TTrue		7
#define TFalse		8

//идентификаторы
#define TIdent      10

//константы
#define TConstInt		20
#define TConstFloat		21

//специальные знаки
#define TTochka     30	//.
#define TZapya		31	//,
#define TTochkaZap  32	//;
#define TLS         33	//(
#define TRS			34	//)
#define TFLS        35	//{
#define TFRS        36	//}

//знаки операций
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

//конец исходного модуля
#define TEnd        70

//ошибочный символ
#define TError      71