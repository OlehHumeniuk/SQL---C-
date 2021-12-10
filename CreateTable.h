#pragma once

#include <string>
#include <iostream>
#include <stdio.h>
#include<windows.h>
#include <map>
#include <iterator>
#include "DatabaseInfo.h"

bool AnalizArgumentsOfComandCreate(string pComand, string pNameTable, DatabaseInfo *pDatabaseInfo);
int CreateTableHellper(string pFullCommand, int pNumberCurrentItemStringForCheck, string pNameTable, DatabaseInfo *pDatabaseInfo);
int CreateTable(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo *pDatabaseInfo);