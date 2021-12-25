#include <windows.h>
#include <map>
#include "Parser.h"

using namespace std;
int main()
{
    SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывод
    system("color 0A");// устанливаем цвет нашей консольки(а почему бы и нет?))
    setlocale(LC_ALL, "ru");//подключаем русский язык 


    //Оглашаем нашы 
    DatabaseInfo lDatabaseInfo;
    DatabaseInsert gDatabaseInsert;
    map <string, int> TablesLinesInfo;
    // Просим пользователя ввести строку
    for(;;)    
    {
        char buffer[255];
        std::cout << "Enter a string: ";
        std::cin.getline(buffer, 255);//записиваем то что мы вводили в командную строку в буфер
        AnaliseComand(buffer, &lDatabaseInfo, &gDatabaseInsert,&TablesLinesInfo);//анализируем наш буфер
        memset(buffer, 0, 255);//обнуляем буфер для того что бы получить следющию строку
    }
}