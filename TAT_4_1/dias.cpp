#include "dias.h"

int dias::Look_Forward(int n)
{
	LEX lex;
	int  savedPointer = scan->GetUK();
	int  savedLine = scan->Get_Number_Line();
	int  savedPos = scan->Get_Position();

	int  nextType;

	for (int i = 0; i < n; i++)
	{
		nextType = scan->FScaner(lex);
	}

	scan->Set_Line_Number(savedLine);
	scan->Set_Position(savedPos);
	scan->PutUK(savedPointer);

	return nextType;
}

void dias::S()//ready
{
/*	ПРОГРАММА
S = 
			--------
	   |---|	D	|---|		Описание данных
	   |	--------	|
	   |	--------	|
	   |---|	I	|---|		Описание класса
	   |	--------	|
	   |	--------	|
	   |---|	F	|---|		Функция
	   |    --------    |
	   |				|
	   \/				|
------------------------------->
*/
	LEX lex;
	int type;
	
	type = Look_Forward(1);

	while (type == TBool || type == TDouble || type == TIdent || type == TClass)
	{
		if (type == TClass)
		{
			I();		//Описание класса
		}
		else
		{
			type = Look_Forward(2);
			if (type == TMain)		//main
			{
				F();
			}
			else if (type == TIdent)	
			{
				type = Look_Forward(3);

				if (type == TSave || type == TTochkaZap || type == TTochka)
				{
					D();	//Описание данных
				}
				else
				{
					F();	//Функция
				}
			}
		}
		type = Look_Forward(1);
	}
}

void dias::D()//ready
{
/* ОПИСАНИЕ ДАННЫХ
D =
								-------------  ,  ------------------
		 ---  bool  ---			|									|
		|			   |		\/									|
--------|--  double  --|-----------  a  ---------------------------------  ;  ------>
		|			   |					|					|
		 -----  a  ----						|					|
											|	     -------	|
											 -  =  -|	Q	|---
													 -------
*/

	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TBool && type != TDouble && type != TIdent)
	{
		scan->PrintError("Ожидался тип", lex, '\0');
	}

	do
	{
		type = scan->FScaner(lex);

		if (type != TIdent)
		{
			scan->PrintError("Ожидался идентификатор", lex, '\0');
		}

		type = Look_Forward(1);

		if (type == TSave)
		{
			type = scan->FScaner(lex);
			Q();
			type = Look_Forward(1);
		}

	} while (type == TZapya);	//,

	type = scan->FScaner(lex);

	if (type != TTochkaZap)		//;
	{
		scan->PrintError("Ожидался символ \";\"", lex, '\0');
	}
}


void dias::F()//ready
/*	
ФУНКЦИЯ =
										   Имя
										 -------
									 ---|   B   |---
		 ---  bool  ---				|	 -------	|				Составной оператор	
		|			   |			|				|					 -------
--------|--  double  --|------------|				|---  (  ---  )  ---|   K   |---->
		|			   |			|				|					 -------
		 -----  a  ----				 ----  main  ---				 
*/
{
	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TBool && type != TDouble && type != TIdent)
	{
		scan->PrintError("Ожидался тип", lex, '\0');
	}

	type = Look_Forward(1);
	
	if (type != TMain)
	{
		B();
	}
	else
	{
		type = scan->FScaner(lex);
	}

	type = scan->FScaner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ \"(\"", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ \")\"", lex, '\0');
	}

	K();
}

void dias::K()
{
/*
СОСТАВНОЙ ОПЕРАТОР =
				 -------
			----|	D	|---		Описание данных
		   |	 --------	|
		   |	 -------	|
		   |----|	I	|---|		Описание класса
		   |	 -------	|
		   |	 -------	|
		   |----|	M	|---|		Оператор
		   |     -------	|
		   |				|
		   \/				|
---  {  ------------------------  }  --->
*/
	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TFLS)
	{
		scan->PrintError("Ожидался символ '{'", lex, '\0');
	}

	type = Look_Forward(1);

	while (type == TBool || type == TDouble || type == TIdent || type == TClass || type == TReturn || type == TFLS || type == TTochkaZap || type == TIf)
	{
		if (type == TClass)
		{
			I();
		}
		else if (type == TBool || type == TDouble)
		{
			D();
		}
		else if (type == TIdent)
		{
			type = Look_Forward(3);

			if (type == TSave || type == TTochkaZap || type == TZapya)
			{
				D();
			}
			else
			{
				M();
			}
		}
		else
		{
			M();
		}

		type = Look_Forward(1);
	}
	type = scan->FScaner(lex);
	if (type != TFRS)
	{
		scan->PrintError("Ожидался символ \"}\"", lex, '\0');
	}

}


void dias::M()//ready
{
/*
ОПЕРАТОР =
					Составной оператор
						 -------
		 ---------------|	K	|-------------------
		|				 -------					|
		|					if						|
		|				 -------					|
		|---------------|	N	|-------------------|
		|				 -------					|
		|				Присваивание				|
--------|				 -------					|
		|		 -------|	O	|-----------		|
		|		|		 -------			|		|
		|		|		 return				|		|
		|		|		 -------          	|		|
		 -------|-------|	H	|-----------|-- ; --|
				|		 -------			|
				|							|
				 ---------------------------
*/
	LEX lex;
	int type;

	type = Look_Forward(1);

	if (type == TFLS)
	{
		K();
	}
	else if (type == TIf)
	{
		N();
	}
	else
	{
		if (type == TReturn)
		{
			H();
		}
		else if (type == TIdent)
		{
			O();
		}

		type = scan->FScaner(lex);

		if (type != TTochkaZap)
		{
			scan->PrintError("Ожидался символ \";\"", lex, '\0');
		}
	}
}


void dias::H()//ready
{
/*
return =
					Выражение
					 -------
------  return  ----|	Q	| ------>
					 -------
*/
	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TReturn)
	{
		scan->PrintError("Ожидался оператор \"return\"", lex, '\0');
	}

	Q();
}


void dias::N()//ready
{
/*
if =
																		 Оператор
																		 -------
														 ----  else  ---|	M	|---
														|				 -------	|
						Выражение			Оператор	|							|
						 -------			 -------	|							|
-----  if  ---  (  -----|	Q	|---  )  ---|	M	|--------------------------------------->
						 -------			 -------
*/
	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TIf)
	{
		scan->PrintError("Ожидался оператор \"if\"", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ \"(\"", lex, '\0');
	}

	Q();

	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ \")\"", lex, '\0');
	}

	M();

	type = Look_Forward(1);

	if (type == TElse)
	{
		type = scan->FScaner(lex);
		M();
	}

}

void dias::O()//ready
{
/*
ПРИСВАИВАНИЕ =
		 -------			 -------
 -------|	B	|---  =  ---|	Q	|----->
		 -------			 -------
		   Имя				Выражение
*/
	LEX lex;
	int type;

	B();

	type = scan->FScaner(lex);

	if (type != TSave)
	{
		scan->PrintError("Ожидался знак \"=\"", lex, '\0');
	}

	Q();
}


void dias::Q()//ready
{
/*
ВЫРАЖЕНИЕ =
					   XOR
					 -------
				 ---|	R	|--  |  ----
				|	 -------			|
	 -------	\/						|
----|	R	|------------------------------->
	 -------
	   XOR
*/

	LEX lex;
	int type;
	R();
	type = Look_Forward(1);
	while (type == TOR)
	{
		type = scan->FScaner(lex);
		R();
		type = Look_Forward(1);
	}
}

void dias::R()//ready
{
/*
XOR =
						И	
					 -------
				 ---|	U	|--  ^  ----
				|	 -------			|
	 -------	\/						|
----|	U	|------------------------------->
	 -------
		И
*/


	LEX lex;
	int type;
	U();
	type = Look_Forward(1);
	while (type == TXOR)
	{
		type = scan->FScaner(lex);
		U();
		type = Look_Forward(1);
	}
}

void dias::V()//ready
{
/*
									 ----  ==  -----
						 -------	|				|
					|---|	W	|---|				|---|
					|	--------	|				|	|
					|				 ----  !=  -----	|
					|									|
		 -------	\/									|
 -------|	W	|------------------------------------------->
		 -------
*/

	LEX lex;
	int type;
	W();
	type = Look_Forward(1);
	while (type == TEQ || type == TNEQ)
	{
		type = scan->FScaner(lex);
		W();
		type = Look_Forward(1);
	}
}



void dias::U()//ready
{
/*
И =
					Равенство	
					 -------
				 ---|	V	|--  &  ----
				|	 -------			|
	 -------	\/						|
----|	V	|------------------------------->
	 -------		 
	Равенство	
*/

	LEX lex;
	int type;
	V();
	type = Look_Forward(1);
	while (type == TAnd)
	{
		type = scan->FScaner(lex);
		V();
		type = Look_Forward(1);
	}
}


void dias::W()//ready
{
/*
СРАВНЕНИЕ =
													 ----  >   -----
									Слагаемое		|				|
									 -------		|----  <   -----|
							 -------|	X	|-------|				|-------
							|		 -------		|----  <=  -----|		|
							|						|				|		|
							|						 ----  >=  -----		|
							|												|
			 -------		\/												|
 -----------|	X	|--------------------------------------------------------------->
			 -------
			Слагаемое
*/


	LEX lex;
	int type;

	X();

	type = Look_Forward(1);

	while (type == TLT || type == TGT || type == TLE || type == TGE)
	{
		type = scan->FScaner(lex);
		X();
		type = Look_Forward(1);
	}
}


void dias::X()//ready
{
/*
СЛАГАЕМОЕ =
						Множитель
						 -------	---  +  ---
					 ---|	Y	|--|			|---
					|	 -------	---  -  ---		|
					|								|
	 -------		\/								|
 ---|	Y	|----------------------------------------->
	 -------
	Множитель
*/



	LEX lex;
	int type;

	Y();

	type = Look_Forward(1);

	while (type == TPlus || type == TMinus)
	{
		type = scan->FScaner(lex);
		Y();
		type = Look_Forward(1);
	}

}

void dias::Y()//ready
{
/*
МНОЖИТЕЛЬ =
						 Со знаком	  ----  *  -----
						  -------	 |				|
					 ----|	 Z	 |---|----  /  -----|---
					|	  -------	 |				|	|
					|				  ----  =  -----	|
     -------		\/									|
----|	Z	|-------------------------------------------------->
	 -------
	Со знаком
*/

	LEX lex;
	int type;

	Z();
	type = Look_Forward(1);

	while (type == TMult || type == TDiv || type == TMod)
	{
		type = scan->FScaner(lex);
		Z();
		type = Look_Forward(1);
	}
}


void dias::Z()//ready
{
/*			
СО ЗНАКОМ =
							   Имя
							 -------
						 ---|	B	|---
						|	 -------	|
						|				|
						|---   с1   ----|
	 ---  +  ---		|				|
	|			|		|---   с2   ----|		
	|			|		|				|
----|-----------|-------|---  true  ----|--------->
	|			|		|				|
	|			|		|---  false ----|
	 ---  -  ---		|				|
						|	 -------	|
						|---|	P	|---|	Вызов функции
						|	 -------	|
						|				|
						|	 -------	|
						 -(-|	Q	|-)-
							 -------
							Выражение
*/

	LEX lex;
	int type;

	type = Look_Forward(1);

	if (type == TPlus || type == TMinus)
	{
		type = scan->FScaner(lex);
	}

	type = Look_Forward(1);

	if (type == TLS)
	{
		type = scan->FScaner(lex);
		Q();

		type = scan->FScaner(lex);

		if (type != TRS)
		{
			scan->PrintError("Ожидался символ \")\"", lex, '\0');
		}
	}
	else if (type == TMain)
	{
		P();
	}
	else if (type == TIdent)
	{
		B();
		type = Look_Forward(1);
		if (type == TLS)
		{
			P();
		}
	}
	else
	{
		type = scan->FScaner(lex);
		if (type != TConstInt && type != TConstFloat && type != TTrue && type != TFalse)
		{
			scan->PrintError("Ожидалось элементарное выражение", lex, '\0');
		}
	}
}



void dias::P()//ready
{
/*
ВЫЗОВ ФУНКЦИИ =
			   Имя
			 -------
		 ---|	B	|---
		|	 -------	|
		|				|
 -------|				|---  (  ----  )  ---->
		|				|
		 ---  main  ----
*/
	LEX lex;
	int type;

	type = Look_Forward(1);

	if (type != TMain)
	{
		B();
	}
	else
	{
		type = scan->FScaner(lex);
	}
	
	type = scan->FScaner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ \"(\"", lex, '\0');
	}
	
	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ \")\"", lex, '\0');
	}
}

void dias::B()//ready
{
/*
ИМЯ =
		 -----  .  -----
		|				|
		|				|
		\/				|
--------------  a  ------------>
*/

	LEX lex;
	int type;

	do
	{
		type = scan->FScaner(lex);

		if (type != TIdent)
		{
			scan->PrintError("Ожидался идентификатор", lex, '\0');
		}

		type = Look_Forward(1);
	} while (type == TTochka);

}

void dias::I()//ready
{
/*
ОПИСАНИЕ КЛАССА =
											 -------
										 ---|	D	|--- 		Описание данных
										|	 -------	|
										|	 -------	|
										|---|	I	|---|		Описание класса
										|	 -------	|
										|	 -------	|
										|---|	F	|---|		Функция
										|    -------    |
										|				|
										\/				|
-------  class  ----  a  -----  {  ----------------------------  }  -------  ;  --------->
*/

	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TClass)
	{
		scan->PrintError("Ожидалось описание класса", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TIdent)
	{
		scan->PrintError("Ожидался идентификатор", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TFLS)
	{
		scan->PrintError("Ожидался символ \"{\"", lex, '\0');
	}

	type = Look_Forward(1);

	while (type == TBool || type == TDouble || type == TIdent || type == TClass)
	{
		if (type == TClass)
		{
			I();
		}
		else
		{
			type = Look_Forward(2);

			if (type == TMain)
			{
				F();
			}
			else if (type == TIdent)
			{
				type = Look_Forward(3);
				if (type == TSave || type == TTochkaZap || type == TTochka)
				{
					D();
				}
				else
				{
					F();
				}
			}
			else
			{
				type = scan->FScaner(lex);
				type = scan->FScaner(lex);
				scan->PrintError("Ожидался идентификатор", lex, '\0');
			}
		}
		type = Look_Forward(1);
	}

	type = scan->FScaner(lex);

	if (type != TFRS)
	{
		scan->PrintError("Ожидался символ \"}\"", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TTochkaZap)
	{
		scan->PrintError("Ожидался символ \";\"", lex, '\0');
	}
}
