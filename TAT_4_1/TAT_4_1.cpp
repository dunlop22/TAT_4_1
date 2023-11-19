#include <iostream>
#include "defs.h"
#include "Scaner.h"
#include "dias.h"

int main()
{
    setlocale(LC_ALL, "rus");   //Поддержка русского языка

    Scaner* scan;
    int type;
    LEX lex;

    scan = new Scaner("input.txt");   //Имя файла для чтения


    dias* diag = new dias(scan);
    diag->S();

    type = scan->FScaner(lex);


    if (type != TEnd)
    {
        scan->PrintError("Лишний текст в конце программы", lex, '\0');
    }
    else
    {
        printf("\n\nОшибки не найдены.\n\n");
    }

    printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
    diag->PrintTree();
    printf("\n\n");

    return 0;
}