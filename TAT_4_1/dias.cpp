#include "dias.h"




void dias::S()
{
/*	���������
* 
* S = 
			--------
	   |---|	D	|---|		�������� ������
	   |	--------	|
	   |	--------	|
	   |---|	I	|---|		�������� ������
	   |	--------	|
	   |	--------	|
	   |---|	F	|---|		�������
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

	while (type == TBool || type == TDouble)
	{

		uk1 = scan->GetUK();
		type = scan->FScaner(lex);


		scan->PutUK(uk1);

		if (type == TMain)
		{
			F();
		}
		else if (type == TIdent)
		{
			type = scan->FScaner(lex);

			if (type == TLS)
			{
				F();
			}
			else if (type = TClass)
			{
				I();
			}
			else
			{
				D();
			}
		}
		else
		{
			scan->PrintError("�������� �������������", lex, '\0');
		}

		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	}
}

void dias::D()
{
	/* �������� ������
	* D =

			-------------  ,  ------------------
		   |									|
		   \/									|
	------------  a  ---------------------------------  ;  ------>
						|					|
						|					|
						|	     --------	|
						|-  =  --|	Q	|---|		�������
								 --------


	*/

	LEX lex;
	int type;
	int uk1;

	do
	{
		type = scan->FScaner(lex);

		if (type != TIdent)
		{
			scan->PrintError("�������� �������������", lex, '\0');
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
		scan->PrintError("�������� ������ ';'", lex, '\0');
	}
}


void dias::F()
/*	�������
			���
		 -------
	  --|   B   |--
	 |	 -------   |				��������� ��������	
	 |			   |					 -------
-----|			   |---  (  ---  )  ----|   K   |---->
	 |			   |					 -------
	  ---  main  --				 
*/
{
	LEX lex;
	int type;
	int uk1;

	type = scan->FScaner(lex);

	
	if (type != TMain)
	{
		B();
	}

	type = scan->FScaner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ '('", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'", lex, '\0');
	}

	K();
}

void dias::K()
{
/*
��������� ��������
				 -------
			----|	D	|---		�������� ������
		   |	 --------	|
		   |	 -------	|
		   |----|	I	|---|		�������� ������
		   |	 -------	|
		   |	 -------	|
		   |----|	M	|---|		
		   |     -------	|
		   |				|
		   \/				|
---  {  ------------------------  }  --->


*/
}
void dias::M()
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
		|		|				 -------	|		|
		 -------|-  return  ----|	Q	|---|-------|
				|				 -------	|
				|							|
				 ---------------------------
*/
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
������������
		 -------			 -------
 -------|	B	|---  =  ---|	Q	|----->
		 -------			 -------
		   ���				���������
		 
*/
	B();
	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TSave)
	{
		scan->PrintError("�������� ���� \"+\"", lex, '\0');
	}

	Q();
}


void dias::Q()
{
/*
���������
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
				�
*/
}
void dias::V()
{
/*
���������
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
�
		 -----  &  -----
		|				|
		\/	 -------	|
 -----------|	V	|----------->
			 -------
			���������
*/
}


void dias::W()
{
/*
���������
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
		    ���������
*/
}


void dias::X()
{
/*
���������
		 -----  +  -----
		|				|
		|-----  -  -----|
		|				|
		\/	 -------	|
 -----------|	Y	|----------->
			 -------
		    ���������
*/
}

void dias::Y()
{
/*
���������
		 -----  *  -----
		|				|
		|-----  /  -----|
		|				|
		|-----  %  -----|
		|				|
		\/	 -------	|
 -----------|	Z	|----------->
			 -------
			�� ������
*/

}
void dias::Z()
{
/*						
							 -------
						 ---|	B	|---
						|	 -------	|
						|				|
						|---   �1   ----|
	 ---  +  ---		|				|
	|			|		|---   �2   ----|		
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
����� �������
			   ���
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
���
		 -----  .  -----
		|				|
		|				|
		\/				|
--------------  a  ------------>
*/
}