#pragma once
//Подключаем библиотеки
#include <windows.h>
#include <map>
#include <vector>
#include "DatabaseInfo.h"//подключим нашу библиотеку для использывания функций центрации строк

using namespace std;
#pragma once
typedef vector< pair<string, string> > DatabaseString; // первым елементом является имя стобца, а вторым елемент самой строки
//первый елемент -  имя индексирываной колонки, второй же карта в которой хранится индексирываное значение и указатель на строку
typedef  vector < pair<string, map<string, map<int, DatabaseString*>>>> IndexedTable;
struct Node
{
    Node* next;
    string  TableName;          // Имя таблици
    map<int, DatabaseString> DatabaseStringMap;//мап первым елементом которого является елемент номер строки
    IndexedTable lIndexedTable;
};
class DatabaseInsert
{
private:// Спецификатор доступа private
    Node* head, * tail;
    size_t cnt;
public:// Спецификатор доступа public
    DatabaseInsert(void) :head(NULL), tail(NULL), cnt(0) {}
    ~DatabaseInsert() { DeleteAllDatabases(); }
    Node* AddColumn(string pTableName, map<int, DatabaseString> pDatabaseStringMap);                                        //Добавление 
    Node* Insert(Node* pos, map<int, DatabaseString> pDatabaseStringMap, string  pTableName);                                //Вставка перед pos
    void AddAllElementsDatabase(DatabaseInsert& lDatabaseInsert, string pTableName, map<int, DatabaseString> pDatabaseStringMap);//Добавления всех колонок нашей базы данных
    Node* Erase(Node* pos);             //Удаление по pos
    void DeleteFirstAddElement(void);   //Удаление из головы
    void DeleteLastAddElement(void);    //Удаление из хвоста за O(n) "опасно" медленно
    void DeleteAllDatabases(void);      //Удаление всех

    void CenterString(string& pDatabaseColumnsInfo, const size_t& width);//Функция размещения таблицы по центру конольного приложения

    Node* begin(void);

    size_t size(void) const;
    bool  empty(void) const;
};



