#pragma once

//Подключаем библиотеки
#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <list>
#include <cstdlib>
#include <cassert>
#include <deque>
#include "DatabaseInsert.h"//подключаем нашу вторую библиотеку для получения значений вывода которе используем в функции показа нашей бд

using namespace std;
struct Knot
{
    Knot* next;
    string  TableName;       // Имя таблици
    string  ColumnName;     // Имя стобца
    string  ColumnIndexationCheck;   // Тип данных
};
class DatabaseInfo
{
private:// Спецификатор доступа private
    Knot* head, * tail;
    size_t cnt;
public:// Спецификатор доступа public
    DatabaseInfo(void) :head(NULL), tail(NULL), cnt(0) {}
    ~DatabaseInfo() { DeleteAllDatabases(); }

    Knot* AddColumn(string pDatabaseName, string pDatabaseColumn, string pColumnTypeOfData);        //добавление 
    Knot* Insert(Knot* pos, string pTableName, string pColumnName, string pColumnTypeOfData);//вставка перед pos

    Knot* Erase(Knot* pos);             //удаление по pos
    void DeleteFirstAddElement(void);   //удаление из головы
    void DeleteLastAddElement(void);    //удаление из хвоста за O(n) "опасно" медленно
    void DeleteAllDatabases(void);      //удаление всех
    void GetConsBuff(int& x, int& y);   //Получение размеров окна консоли без прокрутки    
    void CenterString(string& pTableName, string& pDatabaseColumns, const size_t& width, string* pLastLine);        //Функция размещения таблицы по центру конольного приложения
    void ShowDatabase(string pDatabaseColumns, string pDatabaseName, string* pLastLine);                               //Функция отображения всех столбиков нашей бд
    void FindAllDatabaseColumns(const DatabaseInfo& pDatabaseInfo, string pDatabaseName);         //Поиск всех столбцов БД
    //void RetreatForDatabaseColumns(map<int, string>* pColumns, map<int, int>* lMaxLengthInfo, string* pAllDatabaseColumns);
    string GoodLine(int pSizeOfLine, string* pLine, string pElement);                               //Функция которая возвращает нам строки после заполнения символами
    
    Knot* begin(void) const;
    Knot* end  (void) const;

    size_t size(void) const;
    bool  empty(void) const;
};