#include "dias.h"




void dias::S()//������
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

void dias::D()//������
{
	/* �������� ������
	* D =

			-------------  ,  ------------------
		   |									|
		   \/									|
	-����������-----------  a  ---------------------------------  ;  ------>
						|					|
						|					|
						|	     --------	|
						|-  =  --|	Q	|---|		�������
								 --------


	*/

	LEX lex;
	int type;
	int uk1;

	type = scan->FScaner(lex);

	if (type != TBool && type != TDouble && type != TIdent)
	{
		scan->PrintError("�������� ���", lex, '\0');
	}

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


void dias::F()//������
/*	�������
			���
		 -------
	  --|   B   |--
	 |	 -------   |				��������� ��������	
	 |			   |					 -------
-����������----|			   |---  (  ---  )  ----|   K   |---->
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
		scan->PrintError("�������� ���", lex, '\0');
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
		scan->PrintError("�������� ������ '('", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'", lex, '\0');
	}

	K();
}

void dias::K()//������
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
		   |----|	M	|---|		��������
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
		scan->PrintError("�������� ������ '{'", lex, '\0');
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
			scan->PutUK(uk1);
			M();
		}

		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	}

	if (type != TFRS)
	{
		scan->PrintError("�������� ������ '}'", lex, '\0');
	}

}


void dias::M()//������
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
			scan->PrintError("�������� ������ ';'", lex, '\0');
		}
	}
}


void dias::H()//������
{
/*
return
					 -------
------  return  ----|	Q	| ------>
					 -------
*/
	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TReturn)
	{
		scan->PrintError("�������� �������� return", lex, '\0');
	}

	Q();
}


void dias::N()//������
{
/*
if
																		 -------
														 ----  else  ---|	M	|---
														|				 -------	|
														|							|
						 -------			 -------	|							|
-----  if  ---  (  -----|	Q	|---  )  ---|	M	|--------------------------------------->
						 -------			 -------
*/
	LEX lex;
	int type;
	int uk1;

	type = scan->FScaner(lex);

	if (type != TIf)
	{
		scan->PrintError("�������� �������� if", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ '('", lex, '\0');
	}

	Q();

	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'", lex, '\0');
	}

	M();

	uk1 = scan->GetUK();
	type = scan->FScaner(lex);

	if (type != TElse)
	{
		scan->PutUK(uk1);
	}
	else
	{
		M();
	}

}

void dias::O()//������
{
/*
������������
		 -------			 -------
 -------|	B	|---  =  ---|	Q	|----->
		 -------			 -------
		   ���				���������
		 
*/
	LEX lex;
	int type;

	B();

	type = scan->FScaner(lex);

	if (type != TSave)
	{
		scan->PrintError("�������� ���� \"=\"", lex, '\0');
	}

	Q();
}


void dias::Q()//������
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

	LEX lex;
	int type;
	int uk1;

	do
	{
		R();
		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	} while (type == TOR);

	scan->PutUK(uk1);

}

void dias::R()//������
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

	LEX lex;
	int type;
	int uk1;

	do
	{
		U();
		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	} while (type == TXOR);

	scan->PutUK(uk1);
}

void dias::V()//������
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

	LEX lex;
	int type;
	int uk1;

	do
	{
		W();
		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	} while (type == TEQ || type == TNEQ);

	scan->PutUK(uk1);
}

void dias::U()//������
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

	LEX lex;
	int type;
	int uk1;

	do
	{
		V();
		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	} while (type == TAnd);

	scan->PutUK(uk1);
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
	LEX lex;
	int type;
	int uk1;

	do
	{
		X();
		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	} while (type == TLT || type == TGT || type == TLE || type == TGE);

	scan->PutUK(uk1);
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
	LEX lex;
	int type;
	int uk1;

	do
	{
		Y();
		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	} while (type == TPlus || type == TMinus);

	scan->PutUK(uk1);
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
	LEX lex;
	int type;
	int uk1;

	do
	{
		Z();
		uk1 = scan->GetUK();
		type = scan->FScaner(lex);
	} while (type == TMult || type == TDiv || type == TMod);

	scan->PutUK(uk1);
}


void dias::Z()//������
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

	LEX lex;
	int type;
	int uk1;

	uk1 = scan->GetUK();
	type = scan->FScaner(lex);

	if (type != TPlus && type != TMinus)
	{
		scan->PutUK(uk1);
	}

	uk1 = scan->GetUK();
	type = scan->FScaner(lex);

	if (type == TLS)
	{
		Q();

		type = scan->FScaner(lex);

		if (type != TRS)
		{
			scan->PrintError("�������� ������ ')'", lex, '\0');
		}
	}
	else if (type == TMain)
	{
		scan->PutUK(uk1);
		P();
	}
	else if (type == TIdent)
	{
		B();

		if (type == TLS)
		{
			scan->PutUK(uk1);
			P();
		}
	}
	else if (type != TConstInt && type != TConstFloat && type != TTrue && type != TFalse)
	{
		scan->PrintError("��������� ������������ ���������", lex, '\0');
	}
}





void dias::P()//������
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
	LEX lex;
	int type;
	int uk1;

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
		scan->PrintError("�������� ������ '('", lex, '\0');
	}
	
	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'", lex, '\0');
	}
}

void dias::B()//������
{
/*
���
		 -----  .  -----
		|				|
		|				|
		\/				|
--------------  a  ------------>
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
	} while (type == TTochka);

	scan->PutUK(uk1);
}

void dias::I()
{
	LEX lex;
	int type;
	int uk1;

	type = scan->FScaner(lex);

	if (type != TClass)
	{
		scan->PrintError("��������� �������� ������", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TIdent)
	{
		scan->PrintError("�������� �������������", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TFLS)
	{
		scan->PrintError("�������� ������ '{'", lex, '\0');
	}


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


	type = scan->FScaner(lex);

	if (type != TFRS)
	{
		scan->PrintError("�������� ������ '}'", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TTochkaZap)
	{
		scan->PrintError("�������� ������ ';'", lex, '\0');
	}
}
