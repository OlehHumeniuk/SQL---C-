#include "InsertTable.h"
#include "Checker.h"
#include "Hellper.h"
typedef  map<int, string> IS_map;
typedef IS_map::iterator IS_Iterator;
typedef  map<string, int> SI_map;
typedef SI_map::iterator SI_Iterator;

bool AnalizArgumentsOfComandInsertHellper(string pNameTable, int* NumberColumn, string* NameVariable, IS_Iterator lColumnslIterator, SI_Iterator lIterator, DatabaseInsert *pDatabaseInsert, SI_map TablesLinesInfo)
{
    std::cout << "You insert in table " << pNameTable << "; column number " << *NumberColumn << " has this value " << *NameVariable << endl;
    lIterator = TablesLinesInfo.find(pNameTable); 
    (*pDatabaseInsert).AddColumn(pNameTable, lColumnslIterator->second, *NameVariable, lIterator->second);
    (*NumberColumn)++;
    lColumnslIterator = next(lColumnslIterator);
    (*NameVariable).clear();
    return 1;
}
void AnalizArgumentsOfComandInsert(string pComand, string pNameTable, DatabaseInfo *pDatabaseInfo, DatabaseInsert *pDatabaseInsert, SI_map *TablesLinesInfo)
{
    int lNumberCurrentItemStringCommand = 0, NumberColumn = 1;
    string  NameVariable = ""; //то куда мы записываем имя таблицы
    bool NameColumn = 1, ColumnExistence = 0;
    IS_map DatabaseInsert, lDatabaseColumnsName, lColumns;
    SI_Iterator lIterator = TablesLinesInfo->find(pNameTable);
    DatabaseInfo DatabaseColumns;
    FindAllDatabaseColumns_1(pDatabaseInfo, pNameTable, &lColumns, &DatabaseColumns, lDatabaseColumnsName);//выводим все возможные колонки с нашим именем бд
    map<int, string> ::iterator lColumnslIterator = lColumns.begin();
    if (lIterator == TablesLinesInfo->end())
    {
        TablesLinesInfo->insert(make_pair(pNameTable, 1));
        lIterator = TablesLinesInfo->find(pNameTable);
    }
    else { lIterator->second++; }
    int lCounter = 1;
    while (pComand[lNumberCurrentItemStringCommand + 1])// пока не конец строки
    {
        if (pComand[lNumberCurrentItemStringCommand] != ' ' && NameColumn && pComand[lNumberCurrentItemStringCommand] != ',')
        {
            if (pComand[lNumberCurrentItemStringCommand] == ')')
            {
                std::cout << "We end read comand!" << endl;
                break;
            }
            if (pComand[lNumberCurrentItemStringCommand] != '"')
            {
                NameVariable = NameVariable + pComand[lNumberCurrentItemStringCommand];
                if (pComand[lNumberCurrentItemStringCommand + 1] == ' ' || pComand[lNumberCurrentItemStringCommand + 1] == ',' || pComand[lNumberCurrentItemStringCommand + 1] == ')' || pComand[lNumberCurrentItemStringCommand + 1] == '\"')
                {
                    if ((pComand[lNumberCurrentItemStringCommand + 1] == '\"') || (pComand[lNumberCurrentItemStringCommand + 1] == '\"' && (pComand[lNumberCurrentItemStringCommand + 2] == ',' || pComand[lNumberCurrentItemStringCommand + 2] == ')' || pComand[lNumberCurrentItemStringCommand + 3] == ',' || pComand[lNumberCurrentItemStringCommand + 3] == ')')))
                    {
                        bool ColumnExistence = AnalizArgumentsOfComandInsertHellper(pNameTable, &NumberColumn, &NameVariable, lColumnslIterator, lIterator, pDatabaseInsert, *TablesLinesInfo);
                        lColumnslIterator = next(lColumnslIterator);
                    }
                }
            }
        }
        lNumberCurrentItemStringCommand++;
    }
}
int InsertTableHellper(string pFullCommand, int pNumberCurrentItemStringForCheck, string pNameTable, DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert, SI_map *TablesLinesInfo)
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
                AnalizArgumentsOfComandInsert(lComand, pNameTable, pDatabaseInfo, pDatabaseInsert, TablesLinesInfo);
                if (pNameTable == "")
                {
                    std::cout << "Вы не указали имя таблицы куда будут записыватся данные!" << endl;
                }
                return 0;
            }
        }
        pNumberCurrentItemStringForCheck++;
    }
}
int InsertTable(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo* pDatabaseInfo, DatabaseInsert *pDatabaseInsert, SI_map *TablesLinesInfo)
{
    string  lNameTable = "", lComand;// раздичные переменные которые используем для проверки команды на правильную структуру
    int lNumberCurrentItemStringForCheck = 0;// индекс символов строки

    while (pFullCommand[pNumberCurrentItemString])                           // пока не конец буфера
    {
        bool lCheckLastSymphol = CheckForTheLastCharacter(pFullCommand);
        if (lCheckLastSymphol == 0) { return 0; }
        if (pFullCommand[pNumberCurrentItemString] != ' ')
        {
            lNameTable = lNameTable + pFullCommand[pNumberCurrentItemString];
            if (pFullCommand[pNumberCurrentItemString + 1] == ' ' || pFullCommand[pNumberCurrentItemString + 1] == '\0' || pFullCommand[pNumberCurrentItemString + 1] == '(' && !HellperChek(pFullCommand[pNumberCurrentItemString + 1]))
            {
                std::cout << "The name table in which you insert data is " << lNameTable << "!" << endl;
                bool lCheck = CheckForAvailability(lNameTable, *pDatabaseInfo);
                DatabaseCheckExist(lCheck, lNameTable);
                if (lCheck == 0) { break; }
                if (pFullCommand[pNumberCurrentItemString + 1] == '\0') { break; return 0; }
                if (pFullCommand[pNumberCurrentItemString + 1] == '(' || pFullCommand[pNumberCurrentItemString + 2] == '(')
                {
                    if (pFullCommand[pNumberCurrentItemString + 1] == '(')
                    {
                        lNumberCurrentItemStringForCheck = pNumberCurrentItemString + 2;
                        InsertTableHellper(pFullCommand, lNumberCurrentItemStringForCheck, lNameTable, pDatabaseInfo, pDatabaseInsert, TablesLinesInfo);
                        return 0;
                    }
                    else if (pFullCommand[pNumberCurrentItemString + 2] == '(')
                    {
                        lNumberCurrentItemStringForCheck = pNumberCurrentItemString + 3;
                        InsertTableHellper(pFullCommand, lNumberCurrentItemStringForCheck, lNameTable, pDatabaseInfo, pDatabaseInsert, TablesLinesInfo);
                        return 0;
                    }
                }
            }
        }
        pNumberCurrentItemString++;
    }
}