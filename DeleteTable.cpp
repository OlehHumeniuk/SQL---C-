#include "DeleteTable.h"


void DeleteTable(string pTableName, DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert)
{
	bool lCheck = CheckForAvailability(pTableName, *pDatabaseInfo);
	DatabaseCheckExist(lCheck, pTableName);
	if (lCheck == 0) { return; }
	for (Knot* p = pDatabaseInfo->begin(); p != NULL;)
	{
		if(p->TableName == pTableName)
		{
			(*pDatabaseInfo).Erase(p); 
			p = pDatabaseInfo->begin();
		}
		else { p = next(p); }
	}
	for (Node* p = pDatabaseInsert->begin(); p != NULL; p = pDatabaseInsert->begin())
	{
		if (p->TableName == pTableName)
		{
			(*pDatabaseInsert).Erase(p);
			p = pDatabaseInsert->begin();
		}
		else { p = next(p); }
	}
}