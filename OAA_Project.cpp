#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <vector>
#include <locale>
#include <string>
#include <algorithm>
#include<conio.h>
#include <stdio.h>
#include<windows.h>
#include <string.h>
#include<stdlib.h>
#include <memory>
#include <map>
#include <unordered_map>
#include <iterator>
#include <regex>
#include <sstream>
#include <cctype> 
#include <string_view>
#include <deque>
#include <list>
#include "DatabaseInfo.h"
#include "DatabaseInsert.h"

using namespace std;
typedef std::vector<char>     T_araay;
std::map<string, string> ColumInformation;
char buffer[255];
DatabaseInfo gDatabaseInfo;
DatabaseInsert gDatabaseInsert;
map <string, int> TablesLinesInfo;
//Печать всех значений всех бд которых мы добовляли
void PrintDatabaseColumn(std::ostream& _out, const DatabaseInfo& lDatabaseInfo) {
    for (const Knot* p = lDatabaseInfo.begin(); p != NULL; p = p->next)
        _out << p->DatabaseName << " " << p->DatabaseColumn << " " << p->DatabaseColumn << " " << p->ColumnTypeOfData << std::endl;
    _out << std::endl;
}

//функция нахождения количества строк в бд
void LastLineInfo(DatabaseInsert* pDatabaseInsert, int* pLastLine)
{
    for (const Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
    {
        cout << p->DatabaseName<< " " << p->ColumnData << " " << p->CounterOfLine<< endl;
        if (p->CounterOfLine >= *pLastLine)
        {
            *pLastLine = p->CounterOfLine;
        }
    }
}

//Поиск всех столбцов БД
void FindAllDatabaseColumns_1(const DatabaseInfo* pDatabaseInfo, string pDatabaseName, map<int, string>* pColumns, DatabaseInfo* pDatabaseColumns)
{
    //string DatabaseColumns = "";
    int lCounter = 1;
    DatabaseInfo lDatabaseColumns;
    for (const Knot* p = pDatabaseInfo->begin(); p != NULL; p = p->next)
    {
        if (p->DatabaseName == pDatabaseName)
        {
            pColumns->insert(make_pair(lCounter, p->DatabaseColumn));
            pDatabaseColumns->AddColumn(p->DatabaseName, p->DatabaseColumn, p->ColumnTypeOfData);
            //DatabaseColumns = DatabaseColumns + "|" + p->DatabaseColumn;
            lCounter++;
        }
    }
    //DatabaseColumns = DatabaseColumns + "|";
    for (auto& lColumns : *pColumns)
    {
        std::cout << lColumns.first << "\t" << lColumns.second << endl;
    }
}
//Поиск всехго того что мы вводили в БД
void FindAllDatabaseInsert(const DatabaseInsert* pDatabaseInsert, string pDatabaseName, DatabaseInsert* pDatabaseColumnsInsert)
{
    int lCounter = 1;
    for (const Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
    {
        if (p->DatabaseName == pDatabaseName)
        {
            pDatabaseColumnsInsert->AddColumn(p->DatabaseName, p->ColumnData, p->CounterOfLine);
            lCounter++;
        }
    }
}
//Подсчет размера имен колонок в базе данных
void CountinSize(map<int, string> *pColumns, map<int, int>* pLengthColumn)
{
    int lCounter = 1;
    for (auto& lColumns : *pColumns)
    {
        pLengthColumn->insert(make_pair(lCounter, lColumns.second.length()));
        lCounter++;
    }
}

//Функция получения долготы введенных значений в бд
void FindLength(DatabaseInsert* pDatabaseInsert, map<int, int>* pLengthInfo, int pQuontityColumns)
{
    map<int, int> lLineLengthInfo;
    map<int, int> lLineLengthInfo_2;
    map<int, int> lLineLengthInfo_Last;
    int lCounterForDatabase = 1;
    int lLastLine = 0;
    LastLineInfo(pDatabaseInsert, &lLastLine);

    for (int lCounter = 1; lCounter <= lLastLine; lCounter++)
    {
        for (const Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
        {
            if (p->CounterOfLine == lCounter)
            {
                if  (lCounter == 1){ lLineLengthInfo_2.insert(make_pair(lCounterForDatabase, p->ColumnData.length()));}
                else{lLineLengthInfo.insert(make_pair(lCounterForDatabase, p->ColumnData.length()));}
                lCounterForDatabase++;
            }
        }
    }
    map<int, int> ::iterator pLengthInfoIterator = lLineLengthInfo_2.begin();
    int iterValue = 0;
    for (auto pLineLengthInfo : lLineLengthInfo)
    {
        if (pLineLengthInfo.second > pLengthInfoIterator->second)
        {
            int lFirst = pLengthInfoIterator->first;
            lLineLengthInfo_2.erase(pLengthInfoIterator->first);
            lLineLengthInfo_2.insert(make_pair(lFirst, pLineLengthInfo.second));
            pLengthInfoIterator = lLineLengthInfo_2.find(lFirst);
            iterValue = pLengthInfoIterator->first;
        }
        if   (pLengthInfoIterator != --lLineLengthInfo_2.end())
        {pLengthInfoIterator = std::next(pLengthInfoIterator);}
        else {pLengthInfoIterator = lLineLengthInfo_2.begin();}
    }
    *pLengthInfo = lLineLengthInfo_2;
}

//Функция сравнения долготы введенных значений в бд с значениями названий колонок бд
void CheckLength(map<int, int> *pLengthColumn,map<int, int> *pLengthInfo, map<int, int>* pMaxLengthInfo)
{
    map<int, int> ::iterator lIteratorLengthInfo = pLengthInfo->begin();
    for (auto lLengthColumn : *pLengthColumn)
    {
        if (lLengthColumn.second >= lIteratorLengthInfo->second)
        {
            pMaxLengthInfo->insert(make_pair(lLengthColumn.first, lLengthColumn.second));
            lIteratorLengthInfo = std::next(lIteratorLengthInfo);
        }
        else 
        {
            pMaxLengthInfo->insert(make_pair(lLengthColumn.first, lIteratorLengthInfo->second));
            lIteratorLengthInfo = std::next(lIteratorLengthInfo);
        }
    }
}

//Получения строк с отступом
void RetreatForDatabaseColumns(map<int, string>* pColumns, map<int, int>* lMaxLengthInfo, string* pAllDatabaseColumns)
{
    string DatabaseColumn = "";
    string LeftBordForColumns = "";
    string RightBordForColumns = "";
    int lLenrthRetreat = 0;
    map<int, int> ::iterator lIteratorMaxLengthInfo = lMaxLengthInfo->begin();
    for (auto lColumns : *pColumns)
    {
        if ((lIteratorMaxLengthInfo->second % 2 == 1) && (lColumns.second.length() % 2 == 0))
        {
            lIteratorMaxLengthInfo->second = lIteratorMaxLengthInfo->second + 1;
        }
        int lLength = lIteratorMaxLengthInfo->second - lColumns.second.length();
        lLenrthRetreat = lLength / 2;
        for (int lCounetr = 1; lCounetr <= lLenrthRetreat; lCounetr++)
        {
            LeftBordForColumns = LeftBordForColumns + " ";
        }
        for (int lCounetr = 1; lCounetr <= (lLength - lLenrthRetreat); lCounetr++)
        {
            RightBordForColumns = RightBordForColumns + " ";
        }
        if (lColumns.first == 1)
        {
            DatabaseColumn = "|";
        }
        DatabaseColumn = DatabaseColumn + LeftBordForColumns + lColumns.second + RightBordForColumns + "|";
        *pAllDatabaseColumns = *pAllDatabaseColumns + DatabaseColumn;
        DatabaseColumn.clear();
        LeftBordForColumns.clear();
        RightBordForColumns.clear();
        lIteratorMaxLengthInfo = std::next(lIteratorMaxLengthInfo);
    }
    //cout << *pAllDatabaseColumns << endl;
}

//Получения строк с отступом для данных БД
void RetreatForDatabaseInfo(map<int, string>* pColumnsIndo, map<int, int>* lMaxLengthInfo, map<int, string>* pAllDatabaseColumnsInsert)
{
    string DatabaseColumn = "";
    string LeftBordForColumns = "";
    string lAllDatabaseColumnsInsert = "";
    map<int, string> lDatabaseInfo;
    int lCounter = 1;
    map<int, int> ::iterator lIteratorMaxLengthInfo = lMaxLengthInfo->begin();
    for (auto lColumns : *pColumnsIndo)
    {
            int lLength = lIteratorMaxLengthInfo->second - lColumns.second.length();
            for (int lCounetr = 1; lCounetr <= lLength; lCounetr++)
            {
                LeftBordForColumns = LeftBordForColumns + " ";
            }
            if ((((lColumns.first-1)  % lMaxLengthInfo->size()) == 0) || lColumns.first == 1)
            {
                DatabaseColumn = "|";
            }
        DatabaseColumn = DatabaseColumn + LeftBordForColumns + lColumns.second + +"|";
        lAllDatabaseColumnsInsert = lAllDatabaseColumnsInsert + DatabaseColumn;
        DatabaseColumn.clear();
        LeftBordForColumns.clear();
        if  (lColumns.first % lMaxLengthInfo->size() == 0)
        {
            lIteratorMaxLengthInfo = lMaxLengthInfo->begin();
            pAllDatabaseColumnsInsert->insert(make_pair(lCounter, lAllDatabaseColumnsInsert));
            lAllDatabaseColumnsInsert.clear();
            lCounter++;
        }
        else { lIteratorMaxLengthInfo = std::next(lIteratorMaxLengthInfo); }
    }
}

//Получаем карту значений нашей БД
void  OutDatabaseInsert(map<int, string>* lColumnsInfo, map<int, int>* pMaxLengthInfo, DatabaseInsert *pDatabaseInsert)
{
    int lCounter = 1;
    for (const Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
    {
        lColumnsInfo->insert(make_pair(lCounter, p->ColumnData));
        lCounter++;
    }
}

//Проверка на размер количество введенных данних
void CgeckingVeluesOfInsert(DatabaseInsert* pDatabaseInsert, int pQuontityColumns, bool * pValue)
{
    string DatabaseColumns = "";
    map<int, string> lLineInfo;
    int lLastLine = 0;
    int lCounterForDatabase = 1;
    LastLineInfo(pDatabaseInsert, &lLastLine);
    for (int lCounter = 1;lCounter<= lLastLine;lCounter++)
    {
        for (const Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
        {
            if (p->CounterOfLine == lCounter)
            {
                lLineInfo.insert(make_pair(lCounterForDatabase, p->ColumnData));
                lCounterForDatabase++;
            }
        }
        if (lLineInfo.size() != pQuontityColumns)
        {
            if (lLineInfo.size() < pQuontityColumns) { cout << "В строке под номером " << lCounter << " заполнено не все коноки! "; }
            else cout << "В строке под номером " << lCounter << " заполнено больше колонок чем существует! ";
            cout << " Вы ввели значения для " << lLineInfo.size() << " колонок, а у нас в бд существует " << pQuontityColumns << " колонок!" << endl;
            *pValue = 0;
        }
        lLineInfo.clear();
        lCounterForDatabase = 1;
    }
}

//Печать БД с колонками и строками (работает правильно)
void PrintOurDatabase(DatabaseInfo *pDatabaseColumns, DatabaseInsert *pDatabaseInsert, map<int, string>* pColumns)
{
    bool lValue = 1;
    string AllDatabaseColumns = "";
    string lLastLine = "";
    map<int, string> AllDatabaseColumnsInfo;
    while (lValue == 1)
    {
        map<int, int> lLengthColumn;
        map<int, int> lLengthInfo;
        map<int, int> lMaxLengthInfo;
        map<int, string> lColumnsInfo;
        map<int, string>::iterator lCheking = pColumns->begin();
        string DatabaseName = pDatabaseColumns->begin()->DatabaseName;
        CountinSize(pColumns, &lLengthColumn);
        CgeckingVeluesOfInsert(pDatabaseInsert, size(*pColumns), &lValue);
        if (lValue == 0) { break; }

        FindLength(pDatabaseInsert, &lLengthInfo, size(*pColumns));//находим размер наего Input
        CheckLength(&lLengthColumn, &lLengthInfo, &lMaxLengthInfo);//узнаем максимальный размер строки для того что бы знать сколько делать отступов
        RetreatForDatabaseColumns(pColumns, &lMaxLengthInfo, &AllDatabaseColumns);//Получаем строку с отступами и именами наших колонок
        pDatabaseColumns->ShowDatabase(AllDatabaseColumns, DatabaseName, &lLastLine);

        OutDatabaseInsert(&lColumnsInfo, &lMaxLengthInfo, pDatabaseInsert);//получаем карту значений нашей БД
        RetreatForDatabaseInfo(&lColumnsInfo, &lMaxLengthInfo, &AllDatabaseColumnsInfo);
        for (auto& lAllDatabaseColumnsInfo : AllDatabaseColumnsInfo)
        {
            pDatabaseInsert->CenterString(lAllDatabaseColumnsInfo.second, lAllDatabaseColumnsInfo.second.length());
        }
        cout << lLastLine;
        lValue = 0;
    }
}

void PrintOurDatabaseHellper(DatabaseInfo *pDatabaseInfo, DatabaseInsert *pDatabaseInsert, string pDatabaseName)
{
    DatabaseInfo DatabaseColumns;
    DatabaseInsert lDatabaseInsertColumns;
    map<int, string> lColumns;
    FindAllDatabaseColumns_1(pDatabaseInfo, pDatabaseName, &lColumns, &DatabaseColumns);//выводим все возможные колонки с нашим именем бд
    std::cout << endl;
    FindAllDatabaseInsert(pDatabaseInsert , pDatabaseName ,&lDatabaseInsertColumns);
    const Node* p = lDatabaseInsertColumns.begin();
    if (p == NULL)
    {
        pDatabaseInfo->FindAllDatabaseColumns(gDatabaseInfo, pDatabaseName);
        return;
    }
    std::cout << endl;
    PrintOurDatabase(&DatabaseColumns, &lDatabaseInsertColumns, &lColumns);
}

//////////////////////////////////////////////////////////////////////////////////////////
bool HellperChek(char c)
{
    return isdigit(c) || ('a' <= c && c <= 'z');
}
//Проверка на то существует ли эта таблица!
void CheckForAvailability(string  pNameTable, const DatabaseInfo& pDatabaseInfo, bool* pCheck)
{
    for (const Knot* p = pDatabaseInfo.begin(); p != NULL; p = p->next)
    {
        if (p->DatabaseName == pNameTable)
        {
            *pCheck = 1;
        }
    }
}
//функция вывода бд по имени
void PrintOutDatabaseInfoHellper(string lNameTable, bool lCheck)
{
    std::cout << "The name of your table is " << lNameTable << endl;
    CheckForAvailability(lNameTable, gDatabaseInfo, &lCheck);
    if (lCheck == 0)
    {
        cout << "Таблицы с именем " << lNameTable << " не существует!" << endl;
        return;
    }
    const Node* p = gDatabaseInsert.begin();
    if (p == NULL)
    {
        gDatabaseInfo.FindAllDatabaseColumns(gDatabaseInfo, lNameTable);
    }
    else
    {
        PrintOurDatabaseHellper(&gDatabaseInfo, &gDatabaseInsert, lNameTable);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void AnalizArgumentsOfComandCreate(string pComand, string pNameTable, bool* pColumnExistence)
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
                    *pColumnExistence = 1;
                }
                else if (pComand[lNumberCurrentItemStringCommand + 1] == ',' || pComand[lNumberCurrentItemStringCommand + 1] == ')')
                {
                    std::cout << "You create table " << pNameTable << " with column number " << NumberColumn << " has this name " << NameVariable << endl;
                    DatabaseColumnsName.insert(make_pair(NumberColumn, NameVariable));
                    NumberColumn++;
                    NameVariable.clear();
                    *pColumnExistence = 1;
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
                *pColumnExistence = 1;
            }
        }
        lNumberCurrentItemStringCommand++;
    }
    map<int, string> ::iterator DCTOfDIterator = DatabaseColumnsTypeOfData.begin();
    for (auto lDatabaseColumnsName : DatabaseColumnsName)
    {
        if (DCTOfDIterator != DatabaseColumnsTypeOfData.end())
        {
            gDatabaseInfo.AddColumn(pNameTable, lDatabaseColumnsName.second, DCTOfDIterator->second);
            DCTOfDIterator = next(DCTOfDIterator);
        }
        else if (DCTOfDIterator == DatabaseColumnsTypeOfData.end())
        {
            gDatabaseInfo.AddColumn(pNameTable, lDatabaseColumnsName.second, "");
        }
    }
}
int CreateTable(string pFullCommand, int pNumberCurrentItemString)
{
    bool ColumnExistence = 0;
    string  lNameTable;
    int lQuantityOfDatabaseElements = 0;
    int lNumberCurrentItemStringForCheck;                               // индекс символов строки
    string  lComand;// раздичные переменные которые используем для проверки команды на правильную структуру
    bool lCheck = 0;

    while (pFullCommand[pNumberCurrentItemString])                           // пока не конец буфера
    {
        char endch = pFullCommand[pFullCommand.size() - 2];
        if (endch != ';')
        {
            std::cout << "Вы не закончили команду с помощью символа ';'!" << endl;
        }
        if (pFullCommand[pNumberCurrentItemString] != ' ')
        {
            lNameTable = lNameTable + pFullCommand[pNumberCurrentItemString];
            if (pFullCommand[pNumberCurrentItemString + 1] == ' ' || pFullCommand[pNumberCurrentItemString + 1] == '\0' || pFullCommand[pNumberCurrentItemString + 1] == '(' && !HellperChek(pFullCommand[pNumberCurrentItemString + 1]))
            {
                std::cout << "The name of your table is " << lNameTable << endl;
                CheckForAvailability(lNameTable, gDatabaseInfo, &lCheck);
                if (lCheck == 1)
                {
                    cout << "Таблицы с именем " << lNameTable << " уже существует!" << endl;
                    break;
                }
                lQuantityOfDatabaseElements++;

                if (pFullCommand[pNumberCurrentItemString + 1] == '\0')
                {
                    return 0;
                }
                if (pFullCommand[pNumberCurrentItemString + 1] == '(' || pFullCommand[pNumberCurrentItemString + 2] == '(')
                {
                    if (pFullCommand[pNumberCurrentItemString + 1] == '(')
                    {
                        lNumberCurrentItemStringForCheck = pNumberCurrentItemString + 2;
                        while (pFullCommand[lNumberCurrentItemStringForCheck])                           // пока не конец нашей команды
                        {
                            if (pFullCommand[lNumberCurrentItemStringForCheck] != ';')
                            {
                                lComand = lComand + pFullCommand[lNumberCurrentItemStringForCheck];
                                if (pFullCommand[lNumberCurrentItemStringForCheck] == ')')
                                {
                                    std::cout << lComand << endl;
                                    AnalizArgumentsOfComandCreate(lComand, lNameTable, &ColumnExistence);
                                    if (ColumnExistence == 0)
                                    {
                                        std::cout << "Вы не добавили никаких столбцов в таблицу!" << endl;
                                    }
                                    if (lNameTable == "")
                                    {
                                        std::cout << "Вы не указали имя таблицы!" << endl;
                                    }
                                    return 0;
                                }
                            }
                            lNumberCurrentItemStringForCheck++;
                        }
                    }
                    else if (pFullCommand[pNumberCurrentItemString + 2] == '(')
                    {
                        lNumberCurrentItemStringForCheck = pNumberCurrentItemString + 3;
                        while (pFullCommand[lNumberCurrentItemStringForCheck])                           // пока не конец нашей команды
                        {
                            if (pFullCommand[lNumberCurrentItemStringForCheck] != ';')
                            {
                                lComand = lComand + pFullCommand[lNumberCurrentItemStringForCheck];
                                if (pFullCommand[lNumberCurrentItemStringForCheck] == ')')
                                {
                                    std::cout << lComand << endl;
                                    AnalizArgumentsOfComandCreate(lComand, lNameTable, &ColumnExistence);
                                    if (ColumnExistence == 0)
                                    {
                                        std::cout << "Вы не добавили никаких столбцов в таблицу!" << endl;
                                    }
                                    if (lNameTable == "")
                                    {
                                        std::cout << "Вы не указали имя таблицы!" << endl;
                                    }
                                    return 0;
                                }
                            }
                            lNumberCurrentItemStringForCheck++;
                        }
                    }
                }
            }
        }
        pNumberCurrentItemString++;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void AnalizArgumentsOfComandInsert(string pComand, string pNameTable)
{
    int lNumberCurrentItemStringCommand = 0;
    string  NameVariable = ""; //то куда мы записываем имя таблицы
    int NumberColumn = 1;
    bool NameColumn = 1, ColumnExistence = 0;
    map<int, string> DatabaseInsert;
    map<string, int> :: iterator lIterator = TablesLinesInfo.find(pNameTable);
    if (lIterator == TablesLinesInfo.end())
    {
        TablesLinesInfo.insert(make_pair(pNameTable, 1));
        lIterator = TablesLinesInfo.find(pNameTable);
    }
    else
    {
        lIterator->second++;
    }
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
                    if (pComand[lNumberCurrentItemStringCommand + 1] == '\"')
                    {
                        std::cout << "You insert in table " << pNameTable << "; column number " << NumberColumn << " has this value " << NameVariable << endl;
                        lIterator = TablesLinesInfo.find(pNameTable);
                        gDatabaseInsert.AddColumn(pNameTable, NameVariable, lIterator->second);
                        NumberColumn++;
                        NameVariable.clear();
                        ColumnExistence = 1;
                    }
                    else if (pComand[lNumberCurrentItemStringCommand + 1] == '\"' && (pComand[lNumberCurrentItemStringCommand + 2] == ',' || pComand[lNumberCurrentItemStringCommand + 2] == ')' || pComand[lNumberCurrentItemStringCommand + 3] == ',' || pComand[lNumberCurrentItemStringCommand + 3] == ')'))
                    {
                        std::cout << "You insert in table " << pNameTable << "; column number " << NumberColumn << " has this value " << NameVariable << endl;
                        lIterator = TablesLinesInfo.find(pNameTable);
                        gDatabaseInsert.AddColumn(pNameTable, NameVariable, lIterator->second);
                        NumberColumn++;
                        NameVariable.clear();
                        ColumnExistence = 1;
                    }
                }
            }
        }
        lNumberCurrentItemStringCommand++;
    }
}
int InsertTable(string pFullCommand, int pNumberCurrentItemString)
{
    string  lNameTable = "";
    bool lCheck = 0;
    int lQuantityOfDatabaseElements = 0;
    int lNumberCurrentItemStringForCheck;                               // индекс символов строки
    string  lComand;// раздичные переменные которые используем для проверки команды на правильную структуру

    while (pFullCommand[pNumberCurrentItemString])                           // пока не конец буфера
    {
        char endch = pFullCommand[pFullCommand.size() - 2];
        if (endch != ';')
        {
            std::cout << "Вы не закончили команду с помощью символа ';'!" << endl;
        }
        if (pFullCommand[pNumberCurrentItemString] != ' ')
        {
            lNameTable = lNameTable + pFullCommand[pNumberCurrentItemString];
            if (pFullCommand[pNumberCurrentItemString + 1] == ' ' || pFullCommand[pNumberCurrentItemString + 1] == '\0' || pFullCommand[pNumberCurrentItemString + 1] == '(' && !HellperChek(pFullCommand[pNumberCurrentItemString + 1]))
            {
                std::cout << "The name table in which you insert data is " << lNameTable << endl;
                CheckForAvailability(lNameTable, gDatabaseInfo,&lCheck);
                if (lCheck == 0)
                {
                    cout << "Таблицы с именем "<< lNameTable << " не существует!"<<endl;
                    break;
                }
                lQuantityOfDatabaseElements++;

                if (pFullCommand[pNumberCurrentItemString + 1] == '\0')
                {
                    return 0;
                }
                if (pFullCommand[pNumberCurrentItemString + 1] == '(' || pFullCommand[pNumberCurrentItemString + 2] == '(')
                {
                    if (pFullCommand[pNumberCurrentItemString + 1] == '(')
                    {
                        lNumberCurrentItemStringForCheck = pNumberCurrentItemString + 2;
                        while (pFullCommand[lNumberCurrentItemStringForCheck])                           // пока не конец нашей команды
                        {
                            if (pFullCommand[lNumberCurrentItemStringForCheck] != ';')
                            {
                                lComand = lComand + pFullCommand[lNumberCurrentItemStringForCheck];
                                if (pFullCommand[lNumberCurrentItemStringForCheck] == ')')
                                {
                                    std::cout << lComand << endl;
                                    AnalizArgumentsOfComandInsert(lComand, lNameTable);
                                    if (lNameTable == "")
                                    {
                                        std::cout << "Вы не указали имя таблицы куда будут записыватся данные!" << endl;
                                    }
                                    return 0;
                                }
                            }
                            lNumberCurrentItemStringForCheck++;
                        }
                    }
                    else if (pFullCommand[pNumberCurrentItemString + 2] == '(')
                    {
                        lNumberCurrentItemStringForCheck = pNumberCurrentItemString + 3;
                        while (pFullCommand[lNumberCurrentItemStringForCheck])                           // пока не конец нашей команды
                        {
                            if (pFullCommand[lNumberCurrentItemStringForCheck] != ';')
                            {
                                lComand = lComand + pFullCommand[lNumberCurrentItemStringForCheck];
                                if (pFullCommand[lNumberCurrentItemStringForCheck] == ')')
                                {
                                    std::cout << lComand << endl;
                                    AnalizArgumentsOfComandInsert(lComand, lNameTable);
                                    if (lNameTable == "")
                                    {
                                        std::cout << "Вы не указали имя таблицы куда будут записыватся данные!" << endl;
                                    }
                                    return 0;
                                }
                            }
                            lNumberCurrentItemStringForCheck++;
                        }
                    }
                }
            }
        }
        pNumberCurrentItemString++;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void AnaliseFrom(string pAllComand, string* pComand, string* pTableName)
{
    string Word = "";
    int count = 0;

    if (pAllComand[0] != '\0')
    {
        if (pAllComand[0] != ' ')
        {
            ++count;
        }
        for (int i = 0; pAllComand[i] != '\0'; i++)
        {
            Word = Word + pAllComand[i];
            if (pAllComand[i + 1] == ' ' || pAllComand[i + 1] == '\0')
            {
                std::cout << Word << std::endl;
                if (count == 1)
                {
                    *pComand = Word;
                }
                else
                {
                    *pTableName = Word;
                    break;
                }
                Word.clear();
                i++;
                ++count;
            }
        }
    }
    std::cout << "Count: " << count << '\n';
}
void SelectTable(string pFullCommand, int pNumberCurrentItemString)
{
    string  lNameTable = "";
    bool lCheck = 0;
    int lQuantityOfDatabaseElements = 0;
    int lNumberCurrentItemStringForCheck;                               // индекс символов строки
    string  lComand;// раздичные переменные которые используем для проверки команды на правильную структуру

    while (pFullCommand[pNumberCurrentItemString])                           // пока не конец буфера
    {
        char endch = pFullCommand[pFullCommand.size() - 2];
        if (endch != ';')
        {
            std::cout << "Вы не закончили команду с помощью символа ';'!" << endl;
        }
        if (pFullCommand[pNumberCurrentItemString] != ' ')
        {
            if (pFullCommand[pNumberCurrentItemString] == '*' && pFullCommand[pNumberCurrentItemString + 1] == ' ')
            {
                int lCounter = pNumberCurrentItemString + 2;
                string lFullCommand = "", lComand = "";
                for (pFullCommand[lCounter]; pFullCommand[lCounter] != ';'; lCounter++)
                {
                    lFullCommand = lFullCommand + pFullCommand[lCounter];
                }
                AnaliseFrom(lFullCommand, &lComand, &lNameTable);
                std::regex twoormorespaces(" [ ]+");
                cout << "Наша команда после обработки: " << endl << std::regex_replace(lFullCommand, twoormorespaces, " ") << endl;
                if (lComand == "from")
                {
                    PrintOutDatabaseInfoHellper(lNameTable, lCheck);
                }
            }
        }
        pNumberCurrentItemString++;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void PrintOutDatabaseInfo(string pFullCommand, int pNumberCurrentItemString)
{
    string  lNameTable;
    bool lCheck = 0;
    while (pFullCommand[pNumberCurrentItemString])                           // пока не конец буфера
    {
        char endch = pFullCommand[pFullCommand.size() - 2];
        if (endch != ';')
        {
            std::cout << "Вы не закончили команду с помощью символа ';'!" << endl;
        }
        if (pFullCommand[pNumberCurrentItemString] != ' ')
        {
            lNameTable = lNameTable + pFullCommand[pNumberCurrentItemString];
            if (pFullCommand[pNumberCurrentItemString + 1] == ' ' || pFullCommand[pNumberCurrentItemString + 1] == ';')
            {
                PrintOutDatabaseInfoHellper(lNameTable, lCheck);
            }
        }
        pNumberCurrentItemString++;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void check(string pComandString, string pFullCommand, int pNumberCurrentItemString)
{
    // Массив строк типа string
    // 1. Инициализация массива строк указанием размера массива
    const int lNumberOfCommands = 5;
    string lCommands[lNumberOfCommands] = { "create", "insert", "select","delete","print" };

    for (int pNumberOfCommands = 0; pNumberOfCommands < lNumberOfCommands; pNumberOfCommands++) // Проверка на то являеться ли полученая строка одной из команд. если да то выполняем действия етой команды
    {
        if (pComandString == lCommands[0])//команда Create
        {
            cout << "Create your table" << endl;
            CreateTable(pFullCommand, pNumberCurrentItemString);
            break;
            return;
        }
        else if (pComandString == lCommands[1])//команда Insert
        {
            cout << "Insert into your table!" << endl;
            InsertTable(pFullCommand, pNumberCurrentItemString);
            break;
            return;
        }
        else if (pComandString == lCommands[2])
        {
            cout << "Select your values!" << endl;
            SelectTable(pFullCommand, pNumberCurrentItemString);
            break;
            return;
        }
        else if (pComandString == lCommands[3])
        {
            break;
            return;
        }
        else if (pComandString == lCommands[4])
        {
            PrintOutDatabaseInfo(pFullCommand, pNumberCurrentItemString);
            break;
            return;
        }
    }
}
//Анализируем команды 
void AnaliseComand(char buffer[255])//функцыя анализа нашей строки
{
    string lFullCommand = "", pFullCommand = "", lTolowerComand;
    int lNumberCurrentItemString = 0, pNumberCurrentItem = 0, pIndex = 0;
    char lCurrentItem;              // поточный елемент в строке
    char lTolowerCommandElementChar;// перменная в которую мы записиваем елемент команды
    string lTolowerCommand;         // Строка в которую записиваем значения переведенной команды

    while (buffer[lNumberCurrentItemString] && true)                           // пока не конец буфера проверяем нашу строку на возможные команды
    {
        if (buffer[lNumberCurrentItemString] != ' ')//пока не встретим пробел записуем елемент в строку и потом проверяем является ли ето одной из возможных команд
        {
            lTolowerComand = lTolowerComand + buffer[lNumberCurrentItemString];
            if (buffer[lNumberCurrentItemString + 1] == ' ')
            {
                for (int index = 0; index < lTolowerComand.length(); ++index)         // меняем регистр именно для команды
                {
                    //в окрему функцію яка робить щось
                    while (lTolowerComand[pNumberCurrentItem])                           // пока не конец строки в которую записали команду
                    {
                        lCurrentItem = lTolowerComand[pNumberCurrentItem];
                        lTolowerCommandElementChar = tolower(lCurrentItem);
                        lTolowerCommand = lTolowerCommand + lTolowerCommandElementChar;//переводим всю строку в маленькие буквы для 100% коректного понимания програмы того что мы ввели
                        pNumberCurrentItem++;
                        if (pNumberCurrentItem == lTolowerComand.length())
                        {
                            lCurrentItem = lTolowerComand[pNumberCurrentItem];
                            lTolowerCommandElementChar = tolower(lCurrentItem);
                            lFullCommand = lTolowerCommand; // переводим команду в маленькие буквы для 100 % коректного понимания програмы того что мы ввели
                            std::cout << lTolowerCommand << endl;
                            pIndex = lNumberCurrentItemString + 1;
                            break;
                        }
                    }
                    if (pNumberCurrentItem == lTolowerComand.length()) break;
                }
                for (pIndex; pIndex <= strlen(buffer); pIndex++)
                {
                    pFullCommand = pFullCommand + buffer[pIndex];
                }
                lNumberCurrentItemString++;
                std::regex twoormorespaces(" [ ]+");
                lFullCommand = lFullCommand + " " + pFullCommand;
                cout << "Наша команда после обработки: " << endl << std::regex_replace(lFullCommand, twoormorespaces, " ") << endl;
                lNumberCurrentItemString = size(lTolowerCommand);
                check(lTolowerCommand, lFullCommand, lNumberCurrentItemString);//добавить к толовер команд значения что идут после в буфере, что бы оно могло норм обработать их
                break;
            }
        }
        lNumberCurrentItemString++;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////


int main()
{
    SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывод
    system("color 0A");// устанливаем цвет нашей консольки(а почему бы и нет?))
    setlocale(LC_ALL, "ru");//подключаем русский язык 

    //Vivod();

    // Просим пользователя ввести строку
    bool lBoolValue = 1;
    while (lBoolValue == 1)
    {
        char buffer[255];
        std::cout << "Enter a string: ";
        std::cin.getline(buffer, 255);//записиваем то что мы вводили в командную строку в буфер
        AnaliseComand(buffer);//анализируем наш буфер
        memset(buffer, 0, 255);
    }
}


