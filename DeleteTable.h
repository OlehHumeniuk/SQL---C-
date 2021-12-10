#pragma once

#include <string>
#include <iostream>
#include <stdio.h>
#include<windows.h>
#include <map>
#include <iterator>

#include "DatabaseInfo.h"
#include "DatabaseInsert.h"
#include "Checker.h"
using namespace std;

void DeleteTable(string pTableName, DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert);