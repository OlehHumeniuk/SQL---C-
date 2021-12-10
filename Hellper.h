#pragma once

#include <string>
#include <iostream>
#include <stdio.h>
#include<windows.h>
#include <map>
#include <iterator>
#include "DatabaseInfo.h"
#include "DatabaseInsert.h"

void FindAllDatabaseColumns_1(const DatabaseInfo* pDatabaseInfo, string pDatabaseName, map<int, string>* pColumns, DatabaseInfo* pDatabaseColumns, map<int, string> pDatabaseColumnsName);