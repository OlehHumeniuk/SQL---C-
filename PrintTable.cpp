#include "PrintTable.h"
#include "Hellper.h"
#include "Checker.h"
//функция нахождения количества строк в бд
void LastLineInfo(DatabaseInsert* pDatabaseInsert, int* pLastLine)
{
    for (const Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
    {
        //cout << p->DatabaseName << " " << p->ColumnData << " " << p->CounterOfLine << endl;
        if (p->CounterOfLine >= *pLastLine)
        {
            *pLastLine = p->CounterOfLine;
        }
    }
}

//Поиск всехго того что мы вводили в БД
void FindAllDatabaseInsert(const DatabaseInsert* pDatabaseInsert, string pDatabaseName, DatabaseInsert* pDatabaseColumnsInsert, map<int, string> pDatabaseColumnsName, map<int, string> pColumns)
{
    int lCounter = 1;
    map<int, string> ::iterator lIterrator;
    map<int, string> ::iterator lIterratorColumns;
    lIterratorColumns = pColumns.begin();
    if (pDatabaseColumnsName.begin() == pDatabaseColumnsName.end())
    {
        for (const Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
        {
            if (p->TableName == pDatabaseName)
            {
                pDatabaseColumnsInsert->AddColumn(p->TableName, p->ColumnName, p->ColumnData, p->CounterOfLine);
                if (next(lIterratorColumns) == pColumns.end()) { lIterratorColumns = pColumns.begin(); }
                else { lIterratorColumns = next(lIterratorColumns); }
                lCounter++;
            }
        }
    }
    else
    {
        for (const Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
        {
            for (lIterrator = pDatabaseColumnsName.begin(); lIterrator != pDatabaseColumnsName.end(); lIterrator = next(lIterrator))
            {
                if (p->TableName == pDatabaseName && p->ColumnName == lIterrator->second)
                {
                    pDatabaseColumnsInsert->AddColumn(p->TableName, p->ColumnName, p->ColumnData, p->CounterOfLine);
                    lCounter++; break;
                }
            }
        }
    }
}
//Подсчет размера имен колонок в базе данных
void CountinSize(map<int, string>* pColumns, map<int, int>* pLengthColumn)
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
                if (lCounter == 1) { lLineLengthInfo_2.insert(make_pair(lCounterForDatabase, p->ColumnData.length())); }
                else { lLineLengthInfo.insert(make_pair(lCounterForDatabase, p->ColumnData.length())); }
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
        if (pLengthInfoIterator != --lLineLengthInfo_2.end())
        {
            pLengthInfoIterator = std::next(pLengthInfoIterator);
        }
        else { pLengthInfoIterator = lLineLengthInfo_2.begin(); }
    }
    *pLengthInfo = lLineLengthInfo_2;
}

//Функция сравнения долготы введенных значений в бд с значениями названий колонок бд
void CheckLength(map<int, int>* pLengthColumn, map<int, int>* pLengthInfo, map<int, int>* pMaxLengthInfo)
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
        if ((((lColumns.first - 1) % lMaxLengthInfo->size()) == 0) || lColumns.first == 1)
        {
            DatabaseColumn = "|";
        }
        DatabaseColumn = DatabaseColumn + LeftBordForColumns + lColumns.second + +"|";
        lAllDatabaseColumnsInsert = lAllDatabaseColumnsInsert + DatabaseColumn;
        DatabaseColumn.clear();
        LeftBordForColumns.clear();
        if (lColumns.first % lMaxLengthInfo->size() == 0)
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
void  OutDatabaseInsert(map<int, string>* lColumnsInfo, map<int, int>* pMaxLengthInfo, DatabaseInsert* pDatabaseInsert)
{
    int lCounter = 1;
    for (const Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
    {
        lColumnsInfo->insert(make_pair(lCounter, p->ColumnData));
        lCounter++;
    }
}

//Проверка на размер количество введенных данних
void CgeckingVeluesOfInsert(DatabaseInsert* pDatabaseInsert, int pQuontityColumns, bool* pValue)
{
    string DatabaseColumns = "";
    map<int, string> lLineInfo;
    int lLastLine = 0;
    int lCounterForDatabase = 1;
    LastLineInfo(pDatabaseInsert, &lLastLine);
    for (int lCounter = 1; lCounter <= lLastLine; lCounter++)
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
void PrintOurDatabase(DatabaseInfo* pDatabaseColumns, DatabaseInsert* pDatabaseInsert, map<int, string>* pColumns)
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
        string DatabaseName = pDatabaseColumns->begin()->TableName;
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

void PrintOurDatabaseHellper(DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert, string pDatabaseName, map<int, string> pDatabaseColumnsName)
{
    DatabaseInfo DatabaseColumns;
    DatabaseInsert lDatabaseInsertColumns;
    map<int, string> lColumns;
    FindAllDatabaseColumns_1(pDatabaseInfo, pDatabaseName, &lColumns, &DatabaseColumns, pDatabaseColumnsName);//выводим все возможные колонки с нашим именем бд
    std::cout << endl;
    FindAllDatabaseInsert(pDatabaseInsert, pDatabaseName, &lDatabaseInsertColumns, pDatabaseColumnsName, lColumns);
    for (const Node* p = (lDatabaseInsertColumns).begin(); p != NULL; p = p->next)
    {
        std::cout << p->TableName << "\t" << p->ColumnData << "\t" << p->CounterOfLine << endl;
    }
    const Node* p = lDatabaseInsertColumns.begin();
    if (p == NULL)
    {
        pDatabaseInfo->FindAllDatabaseColumns(*pDatabaseInfo, pDatabaseName);
        return;
    }
    std::cout << endl;
    PrintOurDatabase(&DatabaseColumns, &lDatabaseInsertColumns, &lColumns);
}

//функция вывода бд по имени
void PrintOutDatabaseInfoHellper(string lNameTable, bool lCheck, map<int, string> pDatabaseColumnsName, DatabaseInfo *gDatabaseInfo, DatabaseInsert *gDatabaseInsert)
{
    std::cout << "The name of your table is " << lNameTable << endl;
    lCheck = CheckForAvailability(lNameTable, *gDatabaseInfo);
    DatabaseCheckExist(lCheck, lNameTable); if (lCheck == 0) { return; }
    const Node* p = (*gDatabaseInsert).begin();
    if (p == NULL)
    {
        (*gDatabaseInfo).FindAllDatabaseColumns(*gDatabaseInfo, lNameTable);
    }
    else
    {
        PrintOurDatabaseHellper(gDatabaseInfo, gDatabaseInsert, lNameTable, pDatabaseColumnsName);
    }
}

//функция обработки команды принт
void PrintOutDatabaseInfo(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo *pDatabaseInfo, DatabaseInsert *pDatabaseInsert)
{
    string  lNameTable;    map<int, string> lDatabaseColumnsName;
    bool lCheck = 0;
    while (pFullCommand[pNumberCurrentItemString])                           // пока не конец буфера
    {
        CheckForTheLastCharacter(pFullCommand);
        if (pFullCommand[pNumberCurrentItemString] != ' ')
        {
            lNameTable = lNameTable + pFullCommand[pNumberCurrentItemString];
            if (pFullCommand[pNumberCurrentItemString + 1] == ' ' || pFullCommand[pNumberCurrentItemString + 1] == ';')
            {
                PrintOutDatabaseInfoHellper(lNameTable, lCheck, lDatabaseColumnsName, pDatabaseInfo, pDatabaseInsert);
                return;
            }
        }
        pNumberCurrentItemString++;
    }
}