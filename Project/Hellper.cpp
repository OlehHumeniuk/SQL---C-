#include "Hellper.h"

//Поиск всех столбцов БД
void FindAllDatabaseColumns_1(const DatabaseInfo* pDatabaseInfo, string pDatabaseName, map<int, string>* pColumns, DatabaseInfo* pDatabaseColumns, map<int, string> pDatabaseColumnsName)
{
    int lCounter = 1;
    DatabaseInfo lDatabaseColumns;
    map<int, string> ::iterator lIterrator;
    if ((pDatabaseColumnsName.begin() == pDatabaseColumnsName.end()) || (pDatabaseColumnsName.begin()->second == "*"))
    {
        for (const Knot* p = pDatabaseInfo->begin(); p != NULL; p = p->next)
        {
            if (p->TableName == pDatabaseName)
            {
                pColumns->insert(make_pair(lCounter, p->ColumnName));
                pDatabaseColumns->AddColumn(p->TableName, p->ColumnName, p->ColumnIndexationCheck);
                lCounter++;
            }
        }
    }
    else
    {
        for (const Knot* p = pDatabaseInfo->begin(); p != NULL; p = p->next)
        {
            for (lIterrator = pDatabaseColumnsName.begin(); lIterrator != pDatabaseColumnsName.end(); lIterrator = next(lIterrator))
            {
                if (p->TableName == pDatabaseName && p->ColumnName == lIterrator->second)
                {
                    pColumns->insert(make_pair(lCounter, p->ColumnName));
                    pDatabaseColumns->AddColumn(p->TableName, p->ColumnName, p->ColumnIndexationCheck);
                    lCounter++;
                }
            }
        }
    }
    for (auto& lColumns : *pColumns) { std::cout << lColumns.first << "\t" << lColumns.second << endl; }
}