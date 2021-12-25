#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <map>
#include <algorithm>
#include <iterator>
#include "DatabaseInfo.h"
#include "DatabaseInsert.h"
using namespace std;
//Вывод того что внутри БД
void CoutDatabaseInsert(DatabaseInsert* pDatabaseInsert);
//функция нахождения количества строк в бд
int LastLineInfo(DatabaseInsert* pDatabaseInsert);
//Поиск всехго того что мы вводили в БД
void FindAllDatabaseInsert(DatabaseInsert* pDatabaseInsert, map<int, string> pTableName, DatabaseInsert* pDatabaseColumnsInsert, map<string, string> lCondition, string ConditionSign, string pChecker);
//Подсчет размера имен колонок в базе данных
vector< pair<string, int> > CountinSize(map<int, string>* pColumns);
//Функция получения долготы введенных значений в бд
vector< pair<string, int> > FindLength(DatabaseInsert* pDatabaseInsert, map<int, string>* pColumns);
//Функция сравнения долготы введенных значений в бд с значениями названий колонок бд
vector< pair<string, int> > CheckLength(vector< pair<string, int> >* pLengthColumn, vector < pair<string, int>>* pLengthInfo);
//Получения строк с отступом
void RetreatForDatabaseColumns(map<int, string>* pColumns, vector< pair<string, int> >* lMaxLengthInfo, string* pAllDatabaseColumns);
//Получения строк с отступом для данных БД
void RetreatForDatabaseInfo(DatabaseInsert* pDatabaseInsert, vector< pair<string, int> >* lMaxLengthInfo, map<int, string>* pAllDatabaseColumnsInsert);
//Получаем карту значений нашей БД
void  OutDatabaseInsert(map<int, string>* lColumnsInfo, map<int, int>* pMaxLengthInfo, DatabaseInsert* pDatabaseInsert);
//Проверка на размер количество введенных данних
void CgeckingVeluesOfInsert(DatabaseInsert* pDatabaseInsert, int pQuontityColumns, bool* pValue);
//Печать БД с колонками и строками (работает правильно)
void PrintOurDatabase(DatabaseInfo* pDatabaseColumns, DatabaseInsert* pDatabaseInsert, map<int, string>* pColumns);
//функция которая имеет в себе все функции выше и выводит нашу таблицу
void PrintOurDatabaseHellper(DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert, vector< pair<string, string> > lTableVector, map<int, string> pDatabaseColumnsName);
//функция вывода бд по имени
void PrintOutDatabaseInfoHellper(vector< pair<string, string> > lTableVector, bool lCheck, map<int, string> pDatabaseColumnsName, DatabaseInfo *gDatabaseInfo, DatabaseInsert *gDatabaseInsert);
//функция обработки команды принт
void PrintOutDatabaseInfo(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert);
