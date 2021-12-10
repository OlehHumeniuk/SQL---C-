#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
#include<windows.h>
#include <map>
#include <iterator>
#include "DatabaseInfo.h"
#include "DatabaseInsert.h"

//функция нахождения количества строк в бд
void LastLineInfo(DatabaseInsert* pDatabaseInsert, int* pLastLine);
//Поиск всехго того что мы вводили в БД
void FindAllDatabaseInsert(const DatabaseInsert* pDatabaseInsert, string pDatabaseName, DatabaseInsert* pDatabaseColumnsInsert, map<int, string> pDatabaseColumnsName, map<int, string> pColumns);
//Подсчет размера имен колонок в базе данных
void CountinSize(map<int, string>* pColumns, map<int, int>* pLengthColumn);
//Функция получения долготы введенных значений в бд
void FindLength(DatabaseInsert* pDatabaseInsert, map<int, int>* pLengthInfo, int pQuontityColumns);
//Функция сравнения долготы введенных значений в бд с значениями названий колонок бд
void CheckLength(map<int, int>* pLengthColumn, map<int, int>* pLengthInfo, map<int, int>* pMaxLengthInfo);
//Получения строк с отступом
void RetreatForDatabaseColumns(map<int, string>* pColumns, map<int, int>* lMaxLengthInfo, string* pAllDatabaseColumns);
//Получения строк с отступом для данных БД
void RetreatForDatabaseInfo(map<int, string>* pColumnsIndo, map<int, int>* lMaxLengthInfo, map<int, string>* pAllDatabaseColumnsInsert);
//Получаем карту значений нашей БД
void  OutDatabaseInsert(map<int, string>* lColumnsInfo, map<int, int>* pMaxLengthInfo, DatabaseInsert* pDatabaseInsert);
//Проверка на размер количество введенных данних
void CgeckingVeluesOfInsert(DatabaseInsert* pDatabaseInsert, int pQuontityColumns, bool* pValue);
//Печать БД с колонками и строками (работает правильно)
void PrintOurDatabase(DatabaseInfo* pDatabaseColumns, DatabaseInsert* pDatabaseInsert, map<int, string>* pColumns);
//функция которая имеет в себе все функции выше и выводит нашу таблицу
void PrintOurDatabaseHellper(DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert, string pDatabaseName, map<int, string> pDatabaseColumnsName);
//функция вывода бд по имени
void PrintOutDatabaseInfoHellper(string lNameTable, bool lCheck, map<int, string> pDatabaseColumnsName, DatabaseInfo *gDatabaseInfo, DatabaseInsert *gDatabaseInsert);
//функция обработки команды принт
void PrintOutDatabaseInfo(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert);
