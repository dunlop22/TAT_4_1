#include "Scaner.h"

LEX Keyword[MAX_KEYW] = {	"bool", "double", "if", "else", "main", "return", "true", "false", "class"};

int IndexKeyword[MAX_KEYW] = {	TBool, TDouble, TIf, TElse, TMain, TReturn, TTrue, TFalse, TClass};

void Scaner::PutUK(int i)
{
	uk = i;
}

int Scaner::GetUK()
{
	return uk;
}

//Печать ошибки
void Scaner::PrintError(string error, string a, char znak)
{
	if (a[0] == '\0')
	{
		cout << "Ошибка - " << error;
	}
	else
	{
		cout << "Ошибка - " << error << "- Неверный символ ";
	}
	if (znak != '\0')
	{
		cout << " (" << znak << ")";	//Вывод ошибочного символа
	}
}

int Scaner::FScaner(LEX lex)
{
	int i;          //текущая длина лексемы
	for (i = 0; i < MAX_LEX; i++)
	{
		lex[i] = 0;    //очистка поля лексемы
	}
	i = 0;

start:
	//все игнорируемые элементы:
	while (t[uk] == ' ' || t[uk] == '\n' || t[uk] == '\t')	//Незначащие элементы
	{
		uk++;
	}

	if (t[uk] == '/' && t[uk + 1] == '/')	//Однострочный комментарий
	{
		uk = uk + 2;
		while (t[uk] != '\n' && t[uk] != '\0')
		{
			uk++;
		}
		goto start;
	}

	if (t[uk] == '/' && t[uk + 1] == '*')	//Многострочный комментарий
	{
		uk = uk + 2;
		while ((t[uk] != '*' || t[uk + 1] != '/') && t[uk] != '\0')
		{
			uk++;
		}

		if (t[uk] == '*' && t[uk + 1] == '/')
		{
			uk = uk + 2;
		}

		goto start;
	}

	if (t[uk] == '\0')
	{
		lex[0] = '#';
		return TEnd;
	}

	if (t[uk] >= '0' && t[uk] <= '9')
	{
		lex[i++] = t[uk++];

		while (t[uk] >= '0' && t[uk] <= '9')
		{
			if (i < MAX_CONST - 1)
			{
				lex[i++] = t[uk++];
			}
			else
			{
				uk++;
			}
		}
		if (i == MAX_CONST - 1)
		{
			PrintError("Слишком длинная константа", lex, '\0');
			return TError;
		}

		if (t[uk] == '.')
		{
			lex[i++] = t[uk++];
			goto CONT;
		}

		return TConstInt;
	}
	else if (t[uk] >= 'a' && t[uk] <= 'z' || t[uk] >= 'A' && t[uk] <= 'Z')			//Идентификатор 
	{
		lex[i++] = t[uk++];

		while (t[uk] >= '0' && t[uk] <= '9' || t[uk] >= 'a' && t[uk] <= 'z' ||
			t[uk] >= 'A' && t[uk] <= 'Z' || t[uk] == '_')
		{
			if (i < MAX_LEX - 1)
			{
				lex[i++] = t[uk++];
			}
			else
			{
				uk++;
			}
		}

		int j;
		for (j = 0; j < MAX_KEYW; j++)
		{
			if (strcmp(lex, Keyword[j]) == 0)
			{
				return IndexKeyword[j];
			}
		}

		return TIdent;
	}
	else if (t[uk] == '.')
	{
		lex[i++] = t[uk++];
		if (t[uk + 1] >= '0' && t[uk + 1] <= '9')
		{
			lex[i++] = t[uk++];
			goto CONT;
		}
		return TTochka;
	}
	else if (t[uk] == ',')
	{
		lex[i++] = t[uk++];
		return TZapya;
	}
	else if (t[uk] == ';')
	{
		lex[i++] = t[uk++];
		return TTochkaZap;
	}
	else if (t[uk] == '(')
	{
		lex[i++] = t[uk++];
		return TLS;
	}
	else if (t[uk] == ')')
	{
		lex[i++] = t[uk++];
		return TRS;
	}
	else if (t[uk] == '{')
	{
		lex[i++] = t[uk++];
		return TFLS;
	}
	else if (t[uk] == '}')
	{
		lex[i++] = t[uk++];
		return TFRS;
	}
	else if (t[uk] == '+')
	{
		lex[i++] = t[uk++];
		return TPlus;
	}
	else if (t[uk] == '-')
	{
		lex[i++] = t[uk++];
		return TMinus;
	}
	else if (t[uk] == '*')
	{
		lex[i++] = t[uk++];
		return TMult;
	}
	else if (t[uk] == '/')
	{
		lex[i++] = t[uk++];
		return TDiv;
	}
	else if (t[uk] == '%')
	{
		lex[i++] = t[uk++];
		return TMod;
	}
	else if (t[uk] == '=')
	{
		lex[i++] = t[uk++];
		if (t[uk] == '=')
		{
			lex[i++] = t[uk++];
			return TEQ;
		}

		return TSave;
	}
	else if (t[uk] == '!' && t[uk + 1] == '=')
	{
		lex[i++] = t[uk++];
		lex[i++] = t[uk++];
		return TNEQ;
	}
	else if (t[uk] == '<')
	{
		lex[i++] = t[uk++];
		if (t[uk] == '=')
		{
			lex[i++] = t[uk++];
			return TLE;
		}
		return TLT;
	}
	else if (t[uk] == '>')
	{
		lex[i++] = t[uk++];
		if (t[uk] == '=')
		{
			lex[i++] = t[uk++];
			return TGE;
		}

		return TGT;
	}
	else
	{
		PrintError("Неверный символ", lex, t[uk]);
		uk++;
		return TError;
	}

	CONT:
	while (t[uk] >= '0' && t[uk] <= '9')
	{
		if (i < MAX_CONST - 1)
		{
			lex[i++] = t[uk++];
		}
		else
		{
			uk++;
		}
	}
	if (i == MAX_CONST - 1)
	{
		PrintError("Слишком длинная константа", lex, '\0');
		return TError;
	}

	return TConstFloat;
}

void Scaner::GetData(string FileName)
{
	ifstream file(FileName);
	char symb;

	if (!file.is_open())	//Проверка наличия файла
	{
		PrintError("Ошибка при чтении файла", "", '\0');
		exit(1);
	}
	else         //Удачное открытие файла
	{
		int i = 0;		//Счетчик
		while (!file.eof())		//Чтение файла до конца
		{
			file.get(symb);
			if (!file.eof()) t[i++] = symb;
			if (i >= MAX_TEXT - 1)		//Проверка размера
			{
				PrintError("Слишком большой размер исходного модуля", "", '\0');
				break;
			}
		}

		t[i] = '\0';
		file.close();	//Закрытие файла
	}
}

Scaner::Scaner(string FileName)
{
	GetData(FileName);	//Получение данных из исходного файла
	PutUK(0);
}
