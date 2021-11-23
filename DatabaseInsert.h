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
#include <map>
#include <iterator>
#include "DatabaseInfo.h"//подключим нашу библиотеку для использывания функций центрации строк

using namespace std;
#pragma once
struct Node
{
    Node* next;
    string  DatabaseName;       // Имя базыданных
    string  ColumnData;         // Значения столбца
    int CounterOfLine;          // Счетчик строки
};
class DatabaseInsert
{
private:// Спецификатор доступа private
    Node* head, * tail;
    size_t cnt;
public:// Спецификатор доступа public
    DatabaseInsert(void) :head(NULL), tail(NULL), cnt(0) {}
    ~DatabaseInsert() { DeleteAllDatabases(); }
    Node* AddColumn(string pDatabaseName, string pColumnData, int pCounterOfLine);                                        //Добавление 
    Node* Insert(Node* pos, string pDatabaseName, string pColumnData, int pCounterOfLine);                                //Вставка перед pos
    void AddAllElementsDatabase(DatabaseInsert& lDatabaseInsert, int* pCounter, string pDatabaseName, string pColumnData);//Добавления всех колонок нашей базы данных
    Node* Erase(Node* pos);             //Удаление по pos
    void DeleteFirstAddElement(void);   //Удаление из головы
    void DeleteLastAddElement(void);    //Удаление из хвоста за O(n) "опасно" медленно
    void DeleteAllDatabases(void);      //Удаление всех

    void CenterString(string& pDatabaseColumnsInfo, const size_t& width);//Функция размещения таблицы по центру конольного приложения

    Node* begin(void);
    Node* begin(void) const;

    size_t size(void) const;
    bool  empty(void) const;
};



