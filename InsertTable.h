#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
#include<windows.h>
#include <map>
#include <iterator>
#include "DatabaseInfo.h"
#include "DatabaseInsert.h"

typedef  map<int, string> IS_map;
typedef IS_map::iterator IS_Iterator;
typedef  map<string, int> SI_map;
typedef SI_map::iterator SI_Iterator;

bool AnalizArgumentsOfComandInsertHellper(string pNameTable, int* NumberColumn, string* NameVariable, IS_Iterator lColumnslIterator, SI_Iterator lIterator, DatabaseInsert* pDatabaseInsert, SI_map TablesLinesInfo);
void AnalizArgumentsOfComandInsert(string pComand, string pNameTable, DatabaseInfo *pDatabaseInfo, DatabaseInsert *pDatabaseInsert, SI_map *TablesLinesInfo);
int InsertTableHellper(string pFullCommand, int pNumberCurrentItemStringForCheck, string pNameTable, DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert, SI_map *TablesLinesInfo);
int InsertTable(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo *pDatabaseInfo, DatabaseInsert *pDatabaseInsert, SI_map *TablesLinesInfo);