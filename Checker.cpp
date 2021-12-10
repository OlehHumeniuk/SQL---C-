#include "Checker.h"

//Набор вспомогательных функций которые успользуются во время анализа всех команд
//Проверка на последний символ ;
bool CheckForTheLastCharacter(string pFullCommand)
{
    char endch = pFullCommand[pFullCommand.size() - 2];
    if (endch != ';') { std::cout << "Вы не закончили команду с помощью символа ';'!" << endl; return 0; }
    else { return 1; }
}
//Прооверка на существование таблицы
void DatabaseCheckExist(bool lCheck, string pNameTable)
{
    if (lCheck == 1)
    {
        cout << "Таблицы с именем " << pNameTable << " уже существует!" << endl;
    }
    if (lCheck == 0)
    {
        cout << "Таблицы с именем " << pNameTable << " не существует!" << endl;
    }
}
//Проверка на то существует ли эта таблица!
bool CheckForAvailability(string  pNameTable, const DatabaseInfo& pDatabaseInfo)
{
    if (pDatabaseInfo.size() == 0) { return 0; }
    for (const Knot* p = pDatabaseInfo.begin(); p != NULL; p = p->next)
    {
        if (p->TableName == pNameTable) { return 1; }
        if (next(p) == next(pDatabaseInfo.end())) { return 0; }
    }
}
//проверка на символ, является ли он в диапазоне английского алфавита
bool HellperChek(char c)
{
    return isdigit(c) || ('a' <= c && c <= 'z');
}