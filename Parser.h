#pragma once

#include <string>
#include <iostream>
#include <locale>
#include <string>
#include <algorithm>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <map>
#include <iterator>
#include <regex>
#include "DatabaseInfo.h"
#include "DatabaseInsert.h"
#include "CreateTable.h"
#include "Checker.h"
#include "InsertTable.h"
#include "Hellper.h"
#include "PrintTable.h"
#include "SelectTable.h"
#include "DeleteTable.h"

string GetName(string pFullCommand, int lLength);
void check(string pComandString, string pFullCommand, int pNumberCurrentItemString, DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert, map <string, int>* TablesLinesInfo);
void AnaliseComand(char buffer[255], DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert, map <string, int>* TablesLinesInfo);//функцыя анализа нашей строки