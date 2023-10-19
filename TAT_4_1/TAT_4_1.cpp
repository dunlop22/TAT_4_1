#include <iostream>
#include "defs.h"
#include "Scaner.h"

int main()
{
    setlocale(LC_ALL, "rus");   //Поддержка русского языка

    Scaner* scan;
    int type;
    LEX lex;

    scan = new Scaner("input.txt");   //Имя файла для чтения

    do 
    {
        type = scan->FScaner(lex);
        cout << lex << " - тип " << type << endl;
    } while (type != TEnd);

    return 0;
}