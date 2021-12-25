#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
#include<windows.h>
#include <map>
#include <iterator>
#include <regex>
#include "DatabaseInfo.h"
#include "DatabaseInsert.h"
#include "Checker.h"

void ColumnChecker(map<int, string> pDatabaseColumnsName, string pTableName, const DatabaseInfo& pDatabaseInfo, bool* pCheck);
void AnaliseFrom(string pAllComand, string* pComand, string* pTableName);
vector< pair<string, string> > AnaliseHelpper(string pFullCommand, int pNumberCurrentItemString);
void AnalizArgumentsOfComandISelect(string pComand, DatabaseInfo *gDatabaseInfo, DatabaseInsert *gDatabaseInsert);
void SelectTable(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo* gDatabaseInfo, DatabaseInsert* gDatabaseInsert);
