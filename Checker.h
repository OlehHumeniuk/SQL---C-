#pragma once

#include <string>
#include <iostream>
#include "DatabaseInfo.h"
using namespace std;

//Набор вспомогательных функций которые успользуются во время анализа всех команд
//Проверка на последний символ ;
bool CheckForTheLastCharacter(string pFullCommand);
//Прооверка на существование БД
void DatabaseCheckExist(bool lCheck, string pNameTable);
//Проверка на то существует ли эта таблица!
bool CheckForAvailability(string  pNameTable, const DatabaseInfo& pDatabaseInfo);
//проверка на символ, является ли он в диапазоне английского алфавита
bool HellperChek(char c);
