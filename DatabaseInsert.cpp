#include "DatabaseInsert.h"
int gCounter = 0;


//���������� 
Node* DatabaseInsert::AddColumn(string pDatabaseName, string pColumnName, string pColumnData, int pCounterOfLine)
{
    Node* NewNode = new (std::nothrow) Node();
    if (NewNode != NULL) {
        NewNode->TableName = pDatabaseName;          // ������������� ����� �� ���� ���������� ����� �������
        NewNode->ColumnName = pColumnName;              // ������������� ����� �������
        NewNode->ColumnData = pColumnData;              // ������������� ���������� ������� ��������� � ��
        NewNode->CounterOfLine = pCounterOfLine;        // ������������� �������� �����

        NewNode->next = NULL;

        if (head == NULL)
            head = tail = NewNode;
        else
            tail = tail->next = NewNode;
        ++cnt;
    }
    return NewNode;
}

//������� ����� pos
Node* DatabaseInsert::Insert(Node* pos, string pDatabaseName, string pColumnData, int pCounterOfLine) {
    if (pos == NULL)
        return pos;

    Node* x = head, * y = head;
    while (x != pos) {
        y = x;
        x = x->next;
    }

    Node* NewNode = new (std::nothrow) Node();
    if (NewNode != NULL) {
        NewNode->TableName = pDatabaseName;          // ������������� ����� �� ���� ���������� ����� �������
        NewNode->ColumnData = pColumnData;              // ������������� ���������� ������� ��������� � ��
        NewNode->CounterOfLine = pCounterOfLine;        // ������������� �������� �����

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

//�������� �� pos
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

//�������� �� ������
void DatabaseInsert::DeleteFirstAddElement(void) { Erase(head); }

//�������� �� ������ �� O(n) "������" ��������
void DatabaseInsert::DeleteLastAddElement(void) { Erase(tail); }

//�������� ����
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

//���������� ���� ������� ����� ���� ������
void DatabaseInsert::AddAllElementsDatabase(DatabaseInsert& lDatabaseInsert, int* pCounter, string pTableName, string pColumnName, string pColumnData)
{
    lDatabaseInsert.AddColumn(pTableName, pColumnName, pColumnData, *pCounter);
    (*pCounter)++;
}

//������� ���������� ������� �� ������ ���������� ����������
void DatabaseInsert::CenterString( string& pDatabaseColumnsInfo, const size_t& width)//Width ����� ������ ������ ���� ��� ����� ��������� �� �����, � ������ ������ ������ ������ ��� ���� �������� 5 - ����� ��������� �� 5 �������� � ����� ������
{
    int buffer_x_size, buffer_y_size;                                               //�������� ������� ���� �������
    string LastLine = "";
    DatabaseInfo DatabaseInfo1;
    DatabaseInfo1.GetConsBuff(buffer_x_size, buffer_y_size);
    size_t LeftBordForColumns = (buffer_x_size - width) / 2.;                                           //������������� ����� ������

    for (;;)        //������ ����������� ����
    {
        //����� ������ � ����
        //cout.fill(' '); �� ���������
        cout << setw(LeftBordForColumns) << "";
        //��������� ��������� � ����� � �� �������
        cout << pDatabaseColumnsInfo.substr(0u, width) /*<< endl*/;
        cout << setw(LeftBordForColumns) << "" << LastLine << endl;
        //�������� �� ������ ��� �����, ��� ������ �� �������
        //(s.size() < width ? s.size() : width) ����� �� ����� �� ������� ���������� - ��������
        pDatabaseColumnsInfo.erase(pDatabaseColumnsInfo.begin(), pDatabaseColumnsInfo.begin() + (pDatabaseColumnsInfo.size() < width ? pDatabaseColumnsInfo.size() : width));
        //���� ������ ��� ��������� - ����������� ����������� ���� break;
        if (pDatabaseColumnsInfo.empty())
        {
            break;
        }
    }
}

Node* DatabaseInsert::begin(void) { return head; }
Node* DatabaseInsert::begin(void) const { return head; }

size_t DatabaseInsert::size(void) const { return cnt; }
bool  DatabaseInsert::empty(void) const { return (head == NULL); }