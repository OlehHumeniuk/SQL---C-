#include "CreateTable.h"
#include "Checker.h"

bool AnalizArgumentsOfComandCreate(string pComand, string pNameTable, DatabaseInfo *pDatabaseInfo)
{
    int lNumberCurrentItemStringCommand = 0;
    string  NameVariable; //то куда мы записываем имя таблицы
    int NumberColumn = 1;
    bool NameColumn = 1;
    map<int, string> DatabaseColumnsName;
    map<int, string> DatabaseColumnsTypeOfData;

    string  TypeOfData; //то куда мы записываем тип данных таблицы
    while (pComand[lNumberCurrentItemStringCommand + 1])// пока не конец строки
    {
        if (pComand[lNumberCurrentItemStringCommand] != ' ' && NameColumn && pComand[lNumberCurrentItemStringCommand] != ',')
        {
            if (pComand[lNumberCurrentItemStringCommand] == ')')
            {
                std::cout << "we end read comand!" << endl;
                break;
            }
            NameVariable = NameVariable + pComand[lNumberCurrentItemStringCommand];
            if (pComand[lNumberCurrentItemStringCommand + 1] == ' ' || pComand[lNumberCurrentItemStringCommand + 1] == ',' || pComand[lNumberCurrentItemStringCommand + 1] == ')')
            {
                if (pComand[lNumberCurrentItemStringCommand + 1] == ' ')
                {
                    std::cout << "You create table " << pNameTable << " with column number " << NumberColumn << " has this name " << NameVariable;
                    DatabaseColumnsName.insert(make_pair(NumberColumn, NameVariable));
                    NumberColumn++;
                    NameColumn = 0;
                    NameVariable.clear();
                }
                else if (pComand[lNumberCurrentItemStringCommand + 1] == ',' || pComand[lNumberCurrentItemStringCommand + 1] == ')')
                {
                    std::cout << "You create table " << pNameTable << " with column number " << NumberColumn << " has this name " << NameVariable << endl;
                    DatabaseColumnsName.insert(make_pair(NumberColumn, NameVariable));
                    NumberColumn++;
                    NameVariable.clear();
                }
            }
        }
        else if (!NameColumn)
        {
            if (pComand[lNumberCurrentItemStringCommand] != ',' && pComand[lNumberCurrentItemStringCommand] != ' ')
            {
                TypeOfData = TypeOfData + pComand[lNumberCurrentItemStringCommand];
            }
            else if (pComand[lNumberCurrentItemStringCommand] == ',')
            {
                std::cout << " ,type of data is " << TypeOfData << endl;
                DatabaseColumnsTypeOfData.insert(make_pair(NumberColumn - 1, TypeOfData));
                NameColumn = 1;
                TypeOfData.clear();
            }
        }
        lNumberCurrentItemStringCommand++;
    }
    map<int, string> ::iterator DCTOfDIterator = DatabaseColumnsTypeOfData.begin();
    for (auto lDatabaseColumnsName : DatabaseColumnsName)
    {
        if (DCTOfDIterator != DatabaseColumnsTypeOfData.end())
        {
            (*pDatabaseInfo).AddColumn(pNameTable, lDatabaseColumnsName.second, DCTOfDIterator->second);
            DCTOfDIterator = next(DCTOfDIterator);
        }
        else if (DCTOfDIterator == DatabaseColumnsTypeOfData.end())
        {
            (*pDatabaseInfo).AddColumn(pNameTable, lDatabaseColumnsName.second, "");
        }
    }
    if (DatabaseColumnsName.size() != 0) { return 1; }
    else { return 0; }
}
void CreateTableHellper(string pFullCommand, int pNumberCurrentItemStringForCheck, string pNameTable, DatabaseInfo *pDatabaseInfo)
{
    string lComand = "";
    while (pFullCommand[pNumberCurrentItemStringForCheck])                           // пока не конец нашей команды
    {
        if (pFullCommand[pNumberCurrentItemStringForCheck] != ';')
        {
            lComand = lComand + pFullCommand[pNumberCurrentItemStringForCheck];
            if (pFullCommand[pNumberCurrentItemStringForCheck] == ')')
            {
                std::cout << lComand << endl;
                bool ColumnExistence = AnalizArgumentsOfComandCreate(lComand, pNameTable, pDatabaseInfo);
                if (ColumnExistence == 0)
                {
                    std::cout << "Вы не добавили никаких столбцов в таблицу!" << endl;
                }
                if (pNameTable == "")
                {
                    std::cout << "Вы не указали имя таблицы!" << endl;
                }
                return ;
            }
        }
        pNumberCurrentItemStringForCheck++;
    }
}
void CreateTable(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo *pDatabaseInfo)
{
    string  lNameTable;// раздичные переменные которые используем для проверки команды на правильную структуру
    int lNumberCurrentItemStringForCheck;// ето индекс символов строки

    while (pFullCommand[pNumberCurrentItemString])// пока не конец буфера
    {
        CheckForTheLastCharacter(pFullCommand);
        if (pFullCommand[pNumberCurrentItemString] != ' ')
        {
            lNameTable = lNameTable + pFullCommand[pNumberCurrentItemString];
            if (pFullCommand[pNumberCurrentItemString + 1] == ' ' || pFullCommand[pNumberCurrentItemString + 1] == '\0' || pFullCommand[pNumberCurrentItemString + 1] == '(' && !HellperChek(pFullCommand[pNumberCurrentItemString + 1]))
            {
                std::cout << "The name of your table is " << lNameTable << endl;
                bool lCheck = CheckForAvailability(lNameTable, *pDatabaseInfo);
                DatabaseCheckExist(lCheck, lNameTable);
                if (lCheck == 1) { break; }
                if (pFullCommand[pNumberCurrentItemString + 1] == '\0') { return; }
                if (pFullCommand[pNumberCurrentItemString + 1] == '(' || pFullCommand[pNumberCurrentItemString + 2] == '(')
                {
                    if (pFullCommand[pNumberCurrentItemString + 1] == '(')
                    {
                        lNumberCurrentItemStringForCheck = pNumberCurrentItemString + 2;
                        CreateTableHellper(pFullCommand, lNumberCurrentItemStringForCheck, lNameTable, pDatabaseInfo);
                        return;
                    }
                    else if (pFullCommand[pNumberCurrentItemString + 2] == '(')
                    {
                        lNumberCurrentItemStringForCheck = pNumberCurrentItemString + 3;
                        CreateTableHellper(pFullCommand, lNumberCurrentItemStringForCheck, lNameTable, pDatabaseInfo);
                        return;
                    }
                }
            }
        }
        pNumberCurrentItemString++;
    }
}