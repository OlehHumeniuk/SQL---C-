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
void AnaliseHelpper(string pFullCommand, int pNumberCurrentItemString, string* lFullCommand);
void AnalizArgumentsOfComandISelect(string pComand, DatabaseInfo *gDatabaseInfo, DatabaseInsert *gDatabaseInsert);
void SelectTable(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo* gDatabaseInfo, DatabaseInsert* gDatabaseInsert);
