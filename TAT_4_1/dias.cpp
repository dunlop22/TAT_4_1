#include "dias.h"




void dias::S()//готово
{
/*	ПРОГРАММА
* 
* S = 
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
	int uk1;
	
	uk1 = scan->GetUK();
	type = scan->FScaner(lex);

	while (type == TBool || type == TDouble || type == TIdent || type == TClass)
	{
		if (type == TClass)
		{
			scan->PutUK(uk1);
			I();
		}
		else
		{
			type = scan->FScaner(lex);

			if (type == TMain)
			{
				scan->PutUK(uk1);
				F();
			}
			else if (type == TIdent)
			{
				type = scan->FScaner(lex);
				scan->PutUK(uk1);

				if (type == TSave || type == TTochkaZap || type == TTochka)
				{
					D();
				}
				else
				{
					F();
				}
			}
		}

		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	}
}

void dias::D()//готово
{
	/* ОПИСАНИЕ ДАННЫХ
	* D =

			-------------  ,  ------------------
		   |									|
		   \/									|
	-дорисовать-----------  a  ---------------------------------  ;  ------>
						|					|
						|					|
						|	     --------	|
						|-  =  --|	Q	|---|		Функция
								 --------


	*/

	LEX lex;
	int type;
	int uk1;

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

		uk1 = scan->GetUK();
		type = scan->FScaner(lex);

		if (type == TSave)
		{
			Q();

			uk1 = scan->GetUK();
			type = scan->FScaner(lex);
		}

	} while (type == TZapya);	//,

	if (type != TTochkaZap)		//;
	{
		scan->PrintError("Ожидался символ ';'", lex, '\0');
	}
}


void dias::F()//готово
/*	Функция
			Имя
		 -------
	  --|   B   |--
	 |	 -------   |				Составной оператор	
	 |			   |					 -------
-дорисовать----|			   |---  (  ---  )  ----|   K   |---->
	 |			   |					 -------
	  ---  main  --				 
*/
{
	LEX lex;
	int type;
	int uk1;

	type = scan->FScaner(lex);

	if (type != TBool && type != TDouble && type != TIdent)
	{
		scan->PrintError("Ожидался тип", lex, '\0');
	}

	uk1 = scan->GetUK();
	type = scan->FScaner(lex);
	
	if (type != TMain)
	{
		scan->PutUK(uk1);
		B();
	}

	type = scan->FScaner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ '('", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ ')'", lex, '\0');
	}

	K();
}

void dias::K()//готово
{
/*
СОСТАВНОЙ ОПЕРАТОР
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
	int uk1;

	type = scan->FScaner(lex);

	if (type != TFLS)
	{
		scan->PrintError("Ожидался символ '{'", lex, '\0');
	}

	uk1 = scan->GetUK();
	type = scan->FScaner(lex);

	while (type == TBool || type == TDouble || type == TIdent || type == TClass || type == TReturn || type == TFLS || type == TTochkaZap || type == TIf)
	{
		if (type == TClass)
		{
			scan->PutUK(uk1);
			I();
		}
		else if (type == TBool || type == TDouble)
		{
			scan->PutUK(uk1);
			D();
		}
		else if (type == TIdent)
		{
			type = scan->FScaner(lex);
			scan->PutUK(uk1);

			if (type == TSave && type == TTochkaZap && type == TZapya)
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
			scan->PutUK(uk1);
			M();
		}

		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	}

	if (type != TFRS)
	{
		scan->PrintError("Ожидался символ '}'", lex, '\0');
	}

}


void dias::M()//готово
{
/*
						 -------
		 ---------------|	K	|-------------------
		|				 -------					|
		|											|
		|				 -------					|
		|---------------|	N	|-------------------|
		|				 -------					|
		|											|
--------|				 -------					|
		|		 -------|	O	|-----------		|
		|		|		 -------			|		|
		|		|							|		|
		|		|		 -------          	|		|
		 -------|-------|	H	|-----------|-- ; --|
				|		 -------			|
				|							|
				 ---------------------------
*/
	LEX lex;
	int type;
	int uk1;

	uk1 = scan->GetUK();
	type = scan->FScaner(lex);
	scan->PutUK(uk1);

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
			scan->PrintError("Ожидался символ ';'", lex, '\0');
		}
	}
}


void dias::H()
{
/*
return
					 -------
------  return  ----|	Q	| ------>
					 -------
*/
}


void dias::N()
{
/*
if
																		 -------
														 ----  else  ---|	M	|---
														|				 -------	|
														|							|
						 -------			 -------	\/							|
-----  if  ---  (  -----|	Q	|---  )  ---|	M	|--------------------------------------->
						 -------			 -------
*/
}

void dias::O()
{
/*
ПРИСВАИВАНИЕ
		 -------			 -------
 -------|	B	|---  =  ---|	Q	|----->
		 -------			 -------
		   Имя				Выражение
		 
*/
	B();
	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TSave)
	{
		scan->PrintError("Ожидался знак \"+\"", lex, '\0');
	}

	Q();
}


void dias::Q()
{
/*
ВЫРАЖЕНИЕ
		 -----  |  -----
		|				|
		\/	 -------	|
 -----------|	R	|----------->
			 -------
			   XOR
*/

}

void dias::R()
{
/*
XOR
		 -----  ^  -----
		|				|
		\/	 -------	|
 -----------|	U	|----------->
			 -------
				И
*/
}
void dias::V()
{
/*
РАВЕНСТВО
		 -----  ==  ----
		|				|
		|-----  !=  ----|
		|				|
		\/	 -------	|
 -----------|	W	|----------->
			 -------

*/
}

void dias::U()
{
/*
И
		 -----  &  -----
		|				|
		\/	 -------	|
 -----------|	V	|----------->
			 -------
			Равенство
*/
}


void dias::W()
{
/*
СРАВНЕНИЕ
		 -----  <  -----
		|				|
		|-----  >  -----|
		|				|
		|-----  <= -----|
		|				|
		|-----  >= -----|
		|				|
		\/	 -------	|
 -----------|	X	|----------->
			 -------
		    Слагаемое
*/
}


void dias::X()
{
/*
СЛАГАЕМОЕ
		 -----  +  -----
		|				|
		|-----  -  -----|
		|				|
		\/	 -------	|
 -----------|	Y	|----------->
			 -------
		    Множитель
*/
}

void dias::Y()
{
/*
МНОЖИТЕЛЬ
		 -----  *  -----
		|				|
		|-----  /  -----|
		|				|
		|-----  %  -----|
		|				|
		\/	 -------	|
 -----------|	Z	|----------->
			 -------
			Со знаком
*/

}
void dias::Z()
{
/*						
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
						|---|	P	|---|
						|	 -------	|
						|				|
						|	 -------	|
						 -(-|	Q	|-)-
							 -------
*/
}





void dias::P()
{
/*
ВЫЗОВ ФУНКЦИИ
			   Имя
			 -------
		 ---|	B	|---
		|	 -------	|
		|				|
 -------|				|---  (  ----  )  ---->
		|				|
		 ---  main  ----

*/

}

void dias::B()
{
/*
Имя
		 -----  .  -----
		|				|
		|				|
		\/				|
--------------  a  ------------>
*/
}