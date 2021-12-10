#include "SelectTable.h"
#include "PrintTable.h"
void ColumnChecker(map<int, string> pDatabaseColumnsName, string pTableName, const DatabaseInfo& pDatabaseInfo, bool* pCheck)
{
    map<int, string>::iterator lIterator;
    for (lIterator = pDatabaseColumnsName.begin(); lIterator != pDatabaseColumnsName.end(); lIterator = next(lIterator))
    {
        for (const Knot* p = pDatabaseInfo.begin(); p != NULL; p = p->next)
        {
            if ((p->TableName == pTableName) && (p->ColumnName == lIterator->second))
            {
                *pCheck = 1;
                break;
            }
        }
    }
}
void AnaliseFrom(string pAllComand, string* pComand, string* pTableName)
{
    string Word = "";
    int count = 0;

    if (pAllComand[0] != '\0')
    {
        if (pAllComand[0] != ' ') { ++count; }
        for (int i = 0; pAllComand[i] != '\0'; i++)
        {
            Word = Word + pAllComand[i];
            if (pAllComand[i + 1] == ' ' || pAllComand[i + 1] == '\0')
            {
                std::cout << Word << std::endl;
                if (count == 1) { *pComand = Word; }
                else { *pTableName = Word; break; }
                Word.clear();
                i++;
                ++count;
            }
        }
    }
    std::cout << "Count: " << count << '\n';
}
void AnaliseHelpper(string pFullCommand, int pNumberCurrentItemString, string* lFullCommand)
{
    for (pFullCommand[pNumberCurrentItemString]; pFullCommand[pNumberCurrentItemString] != ';'; pNumberCurrentItemString++)
    {
        *lFullCommand = *lFullCommand + pFullCommand[pNumberCurrentItemString];
    }
}
void AnalizArgumentsOfComandISelect(string pComand,DatabaseInfo *gDatabaseInfo , DatabaseInsert *gDatabaseInsert)
{
    string  NameVariable = ""; //то куда мы записываем имя столбца
    string lNameTable = ""; //то куда мы записываем имя столбца
    int NumberColumn = 1, lNCISC = 0;//lNumberCurrentItemStringCommand
    bool NameColumns = 1, pCheck = 0;
    map<int, string> DatabaseColumnsName;
    string  TypeOfData; //то куда мы записываем тип данных таблицы
    while (pComand[lNCISC + 1])// пока не конец строки
    {
        if (pComand[lNCISC + 1] == 'f' && pComand[lNCISC + 2] == 'r' && pComand[lNCISC + 3] == 'o' && pComand[lNCISC + 4] == 'm')
        {
            string lFullCommand = "", lComand = "";
            NumberColumn = 0;
            AnaliseHelpper(pComand, lNCISC + 6, &lNameTable);
            ColumnChecker(DatabaseColumnsName, lNameTable, *gDatabaseInfo, &pCheck);
            bool lCheckTableName = CheckForAvailability(lNameTable, *gDatabaseInfo);
            if (pCheck == 1)
            {
                PrintOutDatabaseInfoHellper(lNameTable, lCheckTableName, DatabaseColumnsName, gDatabaseInfo, gDatabaseInsert);
                return;
            }
        }
        if (pComand[lNCISC] != ' ' && pComand[lNCISC] != ',')
        {
            NameVariable = NameVariable + pComand[lNCISC];
            if (pComand[lNCISC + 1] == ' ' || pComand[lNCISC + 1] == ',')
            {
                if (NameColumns == 1) { DatabaseColumnsName.insert(make_pair(NumberColumn, NameVariable)); }
                NumberColumn++;
                NameVariable.clear();
            }
        }
        lNCISC++;
    }
}
void SelectTable(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo *gDatabaseInfo, DatabaseInsert *gDatabaseInsert)
{
    string  lNameTable = "";
    bool lCheck = 0;
    int lQuantityOfDatabaseElements = 0;
    int lNumberCurrentItemStringForCheck;                               // индекс символов строки
    string  lComand;// различные переменные которые используем для проверки команды на правильную структуру
    map<int, map<int, string>> lValuesComand;
    map<int, string> lDatabaseColumnsName;
    while (pFullCommand[pNumberCurrentItemString])                           // пока не конец буфера
    {
        bool lLastSymphol = CheckForTheLastCharacter(pFullCommand);
        if (lLastSymphol == 0) { return; }
        if (pFullCommand[pNumberCurrentItemString] != ' ')
        {
            map<int, string> lTemporaryMapForValues;
            if (pFullCommand[pNumberCurrentItemString] == '*' && pFullCommand[pNumberCurrentItemString + 1] == ' ')
            {
                string lFullCommand = "", lComand = "";
                AnaliseHelpper(pFullCommand, pNumberCurrentItemString + 2, &lFullCommand);
                AnaliseFrom(lFullCommand, &lComand, &lNameTable);
                std::regex twoormorespaces(" [ ]+");
                cout << "Наша команда после обработки: " << endl << std::regex_replace(lFullCommand, twoormorespaces, " ") << endl;
                if (lComand == "from") { PrintOutDatabaseInfoHellper(lNameTable, lCheck, lDatabaseColumnsName, gDatabaseInfo, gDatabaseInsert); }
                return;
            }
            else
            {
                int lCounter = pNumberCurrentItemString;
                string lFullCommand = "", lComand = "";
                for (pFullCommand[lCounter]; pFullCommand[lCounter] != '\0'; lCounter++)
                {
                    lFullCommand = lFullCommand + pFullCommand[lCounter];
                }
                AnalizArgumentsOfComandISelect(lFullCommand, gDatabaseInfo, gDatabaseInsert);
                return;
            }
        }
        pNumberCurrentItemString++;
    }
}