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

void dias::S()
{
/*	���������
S = 
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
	int type;
	
	type = Look_Forward(1);

	while (type == TBool || type == TDouble || type == TIdent || type == TClass)
	{
		if (type == TClass)
		{
			I();		//�������� ������
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
					D();	//�������� ������
				}
				else
				{
					F();	//�������
				}
			}
		}
		type = Look_Forward(1);
	}
}

void dias::PrintTree()
{
	root->Print();
}

void dias::D()
{
/* �������� ������
D =
								-------------  ,  ------------------
		 ---  bool  ---			|									|
		|			   |       \/									|
--------|--  double  --|-----------  a  ---------------------------------  ;  ------>
		|			   |					|					|
		 -----  a  ----						|					|
											|	     -------	|
											 -  =  -|	Q	|---
													 -------
*/

	LEX lex;
	int type;
	LEX type_lex;

	type = scan->FScaner(type_lex);

	if (type != TBool && type != TDouble && type != TIdent)
	{
		scan->PrintError("�������� ���", type_lex, '\0');
	}

	DATA_TYPE semType = root->GetTypebyLex(type);

	do
	{
		type = scan->FScaner(lex);

		if (type != TIdent)
		{
			scan->PrintError("�������� �������������", lex, '\0');
		}

		Tree* v;

		//�������� �� ������������� ��������������
		//������ �� ���������
		if (semType == TYPE_OBJ_CL)
		{
			v = root->SemInclude(lex, ObjVar, semType, type_lex);
			Tree* cl = root->SemGetClass(type_lex);
			v->SetRightT(cl->GetRight());
		}
		else
		{
			v = root->SemInclude(lex, ObjVar, semType);
		}

		type = Look_Forward(1);

		if (type == TSave)
		{
			type = scan->FScaner(lex);

			DATA_TYPE valType;
			LEX className;
			Q(&valType, &className);
			root->TypeCastingAssign(semType, valType, type_lex, className);
			type = Look_Forward(1);
		}

	} while (type == TZapya);	//,

	type = scan->FScaner(lex);

	if (type != TTochkaZap)		//;
	{
		scan->PrintError("�������� ������ \";\"", lex, '\0');
	}
}


void dias::F()
/*	
������� =
										   ���
										 -------
									 ---|   a   |---
		 ---  bool  ---				|	 -------	|				��������� ��������	
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
		scan->PrintError("�������� ���", lex, '\0');
	}

	DATA_TYPE semType = root->GetTypebyLex(type);

	//�������� �������������� ������ �� �������������

	type = scan->FScaner(lex);
	if (type != TMain && type != TIdent)
	{
		scan->PrintError("��������� ��� �������", lex, '\0');
	}

	Tree* v = root->SemInclude(lex, ObjFunct, semType);

	type = scan->FScaner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ \"(\"", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ \")\"", lex, '\0');
	}

	K();
	root->SetCur(v);
}

void dias::K()
{
/*
��������� �������� =
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

	type = scan->FScaner(lex);

	if (type != TFLS)
	{
		scan->PrintError("�������� ������ '{'", lex, '\0');
	}

	Tree* v = root->SemNewLevel();


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
		scan->PrintError("�������� ������ \"}\"", lex, '\0');
	}

	root->SetCur(v);
}


void dias::M()
{
/*
�������� =
					��������� ��������
						 -------
		 ---------------|	K	|-------------------
		|				 -------					|
		|					if						|
		|				 -------					|
		|---------------|	N	|-------------------|
		|				 -------					|
		|				������������				|
--------|				 -------					|
		|		 -------|	O	|-----------		|
		|		|		 -------			|		|
		|		|		 return				|		|
		|		|		 -------          	|		|
		 -------|-------|	H	|-----------|-- ; --|
				|		 -------			|
				|							|
				|---------------------------|
				|							|
				|		 -------			|
				 -------|	P	|-----------
						 -------
*/
	LEX lex;
	int type;

	DATA_TYPE resType;
	LEX resTypeName = "";

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
		else if (type == TMain)
		{
			P(&resType, &resTypeName);
		}
		else if (type == TIdent)
		{
			int uk1 = scan->GetUK();
			int line = scan->Get_Number_Line();
			int pos = scan->Get_Position();

			B(&resType, &resTypeName);

			type = Look_Forward(1);

			scan->PutUK(uk1);
			scan->Set_Line_Number(line);
			scan->Set_Position(pos);

			if (type == TLS)
			{
				P(&resType, &resTypeName);
			}
			else
			{
				O();
			}
		}

		type = scan->FScaner(lex);

		if (type != TTochkaZap)
		{
			scan->PrintError("�������� ������ \";\"", lex, '\0');
		}
	}
}


void dias::H()
{
/*
return =
					���������
					 -------
------  return  ----|	Q	| ------>
					 -------
*/
	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TReturn)
	{
		scan->PrintError("�������� �������� \"return\"", lex, '\0');
	}

	DATA_TYPE ifType;
	LEX ifClassName;

	Q(&ifType, &ifClassName);

	Tree* funct = root->GetCur()->GetCurrentFunct();
	LEX functClassName;
	funct->GetClassName(functClassName);

	root->TypeCastingAssign(funct->GetType(), ifType, functClassName, ifClassName);
}


void dias::N()
{
/*
if =
																		 ��������
																		 -------
														 ----  else  ---|	M	|---
														|				 -------	|
						���������			��������	|							|
						 -------			 -------	|							|
-----  if  ---  (  -----|	Q	|---  )  ---|	M	|--------------------------------------->
						 -------			 -------
*/
	LEX lex;
	int type;

	type = scan->FScaner(lex);

	if (type != TIf)
	{
		scan->PrintError("�������� �������� \"if\"", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ \"(\"", lex, '\0');
	}

	DATA_TYPE ifType;
	LEX ifClassName;

	Q(&ifType, &ifClassName);

	root->TypeCastingAssign(TYPE_BOOL, ifType, ifClassName, ifClassName);

	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ \")\"", lex, '\0');
	}

	M();

	type = Look_Forward(1);

	if (type == TElse)
	{
		type = scan->FScaner(lex);
		M();
	}

}

void dias::O()
{
/*
������������ =
		 -------			 -------
 -------|	B	|---  =  ---|	Q	|----->
		 -------			 -------
		   ���				���������
*/
	LEX lex;
	int type;

	DATA_TYPE firstType;
	LEX firstClassName = "";

	B(&firstType, &firstClassName);

	type = scan->FScaner(lex);

	if (type != TSave)
	{
		scan->PrintError("�������� ���� \"=\"", lex, '\0');
	}

	DATA_TYPE valType;
	LEX valClassName = "";

	Q(&valType, &valClassName);

	root->TypeCastingAssign(firstType, valType, firstClassName, valClassName);
}

void dias::Q(DATA_TYPE* resType, LEX* resTypeName)
{
/*
��������� =
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

	DATA_TYPE secondType;
	LEX secondTypeName = "";

	R(resType, resTypeName);
	type = Look_Forward(1);

	while (type == TOR)
	{
		int znak = type - 40;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		R(&secondType, &secondTypeName);
		type = Look_Forward(1);

		*resType = root->TypeCasting(*resType, secondType, Operation_Name[znak]);
	}
}

void dias::R(DATA_TYPE* resType, LEX* resTypeName)
{
/*
XOR =
						�	
					 -------
				 ---|	U	|--  ^  ----
				|	 -------			|
	 -------	\/						|
----|	U	|------------------------------->
	 -------
		�
*/


	LEX lex;
	int type;

	DATA_TYPE secondType;
	LEX secondTypeName = "";

	U(resType, resTypeName);
	type = Look_Forward(1);

	while (type == TXOR)
	{
		int znak = type - 40;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		U(&secondType, &secondTypeName);
		type = Look_Forward(1);

		*resType = root->TypeCasting(*resType, secondType, Operation_Name[znak]);
	}
}

void dias::V(DATA_TYPE* resType, LEX* resTypeName)
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

	DATA_TYPE secondType;
	LEX secondTypeName = "";

	W(resType, resTypeName);
	type = Look_Forward(1);

	while (type == TEQ || type == TNEQ)
	{
		int znak = type - 40;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		W(&secondType, &secondTypeName);
		type = Look_Forward(1);

		root->TypeCasting(*resType, secondType, Operation_Name[znak]);
		*resType = TYPE_BOOL;
	}
}

void dias::U(DATA_TYPE* resType, LEX* resTypeName)
{
/*
� =
					���������	
					 -------
				 ---|	V	|--  &  ----
				|	 -------			|
	 -------	\/						|
----|	V	|------------------------------->
	 -------		 
	���������	
*/

	LEX lex;
	int type;

	DATA_TYPE secondType;
	LEX secondTypeName = "";

	V(resType, resTypeName);
	type = Look_Forward(1);

	while (type == TAnd)
	{
		int znak = type - 40;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		V(&secondType, &secondTypeName);
		root->CheckTypeBool(secondType);

		type = Look_Forward(1);

		*resType = root->TypeCasting(*resType, secondType, Operation_Name[znak]);
	}
}

void dias::W(DATA_TYPE* resType, LEX* resTypeName)
{
/*
��������� =
													 ----  >   -----
									���������		|				|
									 -------		|----  <   -----|
							 -------|	X	|-------|				|-------
							|		 -------		|----  <=  -----|		|
							|						|				|		|
							|						 ----  >=  -----		|
							|												|
			 -------		\/												|
 -----------|	X	|--------------------------------------------------------------->
			 -------
			���������
*/


	LEX lex;
	int type;

	DATA_TYPE secondType;
	LEX secondTypeName = "";

	X(resType, resTypeName);

	type = Look_Forward(1);

	while (type == TLT || type == TGT || type == TLE || type == TGE)
	{
		int znak = type - 40;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		X(&secondType, &secondTypeName);
		type = Look_Forward(1);

		root->TypeCasting(*resType, secondType, Operation_Name[znak]);
		*resType = TYPE_BOOL;
	}
}

void dias::X(DATA_TYPE* resType, LEX* resTypeName)
{
/*
��������� =
						���������
						 -------	---  +  ---
					 ---|	Y	|--|			|---
					|	 -------	---  -  ---		|
					|								|
	 -------		\/								|
 ---|	Y	|----------------------------------------->
	 -------
	���������
*/



	LEX lex;
	int type;

	DATA_TYPE secondType;
	LEX secondTypeName = "";

	Y(resType, resTypeName);

	type = Look_Forward(1);

	while (type == TPlus || type == TMinus)
	{
		int znak = type - 40;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		Y(&secondType, &secondTypeName);
		type = Look_Forward(1);

		*resType = root->TypeCasting(*resType, secondType, Operation_Name[znak]);
	}

}

void dias::Y(DATA_TYPE* resType, LEX* resTypeName)
{
/*
��������� =
						 �� ������	  ----  *  -----
						  -------	 |				|
					 ----|	 Z	 |---|----  /  -----|---
					|	  -------	 |				|	|
					|				  ----  %  -----	|
     -------		\/									|
----|	Z	|-------------------------------------------------->
	 -------
	�� ������
*/

	LEX lex;
	int type;

	DATA_TYPE secondType;
	LEX secondTypeName = "";

	Z(resType, resTypeName);

	type = Look_Forward(1);

	while (type == TMult || type == TDiv || type == TMod)
	{
		int znak = type - 40;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		Z(&secondType, &secondTypeName);

		if (type == TMod)
		{
			root->CheckTypeBool(secondType);
		}

		type = Look_Forward(1);

		*resType = root->TypeCasting(*resType, secondType, Operation_Name[znak]);
	}
}

void dias::Z(DATA_TYPE* resType, LEX* resTypeName)
{
/*			
�� ������ =
							   ���
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
						|---|	P	|---|	����� �������
						|	 -------	|
						|				|
						|	 -------	|
						 -(-|	Q	|-)-
							 -------
							���������
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

		Q(resType, resTypeName);

		type = scan->FScaner(lex);

		if (type != TRS)
		{
			scan->PrintError("�������� ������ \")\"", lex, '\0');
		}
	}
	else if (type == TMain)
	{
		P(resType, resTypeName);
	}
	else if (type == TIdent)
	{
		B(resType, resTypeName);

		type = Look_Forward(1);

		if (type == TLS)
		{
			P(resType, resTypeName);
		}
	}
	else
	{
		type = scan->FScaner(lex);

		if (type == TConstInt)
		{
			*resType = TYPE_DOUBLE;
		}
		else if (type == TConstFloat)
		{
			*resType = TYPE_DOUBLE;
		}
		else if (type == TTrue)
		{
			*resType = TYPE_BOOL;
		}
		else if (type == TFalse)
		{
			*resType = TYPE_BOOL;
		}
		else
		{
			scan->PrintError("��������� ������������ ���������", lex, '\0');
		}
	}
}

void dias::P(DATA_TYPE* resType, LEX* resTypeName)
{
/*
����� ������� =
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

	type = Look_Forward(1);

	if (type != TMain)
	{
		B(resType, resTypeName);
	}
	else
	{
		type = scan->FScaner(lex);

		Tree* funct = root->SemGetFunct(lex);
		*resType = funct->GetType();
	}
	
	type = scan->FScaner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ \"(\"", lex, '\0');
	}
	
	type = scan->FScaner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ \")\"", lex, '\0');
	}
}

void dias::B(DATA_TYPE* resType, LEX* resTypeName)
{
/*
��� =
			 ------  a  -----  .  ------
			|							|
			|							|
			\/							|
----  a  ------------------------------------->
*/

	LEX lex;
	int type;
	type = scan->FScaner(lex);

	if (type != TIdent)
	{
		scan->PrintError("�������� �������������", lex, '\0');
	}

	type = Look_Forward(1);

	//�������� �� ������������� ��������������, ����������� ����
	Tree* ident;
		
	if (type == TLS)
	{
		ident = root->SemGetFunct(lex);
	}
	else
	{
		ident = root->SemGetVar(lex);
	}

	*resType = ident->GetType();

	if (*resType == TYPE_OBJ_CL)
	{
		ident->GetClassName(*resTypeName);
	}


	while (type == TTochka)
	{
		if (*resType != TYPE_OBJ_CL)
		{
			scan->PrintError("������ �� �������� ����������� ������", lex, '\0');
		}

		type = scan->FScaner(lex);
		type = scan->FScaner(lex);

		if (type != TIdent)
		{
			scan->PrintError("�������� �������������", lex, '\0');
		}

		type = Look_Forward(1);

		if (type == TLS)
		{
			ident = ident->FindRightLeftFunct(lex);
		}
		else
		{
			ident = ident->FindRightLeftVar(lex);
		}

		*resType = ident->GetType();

		if (*resType == TYPE_OBJ_CL)
		{
			ident->GetClassName(*resTypeName);
		}
	}
}

void dias::I()
{
/*
�������� ������ =
											 -------
										 ---|	D	|--- 		�������� ������
										|	 -------	|
										|	 -------	|
										|---|	I	|---|		�������� ������
										|	 -------	|
										|	 -------	|
										|---|	F	|---|		�������
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
		scan->PrintError("��������� �������� ������", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TIdent)
	{
		scan->PrintError("�������� �������������", lex, '\0');
	}

	Tree* v = root->SemInclude(lex, ObjClass, NO_TYPE);

	type = scan->FScaner(lex);

	if (type != TFLS)
	{
		scan->PrintError("�������� ������ \"{\"", lex, '\0');
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
				scan->PrintError("�������� �������������", lex, '\0');
			}
		}
		type = Look_Forward(1);
	}

	type = scan->FScaner(lex);

	if (type != TFRS)
	{
		scan->PrintError("�������� ������ \"}\"", lex, '\0');
	}

	type = scan->FScaner(lex);

	if (type != TTochkaZap)
	{
		scan->PrintError("�������� ������ \";\"", lex, '\0');
	}

	root->SetCur(v);
}
