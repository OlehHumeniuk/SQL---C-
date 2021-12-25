#include "PrintTable.h"
#include "Hellper.h"
#include "Checker.h"
//функция нахождения количества строк в таблице
int LastLineInfo(DatabaseInsert* pDatabaseInsert)
{
    Node* p = pDatabaseInsert->begin();
    map<int, DatabaseString> lDatabaseStringMap = p->DatabaseStringMap;
    map<int, DatabaseString>::iterator lIterator = prev(lDatabaseStringMap.end());
    return lIterator->first;
}

//Поиск всехго того что мы вводили в БД
void FindAllDatabaseInsert(DatabaseInsert* pDatabaseInsert, map<int, string> pTableName, DatabaseInsert* pDatabaseColumnsInsert, map<string, string> lCondition, string ConditionSign, string pChecker)
{
    map<int, DatabaseString> lNeeded_1, lNeeded_2;
    if (lCondition.size() == 0 && ConditionSign.size() == 0)
    {
        for (Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
        {
            if (p->TableName == pTableName.begin()->second)
            {
                for (map<int, DatabaseString>::iterator lterator = p->DatabaseStringMap.begin(); lterator != p->DatabaseStringMap.end(); lterator = next(lterator))
                {
                    lNeeded_1.insert(make_pair(lterator->first, lterator->second));
                }
                pDatabaseColumnsInsert->AddColumn(p->TableName, lNeeded_1);
                return;
            }
        }
    }
    //vector < pair<string, map<string, map<int, DatabaseString*>>>> IndexedTable
    else if (lCondition.size() != 0 && ConditionSign.size() != 0 && pChecker.size() == 0)
    {
        for (Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
        {
            if (p->TableName == pTableName.begin()->second)
            {
                for (int lCounter = 0; lCounter < p->lIndexedTable.size(); lCounter++)
                {
                    if (p->lIndexedTable[lCounter].first == lCondition.begin()->first)
                    {
                        for (map<string, map<int, DatabaseString*>>::iterator lMapIterator = p->lIndexedTable[lCounter].second.begin(); lMapIterator != p->lIndexedTable[lCounter].second.end(); lMapIterator = next(lMapIterator))
                        {
                            if (ConditionSign == "=")
                            {
                                if (lMapIterator->first == lCondition.begin()->second)
                                {
                                    for (map<int, DatabaseString* >::iterator lDSIterator = lMapIterator->second.begin(); lDSIterator != lMapIterator->second.end(); lDSIterator = next(lDSIterator))
                                    {
                                        lNeeded_1.insert(make_pair(lDSIterator->first, *lDSIterator->second));
                                    }
                                }
                            }
                            else if (ConditionSign == "!=")
                            {
                                if (lMapIterator->first == lCondition.begin()->second)
                                {
                                    for (map<int, DatabaseString* >::iterator lDSIterator = lMapIterator->second.begin(); lDSIterator != lMapIterator->second.begin(); lDSIterator = next(lDSIterator))
                                    {
                                        lNeeded_1.insert(make_pair(lDSIterator->first, *lDSIterator->second));
                                    }
                                }
                            }
                        }
                    }
                    else if (lCounter + 1 == p->lIndexedTable.size())
                    {
                        break;
                    }
                }
                if (lNeeded_1.size() != 0)
                {
                    pDatabaseColumnsInsert->AddColumn(p->TableName, lNeeded_1);
                    break;
                }
                map<int, DatabaseString> lDatabaseStringMap = p->DatabaseStringMap;
                map<int, DatabaseString>::iterator lterator;
                DatabaseString lDatabaseStringNeeded;
                for (lterator = lDatabaseStringMap.begin(); lterator != lDatabaseStringMap.end(); lterator = next(lterator))
                {
                    for (DatabaseString::iterator lDSterator = lterator->second.begin(); lDSterator != lterator->second.end(); lDSterator = next(lDSterator))
                    {
                        if (ConditionSign == "=")
                        {
                            if (lDSterator->first == lCondition.begin()->first && lDSterator->second == lCondition.begin()->second)
                            {
                                lNeeded_1.insert(make_pair(lterator->first, lterator->second));
                                break;
                            }
                        }
                        else if (ConditionSign == "!=")
                        {
                            if (lDSterator->first == lCondition.begin()->first && lDSterator->second != lCondition.begin()->second)
                            {
                                lNeeded_1.insert(make_pair(lterator->first, lterator->second));
                                break;
                            }
                        }
                    }
                }
                if (lterator == lDatabaseStringMap.end())
                {
                    pDatabaseColumnsInsert->AddColumn(p->TableName, lNeeded_1);
                    break;
                }
            }
        }
    }
}
//vector< pair<string, string> > DatabaseString;
//Подсчет размера имен колонок в базе данных
vector< pair<string, int> > CountinSize(map<int, string>* pColumns)
{
    vector< pair<string, int> > lLengthColumn;
    int lCounter = 1;
    for (auto& lColumns : *pColumns)
    {
        lLengthColumn.push_back(pair<string, int>(lColumns.second, lColumns.second.length()));
        lCounter++;
    }
    return lLengthColumn;
}

//Функция получения долготы введенных значений в бд
vector< pair<string, int> > FindLength(DatabaseInsert* pDatabaseInsert, map<int, string>* pColumns)
{
    vector< pair<string, int> > lLineLengthInfo;
    int lCounterForDatabase = 1;
    int lLastLine = LastLineInfo(pDatabaseInsert);

    for (Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
    {
        map<int, DatabaseString> lDatabaseStringMap = p->DatabaseStringMap;
        map<int, DatabaseString>::iterator lIterator;
        for (lIterator = lDatabaseStringMap.begin(); lIterator != lDatabaseStringMap.end(); lIterator = next(lIterator))
        {
            DatabaseString lDatabaseString = lIterator->second;
            DatabaseString::iterator lIterator_2;
            for (lIterator_2 = lDatabaseString.begin(); lIterator_2 != lDatabaseString.end(); lIterator_2 = next(lIterator_2))
            {
                if (lIterator->first == lDatabaseStringMap.begin()->first)
                {
                    lLineLengthInfo.push_back(pair<string, int>(lIterator_2->first, lIterator_2->second.length()));
                    lCounterForDatabase++;
                }
                else
                {
                    pair<string, string> lpair = make_pair(lIterator_2->first, lIterator_2->second);
                    DatabaseString::iterator it = std::find(lDatabaseString.begin(), lDatabaseString.end(), lpair);
                    if (it != lDatabaseString.end())
                    {
                        vector< pair<string, int> > ::iterator lLineLengthInfoIter;
                        for (int idx = 0; idx < lLineLengthInfo.size(); idx++)
                        {
                            if ((lLineLengthInfo[idx].first == it->first) && (lLineLengthInfo[idx].second) < lIterator_2->second.length())
                            {
                                lLineLengthInfo[idx].second = lIterator_2->second.length();
                            }
                        }
                    }
                }
            }
        }
    }
    for (int lIndex = 0; lIndex < pColumns->size();)
    {
        for (map<int, string>::iterator lIter = pColumns->begin(); lIter != pColumns->end(); lIter= next(lIter))
        {
            if (lLineLengthInfo[lIndex].first == lIter->second)
            {
                lIndex++;
                break;
            }
            else if (next(lIter) == pColumns->end())
            {
                lLineLengthInfo.erase(lLineLengthInfo.begin() + lIndex);
                break;
            }
        }
    }
    return lLineLengthInfo;
}

//Функция сравнения долготы введенных значений в бд с значениями названий колонок бд
vector< pair<string, int> > CheckLength(vector< pair<string, int> >* pLengthColumn, vector < pair<string, int>>* pLengthInfo)
{
    vector< pair<string, int> > pMaxLengthInfo;
    vector < pair<string, int>> ::iterator lIteratorLengthInfo = pLengthInfo->begin();
    for (auto lLengthColumn : *pLengthColumn)
    {
        if (lLengthColumn.second >= lIteratorLengthInfo->second)
        {
            pMaxLengthInfo.push_back(pair<string, int>(lLengthColumn.first, lLengthColumn.second));
            lIteratorLengthInfo = std::next(lIteratorLengthInfo);
        }
        else
        {
            pMaxLengthInfo.push_back(pair<string, int>(lLengthColumn.first, lIteratorLengthInfo->second));
            lIteratorLengthInfo = std::next(lIteratorLengthInfo);
        }
    }
    return pMaxLengthInfo;
}

//Получения строк с отступом
void RetreatForDatabaseColumns(map<int, string>* pColumns, vector< pair<string, int> >* lMaxLengthInfo, string* pAllDatabaseColumns)
{
    string DatabaseColumn = "";
    string LeftBordForColumns = "";
    string RightBordForColumns = "";
    int lLenrthRetreat = 0;
    vector< pair<string, int> > ::iterator lIteratorMaxLengthInfo = lMaxLengthInfo->begin();
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
}

//Получения строк с отступом для данных БД
void RetreatForDatabaseInfo(DatabaseInsert* pDatabaseInsert, vector< pair<string, int> >* lMaxLengthInfo, map<int, string>* pAllDatabaseColumnsInsert)
{
    string DatabaseColumn = "";
    string LeftBordForColumns = "";
    string lAllDatabaseColumnsInsert = "";
    int lCounter = 1;

    for (Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
    {
        map<int, DatabaseString> lDatabaseStringMap = p->DatabaseStringMap;
        map<int, DatabaseString>::iterator lterator;
        for (lterator = lDatabaseStringMap.begin(); lterator != lDatabaseStringMap.end(); lterator = next(lterator))
        {
            DatabaseString lDatabaseStringNeeded = lterator->second;
            DatabaseString::iterator lterator_2;
            for (lterator_2 = lDatabaseStringNeeded.begin(); lterator_2 != lDatabaseStringNeeded.end(); lterator_2 = next(lterator_2))
            {
                pair<string, int> lPair;
                vector< pair<string, int> >::iterator lMIIterator;
                for (lMIIterator= lMaxLengthInfo->begin(); lMIIterator != lMaxLengthInfo->end(); lMIIterator=next(lMIIterator))
                {
                    if (lMIIterator->first == lterator_2->first)
                    {
                        lPair = make_pair(lMIIterator->first, lMIIterator->second);
                        break;
                    }
                }
                vector< pair<string, int> > ::iterator lIterator = std::find(lMaxLengthInfo->begin(), lMaxLengthInfo->end(), lPair);
                if (lIterator != lMaxLengthInfo->end())
                {
                    int lLength = lIterator->second - lterator_2->second.length();
                    for (int lCounetr = 1; lCounetr <= lLength; lCounetr++)
                    {
                        LeftBordForColumns = LeftBordForColumns + " ";
                    }
                    auto idx = std::distance(lMaxLengthInfo->begin(), lIterator);
                    if (idx == 0)
                    {
                        DatabaseColumn = "|";
                    }
                    DatabaseColumn = DatabaseColumn + LeftBordForColumns + lterator_2->second + +"|";
                    lAllDatabaseColumnsInsert = lAllDatabaseColumnsInsert + DatabaseColumn;
                    DatabaseColumn.clear();
                    LeftBordForColumns.clear();
                    vector< pair<string, int> > ::iterator lMaxLengthInfoIterator = prev(lMaxLengthInfo->end());
                    if (lterator_2->first == lMaxLengthInfoIterator->first)
                    {
                        pAllDatabaseColumnsInsert->insert(make_pair(lCounter, lAllDatabaseColumnsInsert));
                        lAllDatabaseColumnsInsert.clear();
                        lCounter++;
                    }
                }
            }
        }
    }
}

//Получаем карту значений нашей БД
void  OutDatabaseInsert(map<int, string>* lColumnsInfo, map<string, int>* pMaxLengthInfo, DatabaseInsert* pDatabaseInsert)
{
    int lCounter = 1;
    Node* p = pDatabaseInsert->begin();
    map<int, DatabaseString> lDatabaseStringMap = p->DatabaseStringMap;
    map<int, DatabaseString>::iterator lIterator;
    for (lIterator = lDatabaseStringMap.begin(); lIterator != lDatabaseStringMap.end(); lIterator = next(lIterator))
    {
        DatabaseString lDatabaseString = lIterator->second;
        DatabaseString::iterator lIterator_2;
        for (lIterator_2 = lDatabaseString.begin(); lIterator_2 != lDatabaseString.end(); lIterator_2 = next(lIterator_2))
        {
            lColumnsInfo->insert(make_pair(lCounter, lIterator_2->second));
            lCounter++;
        }
    }
}

//Проверка на размер количество введенных данних
void CgeckingVeluesOfInsert(DatabaseInsert* pDatabaseInsert, int pQuontityColumns, bool* pValue)
{
    string DatabaseColumns = "";
    map<int, string> lLineInfo;
    int lCounterForDatabase = 1;
    int lLastLine = LastLineInfo(pDatabaseInsert);
    for (const Node* lNode = pDatabaseInsert->begin(); lNode != NULL; lNode = lNode->next)
    {
        if (lNode->next == NULL)
        {
            map<int, DatabaseString> lDatabaseStringMap = lNode->DatabaseStringMap;
            map<int, DatabaseString>::iterator lDatabaseStringIterator;
            for (lDatabaseStringIterator = lDatabaseStringMap.begin(); lDatabaseStringIterator != lDatabaseStringMap.end(); lDatabaseStringIterator = next(lDatabaseStringIterator))
            {

                DatabaseString lDatabaseString = lDatabaseStringIterator->second;
                DatabaseString::iterator lIterator_2;
                for (lIterator_2 = lDatabaseString.begin(); lIterator_2 != lDatabaseString.end(); lIterator_2 = next(lIterator_2))
                {
                    lLineInfo.insert(make_pair(lCounterForDatabase, lIterator_2->second));
                    lCounterForDatabase++;
                }
                if (lLineInfo.size() != pQuontityColumns)
                {
                    if (lLineInfo.size() < pQuontityColumns) { cout << "В строке под номером " << lDatabaseStringIterator->first << " заполнено не все коноки! "; }
                    else cout << "В строке под номером " << lDatabaseStringIterator->first << " заполнено больше колонок чем существует! ";
                    cout << " Вы ввели значения для " << lLineInfo.size() << " колонок, а у нас в бд существует " << pQuontityColumns << " колонок!" << endl;
                    *pValue = 0;
                }
                lLineInfo.clear();
                lCounterForDatabase = 1;
            }
        }
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
        map<int, string>::iterator lCheking = pColumns->begin();//не юзаем
        string DatabaseName = pDatabaseColumns->begin()->TableName;
        vector< pair<string, int> > lLengthColumn = CountinSize(pColumns);
        vector< pair<string, int> > lLengthInfo = FindLength(pDatabaseInsert, pColumns);//находим размер наего Input
        vector< pair<string, int> > lMaxLengthInfo = CheckLength(&lLengthColumn, &lLengthInfo);//узнаем максимальный размер строки для того что бы знать сколько делать отступов
        RetreatForDatabaseColumns(pColumns, &lMaxLengthInfo, &AllDatabaseColumns);//Получаем строку с отступами и именами наших колонок
        pDatabaseColumns->ShowDatabase(AllDatabaseColumns, DatabaseName, &lLastLine);

        //OutDatabaseInsert(&lColumnsInfo, lMaxLengthInfo, pDatabaseInsert);//получаем карту значений нашей БД
        RetreatForDatabaseInfo(pDatabaseInsert, &lMaxLengthInfo, &AllDatabaseColumnsInfo);//получаем то что будем центрирывать
        for (auto& lAllDatabaseColumnsInfo : AllDatabaseColumnsInfo)
        {
            pDatabaseInsert->CenterString(lAllDatabaseColumnsInfo.second, lAllDatabaseColumnsInfo.second.length());
        }
        cout << lLastLine;
        lValue = 0;
    }
}
//Вывод того что внутри БД
void CoutDatabaseInsert(DatabaseInsert* pDatabaseInsert)
{
    Node * lNode;
    for (lNode = pDatabaseInsert->begin(); lNode != NULL; lNode = lNode->next)
    {
        if (lNode->next == NULL)
        {
            map<int, DatabaseString> lDatabaseStringMap = lNode->DatabaseStringMap;
            map<int, DatabaseString>::iterator lDatabaseStringIterator;
            for (lDatabaseStringIterator = lDatabaseStringMap.begin(); lDatabaseStringIterator != lDatabaseStringMap.end(); lDatabaseStringIterator = next(lDatabaseStringIterator))
            {
                DatabaseString lDatabaseString = lDatabaseStringIterator->second;
                DatabaseString::iterator lIterator;
                for (lIterator = lDatabaseString.begin(); lIterator != lDatabaseString.end(); lIterator = next(lIterator))
                {
                    std::cout << lIterator->first << "\t" << lIterator->second << endl;
                }
            }
        }
    }
}
void PrintOurDatabaseHellper(DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert, vector< pair<string, string> > lTableVector, map<int, string> pDatabaseColumnsName)
{
    DatabaseInfo DatabaseColumns_1, DatabaseColumns_2;
    DatabaseInsert lDatabaseInsertColumns;
    map<int, string> lColumns_1, lColumns_2, lTableName;
    map<string, string> lCondition;
    pair<string, string> lPairForCheckWHERE, lPairForCheckFULL_JOIN;
    lPairForCheckWHERE = make_pair("Comand","WHERE");
    lPairForCheckFULL_JOIN = make_pair("Comand", "FULL_JOIN");
    string lEmptyVariable;
    bool Pass = 0;
    if (lTableVector.size() == 1)
    {
        FindAllDatabaseColumns_1(pDatabaseInfo, lTableVector.begin()->second, &lColumns_1, &DatabaseColumns_1, pDatabaseColumnsName);//выводим все возможные колонки с нашим именем бд
        std::cout << endl; 
        lTableName.insert(make_pair(1, lTableVector[0].second));
        FindAllDatabaseInsert(pDatabaseInsert, lTableName, &lDatabaseInsertColumns, lCondition, lEmptyVariable, lEmptyVariable);
        std::cout << pDatabaseInsert->begin()->TableName << endl;
        CoutDatabaseInsert(pDatabaseInsert);
        std::cout << endl;
        PrintOurDatabase(&DatabaseColumns_1, &lDatabaseInsertColumns, &lColumns_1);
    }
    else if (lTableVector[1] == lPairForCheckWHERE)
    {
        FindAllDatabaseColumns_1(pDatabaseInfo, lTableVector[0].second, &lColumns_1, &DatabaseColumns_1, pDatabaseColumnsName);//выводим все возможные колонки с нашим именем бд
        std::cout << endl;
        lTableName.insert(make_pair(1, lTableVector[0].second));
        lCondition.insert(make_pair(lTableVector[2].second, lTableVector[4].second));
        FindAllDatabaseInsert(pDatabaseInsert, lTableName, &lDatabaseInsertColumns, lCondition, lTableVector[3].second, lEmptyVariable);
        std::cout << pDatabaseInsert->begin()->TableName << endl << "Данние которые мы выбрали:" << endl;
        CoutDatabaseInsert(&lDatabaseInsertColumns);
        if (lDatabaseInsertColumns.begin() == NULL)
        {
            pDatabaseInfo->FindAllDatabaseColumns(*pDatabaseInfo, lTableVector.begin()->second);
            return;
        }
        std::cout << endl;
        PrintOurDatabase(&DatabaseColumns_1, &lDatabaseInsertColumns, &lColumns_1);
    }
    //FULL_JOIN
    else if(lTableVector[1] == lPairForCheckFULL_JOIN)
    {
        FindAllDatabaseColumns_1(pDatabaseInfo, lTableVector[0].second, &lColumns_1, &DatabaseColumns_1, pDatabaseColumnsName);//выводим все возможные колонки с нашим именем бд первой
        FindAllDatabaseColumns_1(pDatabaseInfo, lTableVector[2].second, &lColumns_2, &DatabaseColumns_2, pDatabaseColumnsName);//выводим все возможные колонки с нашим именем бд второй
        if (pDatabaseColumnsName.begin()->second == "*")
        {
            Pass = 1;
            map<int, string>::iterator BeginIerator = pDatabaseColumnsName.begin();
            BeginIerator = pDatabaseColumnsName.erase(BeginIerator); // C++11
            for (BeginIerator = lColumns_1.begin(); BeginIerator != lColumns_1.end(); BeginIerator = next(BeginIerator))
            {
                pDatabaseColumnsName.insert(make_pair(BeginIerator->first, BeginIerator->second));
            }
            for (BeginIerator = lColumns_2.begin(); BeginIerator != lColumns_2.end(); BeginIerator = next(BeginIerator))
            {
                pDatabaseColumnsName.insert(make_pair(prev(pDatabaseColumnsName.end())->first + 1, BeginIerator->second));
            }
        }
        std::cout << endl;
        lTableName.insert(make_pair(1, lTableVector[0].second));
        lTableName.insert(make_pair(2, lTableVector[2].second));
        lCondition.insert(make_pair(lTableVector[4].second, lTableVector[6].second));
        Node* lNode_1 = pDatabaseInsert->begin();
        Node* lNode_2 = pDatabaseInsert->begin();
        for (Node* p = pDatabaseInsert->begin(); p != NULL; p = p->next)
        {
            if (p->TableName == lTableVector[0].second)
            {
                lNode_1 = p;
            }
            else if (p->TableName == lTableVector[2].second)
            {
                lNode_2 = p;
            }
        }
        map<string, DatabaseString*> lMapwith_1, lMapwith_2;
        for (int lCounter = 0; lCounter < lNode_1->lIndexedTable.size(); lCounter++)
        {
            if (lNode_1->lIndexedTable[lCounter].first == lCondition.begin()->first)
            {
                for (map<string, map<int, DatabaseString*>> ::iterator lDSIteratorUp = lNode_1->lIndexedTable[lCounter].second.begin(); lDSIteratorUp != lNode_1->lIndexedTable[lCounter].second.end(); lDSIteratorUp = next(lDSIteratorUp))
                {
                    for (map<int, DatabaseString*>::iterator lMap = lDSIteratorUp->second.begin(); lMap != lDSIteratorUp->second.end(); lMap = next(lMap))
                    {
                        lMapwith_1.insert(make_pair(lDSIteratorUp->first, lMap->second));
                    }
                }
            }
            else if ((lCounter + 1) == lNode_1->lIndexedTable.size())
            {
                break;
            }
        }
        if (lMapwith_1.size() == 0)
        {
            for (map<int, DatabaseString> ::iterator lNode = lNode_1->DatabaseStringMap.begin(); lNode != lNode_1->DatabaseStringMap.end(); lNode = next(lNode))
            {
                for (DatabaseString::iterator LDatabaseString = lNode->second.begin(); LDatabaseString != lNode->second.begin(); LDatabaseString = next(LDatabaseString))
                {
                    for (int lCounter = 0; lCounter < lNode->second.size(); lCounter++)
                        if (LDatabaseString[0].first == pDatabaseColumnsName.begin()->second)
                        {
                            lMapwith_1.insert(make_pair(LDatabaseString[0].second, &lNode->second));
                        }
                }
            }
        }
        for (int lCounter = 0; lCounter < lNode_2->lIndexedTable.size(); lCounter++)
        {
            if (lNode_2->lIndexedTable[lCounter].first == lCondition.begin()->second)
            {
                for (map<string, map<int, DatabaseString*>> ::iterator lDSIteratorUp = lNode_2->lIndexedTable[lCounter].second.begin(); lDSIteratorUp != lNode_2->lIndexedTable[lCounter].second.end(); lDSIteratorUp = next(lDSIteratorUp))
                {
                    for (map<int, DatabaseString*>::iterator lMap = lDSIteratorUp->second.begin(); lMap != lDSIteratorUp->second.end(); lMap = next(lMap))
                    {
                        lMapwith_2.insert(make_pair(lDSIteratorUp->first, lMap->second));
                    }
                }
            }
            else if ((lCounter + 1) == lNode_2->lIndexedTable.size())
            {
                break;
            }
        }
        if (lMapwith_2.size() == 0)
        {
            for (map<int, DatabaseString> ::iterator lNode = lNode_2->DatabaseStringMap.begin(); lNode != lNode_2->DatabaseStringMap.end(); lNode = next(lNode))
            {
                for (DatabaseString::iterator LDatabaseString = lNode->second.begin(); LDatabaseString != lNode->second.begin(); LDatabaseString = next(LDatabaseString))
                {
                    for (int lCounter = 0; lCounter < lNode->second.size(); lCounter++)
                        if (LDatabaseString[0].first == pDatabaseColumnsName.begin()->second)
                        {
                            lMapwith_2.insert(make_pair(LDatabaseString[0].second, &lNode->second));
                        }
                }
            }
        }
        std::cout <<  "Данние которые мы выбрали:" << endl;
        map<int, string> AllDatabaseColumnsInfo;
        string AllDatabaseColumns = "";
        vector< pair<string, int> > lLengthColumn = CountinSize(&pDatabaseColumnsName);
        vector< pair<string, int> > lLengthInfo = FindLength(pDatabaseInsert, &pDatabaseColumnsName);//находим размер наего Input
        vector< pair<string, int> > lMaxLengthInfo = CheckLength(&lLengthColumn, &lLengthInfo);//узнаем максимальный размер строки для того что бы знать сколько делать отступов

        string FirstLine = "", lLastLine = "";
        string lTableName = pDatabaseInsert->begin()->TableName + "&" + (pDatabaseInsert->begin()->next)->TableName;
        int lRight = 0, lLeft = 0, lMaxLength;
        for (int lNumber = 0; lNumber < lMaxLengthInfo.size(); lNumber++)
        {
            lRight = lRight + lMaxLengthInfo[lNumber].second;
        }
        lMaxLength = lRight;
        lRight = (lMaxLength - lTableName.size()/ 2) / 2;
        lLeft = lMaxLength - lRight - lTableName.size() / 2;
        for (int lCounter = 1; lCounter < lLeft-1; lCounter++)
        {
            FirstLine = FirstLine + "-";
        }
        FirstLine = FirstLine + lTableName;
        for (int lCounter = 1; lCounter < lRight; lCounter++)
        {
            FirstLine = FirstLine + "-";
        }
        for (int lCounter = 0; lCounter <= lMaxLength + lMaxLengthInfo.size(); lCounter++)
        {
            lLastLine = lLastLine + "-";
        }
        int lCounter = 1;
        AllDatabaseColumnsInfo.insert(make_pair(lCounter, FirstLine));
        lCounter++;
        RetreatForDatabaseColumns(&pDatabaseColumnsName, &lMaxLengthInfo, &AllDatabaseColumns);//Получаем строку с отступами и именами наших колонок
        AllDatabaseColumnsInfo.insert(make_pair(lCounter, AllDatabaseColumns));
        lCounter++;
        AllDatabaseColumnsInfo.insert(make_pair(lCounter, lLastLine));
        lCounter++;
        AllDatabaseColumns.clear();
        map<int, string> lDatabaseColumnsName;
        map<string, DatabaseString*>::iterator LMapwith2, LMapwith2_1;
        for (map<string, DatabaseString*>:: iterator LMapwith = lMapwith_1.begin(); LMapwith != lMapwith_1.end(); LMapwith=next(LMapwith))
        {
            LMapwith2 = lMapwith_2.find(LMapwith->first);
            if (LMapwith2 == lMapwith_2.end())
            {
                for (DatabaseString::iterator lDatabaseString = LMapwith->second->begin(); lDatabaseString != LMapwith->second->end(); lDatabaseString=next(lDatabaseString))
                {
                    if (lDatabaseString->first == pDatabaseColumnsName.begin()->second)
                    {
                        if (Pass == 1)
                        {
                            for (int lBeb = 1; lBeb <= LMapwith->second->size(); lBeb++)
                            {
                                lDatabaseColumnsName.insert(make_pair(lBeb, lDatabaseString->second));
                                lDatabaseString = next(lDatabaseString);
                            }
                            for (int lBeb = prev(lDatabaseColumnsName.end())->first; lBeb < lMaxLengthInfo.size(); lBeb++)
                            {
                                lDatabaseColumnsName.insert(make_pair(prev(lDatabaseColumnsName.end())->first + 1, "NULL"));
                            }
                        }
                        else
                        {
                            lDatabaseColumnsName.insert(make_pair(1, lDatabaseString->second));
                            lDatabaseColumnsName.insert(make_pair(2, "NULL"));
                        }
                        break;
                    }
                }
            }
            else
            {
                map<int, string> ::iterator lpDatabaseColumnsName = pDatabaseColumnsName.begin();
                for (DatabaseString::iterator lDatabaseString = LMapwith->second->begin(); lDatabaseString != LMapwith->second->end(); lDatabaseString = next(lDatabaseString))
                {
                    if (lDatabaseString->first == lpDatabaseColumnsName->second)
                    {
                        if (Pass ==1)
                        {
                            for (int lBeb = 1; lBeb <= LMapwith->second->size(); lBeb++)
                            {
                                lDatabaseColumnsName.insert(make_pair(lBeb, lDatabaseString->second));
                                lDatabaseString = next(lDatabaseString);
                                lpDatabaseColumnsName = next(lpDatabaseColumnsName);
                            }
                        }
                        else 
                        {
                            lDatabaseColumnsName.insert(make_pair(1, lDatabaseString->second)); 
                            lpDatabaseColumnsName = next(lpDatabaseColumnsName);
                        }
                        break;
                    }
                }
                for (DatabaseString::iterator lDatabaseString = LMapwith2->second->begin(); lDatabaseString != LMapwith2->second->end(); lDatabaseString = next(lDatabaseString))
                {
                    if (lDatabaseString->first == lpDatabaseColumnsName->second)
                    {
                        if (Pass == 1)
                        {
                            for (int lBeb = prev(lDatabaseColumnsName.end())->first; lBeb < lMaxLengthInfo.size(); lBeb++)
                            {
                                lDatabaseColumnsName.insert(make_pair(prev(lDatabaseColumnsName.end())->first + 1, lDatabaseString->second));
                                if(lBeb < lMaxLengthInfo.size()-1)
                                    lDatabaseString = next(lDatabaseString);
                            }
                        }
                        else
                        {
                            lDatabaseColumnsName.insert(make_pair(2, lDatabaseString->second));
                        }
                    }
                    if (next(lDatabaseString) == LMapwith2->second->end())
                    {
                        LMapwith2_1 = lMapwith_2.find(LMapwith2->first);
                        LMapwith2_1 = lMapwith_2.erase(LMapwith2_1); // C++11
                        break;
                    }
                }
            }
            RetreatForDatabaseColumns(&lDatabaseColumnsName, &lMaxLengthInfo, &AllDatabaseColumns);//Получаем строку с отступами и именами наших колонок
            AllDatabaseColumnsInfo.insert(make_pair(lCounter, AllDatabaseColumns));
            lCounter++;
            AllDatabaseColumns.clear();
            lDatabaseColumnsName.clear();
        }
        for (LMapwith2 = lMapwith_2.begin(); LMapwith2 != lMapwith_2.end(); LMapwith2 = next(LMapwith2))
        {
            for (map<string, DatabaseString*>::iterator lMapwith = lMapwith_2.begin(); LMapwith2 != lMapwith_2.end(); lMapwith = next(lMapwith))
            {
                for (DatabaseString::iterator lDatabaseString = lMapwith->second->begin(); lDatabaseString != lMapwith->second->end(); lDatabaseString = next(lDatabaseString))
                {
                    if (Pass == 1)
                    {
                        for (int lBeb = 1; lBeb <= lMapwith->second->size(); lBeb++)
                        {
                            lDatabaseColumnsName.insert(make_pair(lBeb, "NULL"));
                        }
                        for (int lBeb = prev(lDatabaseColumnsName.end())->first; lBeb < lMaxLengthInfo.size(); lBeb++)
                        {
                            lDatabaseColumnsName.insert(make_pair(prev(lDatabaseColumnsName.end())->first + 1, lDatabaseString->second));
                            lDatabaseString = next(lDatabaseString);
                        }
                    }
                    else
                    {
                        for (;;)
                        {
                            if (lDatabaseString->first == next(pDatabaseColumnsName.begin())->second)
                            {
                                lDatabaseColumnsName.insert(make_pair(1, "NULL"));
                                lDatabaseColumnsName.insert(make_pair(2, lDatabaseString->second));
                                if (next(lDatabaseString) == LMapwith2->second->end())
                                {
                                    LMapwith2_1 = lMapwith_2.find(LMapwith2->first);
                                    LMapwith2_1 = lMapwith_2.erase(LMapwith2_1); // C++11
                                    break;
                                }
                                break;
                            }
                            lDatabaseString = next(lDatabaseString);
                        }
                    }
                    RetreatForDatabaseColumns(&lDatabaseColumnsName, &lMaxLengthInfo, &AllDatabaseColumns);//Получаем строку с отступами и именами наших колонок
                    AllDatabaseColumnsInfo.insert(make_pair(lCounter, AllDatabaseColumns));
                    lCounter++;
                    LMapwith2_1 = lMapwith_2.begin();
                    if (next(LMapwith2_1) == lMapwith_2.end())
                    {
                        if (Pass != 1)
                        {
                            AllDatabaseColumnsInfo.insert(make_pair(lCounter, LMapwith2_1->first));
                        }
                        AllDatabaseColumnsInfo.insert(make_pair(lCounter+1, lLastLine));
                        for (auto& lAllDatabaseColumnsInfo : AllDatabaseColumnsInfo)
                        {
                            pDatabaseInsert->CenterString(lAllDatabaseColumnsInfo.second, lAllDatabaseColumnsInfo.second.length());
                        }
                        return;
                    }
                }

            }
        }
        AllDatabaseColumnsInfo.insert(make_pair(lCounter, lLastLine));
        for (auto& lAllDatabaseColumnsInfo : AllDatabaseColumnsInfo)
        {
            pDatabaseInsert->CenterString(lAllDatabaseColumnsInfo.second, lAllDatabaseColumnsInfo.second.length());
        }
    }
}

//функция вывода бд по имени
void PrintOutDatabaseInfoHellper(vector< pair<string, string> > lTableVector, bool lCheck, map<int, string> pDatabaseColumnsName, DatabaseInfo *gDatabaseInfo, DatabaseInsert *gDatabaseInsert)
{
    std::cout << "The name of your table is " << lTableVector.begin()->second << endl;
    lCheck = CheckForAvailability(lTableVector.begin()->second, *gDatabaseInfo);
    DatabaseCheckExist(lCheck, lTableVector.begin()->second); if (lCheck == 0) { return; }
    for(Node* lNode = (*gDatabaseInsert).begin(); lNode != NULL; lNode = lNode->next)
    { 
        if      (lNode->TableName == lTableVector.begin()->second)
        {
            PrintOurDatabaseHellper(gDatabaseInfo, gDatabaseInsert, lTableVector, pDatabaseColumnsName);
            break;
        }
        else if (lNode->next == NULL)
        {
            (*gDatabaseInfo).FindAllDatabaseColumns(*gDatabaseInfo, lTableVector.begin()->second);
            break;
        }
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
                vector< pair<string, string> > lNameTableVector;
                lNameTableVector.push_back(pair<string, string>("NameTable", lNameTable));
                PrintOutDatabaseInfoHellper(lNameTableVector, lCheck, lDatabaseColumnsName, pDatabaseInfo, pDatabaseInsert);
                return;
            }
        }
        pNumberCurrentItemString++;
    }
}