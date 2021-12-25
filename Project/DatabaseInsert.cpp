#include "DatabaseInsert.h"

//Добавление 
Node* DatabaseInsert::AddColumn(string pTableName, map<int, DatabaseString> pDatabaseStringMap)
{
    Node* NewNode = new (std::nothrow) Node();
    if (NewNode != NULL) {
        NewNode->TableName = pTableName;             // Инициализация имени таблици куда записываем новый столбик
        NewNode->DatabaseStringMap = pDatabaseStringMap;

        //NewNode->ColumnName = pColumnName;              // Инициализация имени колонки
        //NewNode->ColumnData = pColumnData;              // Инициализация информации которую вписиваем в бд
        //NewNode->CounterOfLine = pCounterOfLine;        // Инициализация счетчика строк

        NewNode->next = NULL;

        if (head == NULL)
            head = tail = NewNode;
        else
            tail = tail->next = NewNode;
        ++cnt;
    }
    return NewNode;
}

//Вставка перед pos
Node* DatabaseInsert::Insert(Node* pos, map<int, DatabaseString> pDatabaseStringMap, string  pTableName) 
{
    if (pos == NULL)
        return pos;

    Node* x = head, * y = head;
    while (x != pos) {
        y = x;
        x = x->next;
    }

    Node* NewNode = new (std::nothrow) Node();
    if (NewNode != NULL) {
        NewNode->TableName = pTableName;                // Инициализация имени бд 
        NewNode->DatabaseStringMap = pDatabaseStringMap;// Инициализация мапа со всей вставкой
        //NewNode->ColumnData = pColumnData;              // Инициализация информации которую вписиваем в бд
        //NewNode->CounterOfLine = pCounterOfLine;        // Инициализация счетчика строк

        if (pos == head) {
            NewNode->next = head;
            head = NewNode;
        }
        else {
            NewNode->next = pos;
            y->next = NewNode;
        }
        ++cnt;
    }
    return pos;
}

//Удаление по pos
Node* DatabaseInsert::Erase(Node* pos) {
    if (pos == NULL)
        return pos;
    else if (pos == head) {
        head = head->next;
        if (head == NULL)
            tail = NULL;
    }
    else {
        Node* x = head, * y = head;
        while (x != pos) {
            y = x;
            x = x->next;
        }
        y->next = pos->next;

        if (pos == tail)
            tail = y;
    }

    Node* p = pos->next;
    delete pos;
    --cnt;
    return p;
}

//Удаление из головы
void DatabaseInsert::DeleteFirstAddElement(void) { Erase(head); }

//Удаление из хвоста за O(n) "опасно" медленно
void DatabaseInsert::DeleteLastAddElement(void) { Erase(tail); }

//Удаление всех
void DatabaseInsert::DeleteAllDatabases(void) {
    Node* t;
    while (head != NULL) {
        t = head;
        head = head->next;
        delete t;
    }
    tail = NULL;
    cnt = 0;
}

//Добавления всех колонок нашей базы данных
void DatabaseInsert::AddAllElementsDatabase(DatabaseInsert& lDatabaseInsert, string pTableName, map<int, DatabaseString> pDatabaseStringMap)
{
    lDatabaseInsert.AddColumn(pTableName, pDatabaseStringMap);
}

//Функция размещения таблицы по центру конольного приложения
void DatabaseInsert::CenterString( string& pDatabaseColumnsInfo, const size_t& width)//Width грубо говоря размер того что будет выводится на екран, а точнее размер самого текста тип если поставим 5 - будет выводится по 5 символом с нашей строки
{
    int buffer_x_size, buffer_y_size;                                               //Получаем размеры окна консоли
    string LastLine = "";
    DatabaseInfo DatabaseInfo1;
    DatabaseInfo1.GetConsBuff(buffer_x_size, buffer_y_size);
    size_t lbuffer_x_size = buffer_x_size;
    size_t LeftBordForColumns = (lbuffer_x_size - width) / 2.;                                           //Устанавливаем левый отступ

    for (;;)        //Делаем бесконечный цикл
    {
        //Левый отступ в деле
        //cout.fill(' '); по умолчанию
        cout << setw(LeftBordForColumns) << "";
        //Выделение подстроки и вывод её на консоль
        cout << pDatabaseColumnsInfo.substr(0u, width) /*<< endl*/;
        cout << setw(LeftBordForColumns) << "" << LastLine << endl;
        //Удаление из строки той части, что вывели на консоль
        //(s.size() < width ? s.size() : width) чтобы не выйти за пределы контейнера - проверка
        pDatabaseColumnsInfo.erase(pDatabaseColumnsInfo.begin(), pDatabaseColumnsInfo.begin() + (pDatabaseColumnsInfo.size() < width ? pDatabaseColumnsInfo.size() : width));
        //Если строку всю потратили - заканчиваем бесконечный цикл break;
        if (pDatabaseColumnsInfo.empty())
        {
            break;
        }
    }
}

Node* DatabaseInsert::begin(void) { return head; }
size_t DatabaseInsert::size(void) const { return cnt; }
bool  DatabaseInsert::empty(void) const { return (head == NULL); }