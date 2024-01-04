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

	root->flagInterpret = true;
	root->flagReturn = false;
	
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

		if (root->flagInterpret)
		{
			printf("\n\n�������� ����������: %s", lex);
		}

		Tree* v;

		//�������� �� ������������� ��������������
		//����������� ���������
		if (semType == TYPE_OBJ_CL)
		{
			v = root->SemInclude(lex, ObjVar, semType, type_lex);
			Tree* cl = root->SemGetClass(type_lex);

			if (root->flagInterpret)
			{
				v->MakeClassCopy(cl);
			}
		}
		else
		{
			v = root->SemInclude(lex, ObjVar, semType);
		}

		type = Look_Forward(1);

		if (type == TSave)
		{
			type = scan->FScaner(lex);

			DataS value;
			Q(&value);

			if (root->flagInterpret)
			{
				root->TypeCastingAssign(semType, value, type_lex, value.className);
			}

			type = Look_Forward(1);

			if (root->flagInterpret)
			{
				if (v->GetType() == TYPE_DOUBLE)
				{
					v->GetValue()->DataAsDouble = value.dataValue.DataAsDouble;
				}
				else if (v->GetType() == TYPE_BOOL)
				{
					v->GetValue()->DataAsBool = value.dataValue.DataAsBool;
				}
			}
		}

		if (DEBUG)
		{
			printf("\n\n������������� ������\n\n");
			PrintTree();
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
	bool mainFunc = false;

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
	else if (type == TMain)
	{
		mainFunc = true;
	}

	printf("\n\n�������� �������: %s", lex);

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

	bool flagInterpretCopy = root->flagInterpret;

	if (flagInterpretCopy)
	{
		if (!mainFunc)
		{
			root->flagInterpret = false;
		}

		v->SetStart(scan->GetUK(), scan->Get_Number_Line(), scan->Get_Position());
	}

	K();

	root->flagInterpret = flagInterpretCopy;

	if (root->flagInterpret)
	{
		root->SetCur(v);
		v->CleanChild();
	}
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


	if (root->flagInterpret)
	{
		root->SetCur(v);

		v->CleanChild();

		if (DEBUG)
		{
			printf("\n\n������������ ������ - ����� ���������� ���������");

			printf("\n\n������������� ������\n\n");
			PrintTree();
		}
	}
	
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

	DataS res;

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
			P(&res);
		}
		else if (type == TIdent)
		{
			int uk1 = scan->GetUK();
			int line = scan->Get_Number_Line();
			int pos = scan->Get_Position();
			Tree* ident = NULL;

			B(&ident);

			if (root->flagInterpret)
			{
				res.dataType = ident->GetType();
				res.dataValue = *ident->GetValue();

				if (res.dataType == TYPE_OBJ_CL)
				{
					ident->GetClassName(res.className);
				}
			}

			type = Look_Forward(1);

			scan->PutUK(uk1);
			scan->Set_Line_Number(line);
			scan->Set_Position(pos);

			if (type == TLS)
			{
				P(&res);
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

	DataS ifData;

	Q(&ifData);

	if (root->flagInterpret)
	{
		Tree* funct = root->GetCur()->GetCurrentFunct();
		LEX functClassName;
		funct->GetClassName(functClassName);

		ifData = root->TypeCastingAssign(funct->GetType(), ifData, functClassName, ifData.className);

		if (funct->GetType() == TYPE_BOOL)
		{
			funct->GetValue()->DataAsBool = ifData.dataValue.DataAsBool;

		}
		else if (funct->GetType() == TYPE_DOUBLE)
		{
			funct->GetValue()->DataAsDouble = ifData.dataValue.DataAsDouble;
		}

		root->flagInterpret = false;
		root->flagReturn = true;
	}
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

	DataS ifData;

	Q(&ifData);

	root->TypeCastingAssign(TYPE_BOOL, ifData, ifData.className, ifData.className);

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
	Tree* ident = NULL;
	DataS firstData;

	B(&ident);

	if (root->flagInterpret)
	{
		firstData.dataType = ident->GetType();

		if (firstData.dataType == TYPE_OBJ_CL)
		{
			ident->GetClassName(firstData.className);
		}
	}

	type = scan->FScaner(lex);

	if (type != TSave)
	{
		scan->PrintError("�������� ���� \"=\"", lex, '\0');
	}

	DataS value;

	Q(&value);

	if (root->flagInterpret)
	{
		value = root->TypeCastingAssign(firstData.dataType, value, firstData.className, value.className);

		if (firstData.dataType == TYPE_BOOL)
		{
			ident->GetValue()->DataAsBool = value.dataValue.DataAsBool;

		}
		else if (firstData.dataType == TYPE_DOUBLE)
		{
			ident->GetValue()->DataAsDouble = value.dataValue.DataAsDouble;
		}
	}
}

void dias::Q(DataS* res)
{
/*
��������� =
					   XOR
					 -------
				 ---|	R	|--  ||  ----
				|	 -------			|
	 -------	\/						|
----|	R	|------------------------------->
	 -------
	   XOR
*/

	LEX lex;
	int type;

	DataS secondData;

	R(res);
	type = Look_Forward(1);

	if (type == TOR)
	{
		root->CheckTypeBool(res->dataType);
	}

	while (type == TOR)
	{
		int znak = type - 40;
		int operation = type;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		R(&secondData);
		root->CheckTypeBool(secondData.dataType);
		type = Look_Forward(1);

		root->TypeCasting(res, secondData, operation, Operation_Name[znak]);
	}
}

void dias::R(DataS* res)
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

	DataS secondData;

	U(res);
	type = Look_Forward(1);

	if (type == TOR)
	{
		root->CheckTypeBool(res->dataType);
	}

	while (type == TXOR)
	{
		int znak = type - 40;
		int operation = type;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		U(&secondData);
		root->CheckTypeBool(secondData.dataType);
		type = Look_Forward(1);

		root->TypeCasting(res, secondData, operation, Operation_Name[znak]);
	}
}

void dias::V(DataS* res)
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

	DataS secondData;
	

	W(res);
	type = Look_Forward(1);

	while (type == TEQ || type == TNEQ)
	{
		int znak = type - 40;
		int operation = type;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		W(&secondData);
		type = Look_Forward(1);

		root->TypeCasting(res, secondData, operation, Operation_Name[znak]);
	}
}

void dias::U(DataS* res)
{
/*
� =
					���������	
					 -------
				 ---|	V	|--  &&  ----
				|	 -------			|
	 -------	\/						|
----|	V	|------------------------------->
	 -------		 
	���������	
*/

	LEX lex;
	int type;

	DataS secondData;
	

	V(res);
	type = Look_Forward(1);

	if (type == TAnd)
	{
		root->CheckTypeBool(res->dataType);
	}

	while (type == TAnd)
	{
		int znak = type - 40;
		int operation = type;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		V(&secondData);
		root->CheckTypeBool(secondData.dataType);

		type = Look_Forward(1);

		root->TypeCasting(res, secondData, operation, Operation_Name[znak]);
	}
}

void dias::W(DataS* res)
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

	DataS secondData;
	

	X(res);

	type = Look_Forward(1);

	while (type == TLT || type == TGT || type == TLE || type == TGE)
	{
		int znak = type - 40;
		int operation = type;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		X(&secondData);
		type = Look_Forward(1);

		root->TypeCasting(res, secondData, operation, Operation_Name[znak]);
	}
}

void dias::X(DataS* res)
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

	DataS secondData;
	

	Y(res);

	type = Look_Forward(1);

	while (type == TPlus || type == TMinus)
	{
		int znak = type - 40;
		int operation = type;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		Y(&secondData);
		type = Look_Forward(1);

		root->TypeCasting(res, secondData, operation, Operation_Name[znak]);
	}

}

void dias::Y(DataS* res)
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

	DataS secondData;
	

	Z(res);

	type = Look_Forward(1);

	if (type == TMod)
	{
		root->CheckTypeBool(res->dataType);
	}

	while (type == TMult || type == TDiv || type == TMod)
	{
		int znak = type - 40;
		int operation = type;
		if (znak > 10) znak--;

		type = scan->FScaner(lex);
		Z(&secondData);

		if (type == TMod)
		{
			root->CheckTypeBool(secondData.dataType);
		}

		type = Look_Forward(1);

		root->TypeCasting(res, secondData, operation, Operation_Name[znak]);
	}
}

void dias::Z(DataS* res)
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

	bool minus = false;

	type = Look_Forward(1);

	if (type == TPlus || type == TMinus)
	{
		type = scan->FScaner(lex);

		if (type == TMinus)
		{
			minus = true;
		}
	}

	type = Look_Forward(1);

	if (type == TLS)
	{
		type = scan->FScaner(lex);

		Q(res);

		type = scan->FScaner(lex);

		if (type != TRS)
		{
			scan->PrintError("�������� ������ \")\"", lex, '\0');
		}
	}
	else if (type == TMain)
	{
		P(res);
	}
	else if (type == TIdent)
	{
		int uk1 = scan->GetUK();
		int line = scan->Get_Number_Line();
		int pos = scan->Get_Position();

		Tree* ident = NULL;

		B(&ident);

		if (root->flagInterpret)
		{
			res->dataType = ident->GetType();
			res->dataValue = *ident->GetValue();

			if (res->dataType == TYPE_OBJ_CL)
			{
				ident->GetClassName(res->className);
			}
		}

		type = Look_Forward(1);

		if (type == TLS)
		{
			scan->PutUK(uk1);
			scan->Set_Line_Number(line);
			scan->Set_Position(pos);

			P(res);
		}
	}
	else
	{
		type = scan->FScaner(lex);

		if (type == TConstInt)
		{
			res->dataType = TYPE_DOUBLE;
			res->dataValue.DataAsDouble = atoi(lex);
		}
		else if (type == TConstFloat)
		{
			res->dataType = TYPE_DOUBLE;
			res->dataValue.DataAsDouble = atof(lex);
		}
		else if (type == TTrue)
		{
			res->dataType = TYPE_BOOL;
			res->dataValue.DataAsBool = true;
		}
		else if (type == TFalse)
		{
			res->dataType = TYPE_BOOL;
			res->dataValue.DataAsBool = false;
		}
		else
		{
			scan->PrintError("��������� ������������ ���������", lex, '\0');
		}

	}

	if (minus)
	{
		if (res->dataType == TYPE_BOOL)
		{
			res->dataValue.DataAsBool = -res->dataValue.DataAsBool;
		}
		else
		{
			res->dataValue.DataAsDouble = -res->dataValue.DataAsDouble;
		}
	}
}

void dias::P(DataS* res)
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
	Tree* sfunct = NULL;

	type = Look_Forward(1);

	if (type != TMain)
	{
		B(&sfunct);
	}
	else
	{
		type = scan->FScaner(lex);
		sfunct = root->SemGetFunct(lex);
	}

	Tree* funct = root->SemInclude(sfunct);

	if (root->flagInterpret)
	{
		int uk = scan->GetUK();
		int line = scan->Get_Number_Line();
		int pos = scan->Get_Position();

		FStart fs = funct->GetStart();

		scan->PutUK(fs.uk);
		scan->Set_Line_Number(fs.line);
		scan->Set_Position(fs.pos);

		M();

		res->dataType = funct->GetType();

		if (res->dataType == TYPE_DOUBLE)
		{
			res->dataValue.DataAsDouble = funct->GetValue()->DataAsDouble;
		}
		else if (res->dataType == TYPE_BOOL)
		{
			res->dataValue.DataAsBool = funct->GetValue()->DataAsBool;
		}
		else if (res->dataType == TYPE_OBJ_CL)
		{
			funct->GetClassName(res->className);
		}

		scan->PutUK(uk);
		scan->Set_Line_Number(line);
		scan->Set_Position(pos);

		if (root->flagReturn)
		{
			root->flagReturn = false;
			root->flagInterpret = true;
			funct->CleanChild();
			root->SetCur(funct);
			root->Back();
		}
		else
		{
			scan->PrintError("����������� ������������ �������� �������");
		}
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

void dias::B(Tree** ident)
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
	if (root->flagInterpret)
	{
		if (type == TLS)
		{
			*ident = root->SemGetFunct(lex);
		}
		else
		{
			*ident = root->SemGetVar(lex);
		}
	}

	while (type == TTochka)
	{
		if (root->flagInterpret && (*ident)->GetType() != TYPE_OBJ_CL)
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

		if (root->flagInterpret)
		{
			if (type == TLS)
			{
				*ident = (*ident)->FindRightLeftFunct(lex);
			}
			else
			{
				*ident = (*ident)->FindRightLeftVar(lex);
			}
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

	printf("\n\n�������� ������: %s", lex);

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
