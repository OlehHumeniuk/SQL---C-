#include "DatabaseInfo.h"

//Функция которая возвращает нам строки после заполнения символами
string DatabaseInfo::GoodLine(int pSizeOfLine, string* pLine, string pElement)
{
    for (int i = 0; i < pSizeOfLine; i++)
    {
        *pLine = *pLine + pElement;
    }
    return *pLine;
}
//Добавление 
Knot* DatabaseInfo::AddColumn(string pDatabaseName, string pDatabaseColumn, string pColumnTypeOfData) {
    Knot* NewNode = new (std::nothrow) Knot();
    if (NewNode != NULL) {
        NewNode->DatabaseName = pDatabaseName;          // Инициализация имени бд куда записываем новый столбик
        NewNode->DatabaseColumn = pDatabaseColumn;       // Инициализация имени колонки
        NewNode->ColumnTypeOfData = pColumnTypeOfData;   // Инициализация типа данных колонки

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
Knot* DatabaseInfo::Insert(Knot* pos, string pDatabaseName, string pDatabaseColumn, string pColumnTypeOfData) {
    if (pos == NULL)
        return pos;

    Knot* x = head, * y = head;
    while (x != pos) {
        y = x;
        x = x->next;
    }

    Knot* NewNode = new (std::nothrow) Knot();
    if (NewNode != NULL) {
        NewNode->DatabaseName = pDatabaseName;          // Инициализация имени бд куда записываем новый столбик
        NewNode->DatabaseColumn = pDatabaseColumn;       // Инициализация имени колонки
        NewNode->ColumnTypeOfData = pColumnTypeOfData;   // Инициализация типа данных колонки

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
Knot* DatabaseInfo::Erase(Knot* pos) {
    if (pos == NULL)
        return pos;
    else if (pos == head) {
        head = head->next;
        if (head == NULL)
            tail = NULL;
    }
    else {
        Knot* x = head, * y = head;
        while (x != pos) {
            y = x;
            x = x->next;
        }
        y->next = pos->next;

        if (pos == tail)
            tail = y;
    }

    Knot* p = pos->next;
    delete pos;
    --cnt;
    return p;
}

//Удаление из головы
void DatabaseInfo::DeleteFirstAddElement(void) { Erase(head); }

//Удаление из хвоста за O(n) "опасно" медленно
void DatabaseInfo::DeleteLastAddElement(void) { Erase(tail); }

//Удаление всех
void DatabaseInfo::DeleteAllDatabases(void) {
    Knot* t;
    while (head != NULL) {
        t = head;
        head = head->next;
        delete t;
    }
    tail = NULL;
    cnt = 0;
}

//Получение размеров окна консоли без прокрутки    
void DatabaseInfo::GetConsBuff(int& x, int& y)
{
    HANDLE hWndConsole;        //Тут я объявляю дескриптор объекта, и после инициализация дескриптора hWndConsole, он нам позволяет обратиться к консоле
    if (hWndConsole = GetStdHandle(-12))
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;            //Переменная, которая будет хранить всю информацию о консоле
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))            //Просим заполнить информацию о консоле из дескриптора
        {
            //Ищем размеры окна консоли
            //x - горизонт, y - вертикаль(вроде логично)
            x = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
            y = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
        }
        else
            printf("Error: %d\n", GetLastError());//Вывод ошибки если что-то пойдет не так
    }
    else
        printf("Error: %d\n", GetLastError());//Вывод ошибки если что-то пойдет не так
}

//Функция размещения таблицы по центру конольного приложения
void DatabaseInfo::CenterString(string& pDatabaseName, string& pDatabaseColumns, const size_t& width, string* pLastLine)//Width грубо говоря размер того что будет выводится на екран, а точнее размер самого текста тип если поставим 5 - будет выводится по 5 символом с нашей строки
{
    int buffer_x_size, buffer_y_size;                                               //Получаем размеры окна консоли
    string IndentForDatabaseName = "";
    string RightIndentForDatabaseName = "";
    string lLastLine = "";
    string lLeftBordForColumns = "";
    GetConsBuff(buffer_x_size, buffer_y_size);
    size_t LeftBordForColumns = (buffer_x_size - width) / 2.;                                           //Устанавливаем левый отступ
    size_t LeftBordForDatabaseName = (width - pDatabaseName.length()) / 2.;                             //Устанавливаем левый отступ для имени бд
    size_t RightBordForDatabaseName = width - LeftBordForDatabaseName - pDatabaseName.length();         //Устанавливаем правый отступ для имени бд

    GoodLine(LeftBordForDatabaseName, &IndentForDatabaseName, "-");
    GoodLine(RightBordForDatabaseName, &RightIndentForDatabaseName, "-");
    GoodLine(width, &lLastLine, "-");

    for (;;)        //Делаем бесконечный цикл
    {
        cout << setw(LeftBordForColumns) << "";                                         //Левый отступ в деле
        cout << setw(LeftBordForDatabaseName) << IndentForDatabaseName;                 //Левый отступ перед именем в деле
        cout << pDatabaseName.substr(0u, width);                                        //Выделение подстроки и вывод её на консоль
        cout << setw(LeftBordForDatabaseName) << RightIndentForDatabaseName << endl;    //Правый отступ после имени в деле
        pDatabaseName.erase(pDatabaseName.begin(), pDatabaseName.begin() + (pDatabaseName.size() < width ? pDatabaseName.size() : width));
        //Если строку всю потратили - заканчиваем бесконечный цикл break;
        if (pDatabaseName.empty())
        {
            break;
        }
    }
    for (int lCounter = 0; lCounter < LeftBordForColumns; lCounter++)//получаем отступ
    {
        lLeftBordForColumns = lLeftBordForColumns + " ";
    }
    *pLastLine = lLeftBordForColumns + lLastLine + "\n";//получаем последнюю строку
    for (;;)        //Делаем бесконечный цикл
    {
        //Левый отступ в деле
        //cout.fill(' '); по умолчанию
        cout << setw(LeftBordForColumns) << "";
        //Выделение подстроки и вывод её на консоль
        cout << pDatabaseColumns.substr(0u, width) << endl;
        cout << setw(LeftBordForColumns) << "" << lLastLine << endl;
        //Удаление из строки той части, что вывели на консоль
        //(s.size() < width ? s.size() : width) чтобы не выйти за пределы контейнера - проверка
        pDatabaseColumns.erase(pDatabaseColumns.begin(), pDatabaseColumns.begin() + (pDatabaseColumns.size() < width ? pDatabaseColumns.size() : width));
        //Если строку всю потратили - заканчиваем бесконечный цикл break;
        if (pDatabaseColumns.empty())
        {
            break;
        }
    }
}

//Функция отображения всех столбиков нашей бд
void DatabaseInfo::ShowDatabase(string pDatabaseColumns, string pDatabaseName, string* pLastLine)
{
    setlocale(LC_ALL, "rus"); // установка локали
    cout << "База данных выглядит так: " << endl;
    CenterString(pDatabaseName, pDatabaseColumns, pDatabaseColumns.length(), pLastLine);
}

//Поиск всех столбцов БД
void DatabaseInfo::FindAllDatabaseColumns(const DatabaseInfo& pDatabaseInfo, string pDatabaseName)
{
    string DatabaseColumns = "";
    for (const Knot* p = pDatabaseInfo.begin(); p != NULL; p = p->next)
    {
        if (p->DatabaseName == pDatabaseName)
        {
            DatabaseColumns = DatabaseColumns + "|" + p->DatabaseColumn;
        }
    }
    DatabaseColumns = DatabaseColumns + "|";
    string lLastLine = "";
    ShowDatabase(DatabaseColumns, pDatabaseName,&lLastLine);
}

Knot* DatabaseInfo::begin(void) { return head; }
Knot* DatabaseInfo::begin(void) const { return head; }

size_t DatabaseInfo::size(void) const { return cnt; }
bool  DatabaseInfo::empty(void) const { return (head == NULL); }
