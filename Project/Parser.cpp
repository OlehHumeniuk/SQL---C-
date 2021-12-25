#include "Parser.h"

string GetName(string pFullCommand, int lLength)
{
    string lTableNme = "";
    for (int i = 0; pFullCommand[lLength + i + 2] != ';'; i++)
    {
        lTableNme = lTableNme + pFullCommand[lLength + i + 2];
    }
    return lTableNme;
}
void check(string pComandString, string pFullCommand, int pNumberCurrentItemString , DatabaseInfo *pDatabaseInfo, DatabaseInsert *pDatabaseInsert, map <string, int> *TablesLinesInfo)
{
    // Массив строк типа string
    // 1. Инициализация массива строк указанием размера массива
    const int lNumberOfCommands = 6;
    string lCommands[lNumberOfCommands] = { "create", "insert", "select","delete","print","exit" };

    for (int pNumberOfCommands = 0; pNumberOfCommands < lNumberOfCommands; pNumberOfCommands++) // Проверка на то являеться ли полученая строка одной из команд. если да то выполняем действия етой команды
    {
        if (pComandString == lCommands[0])//команда Create
        {
            cout << "Create your table" << endl;
            CreateTable(pFullCommand, pNumberCurrentItemString, pDatabaseInfo);
            break;
            return;
        }
        else if (pComandString == lCommands[1])//команда Insert
        {
            cout << "Insert into your table!" << endl;
            InsertTable(pFullCommand, pNumberCurrentItemString, pDatabaseInfo, pDatabaseInsert, TablesLinesInfo);
            break;
            return;
        }
        else if (pComandString == lCommands[2])
        {
            cout << "Select your values!" << endl;
            SelectTable(pFullCommand, pNumberCurrentItemString, pDatabaseInfo, pDatabaseInsert);
            break;
            return;
        }
        else if (pComandString == lCommands[3])
        {
            string lTableNme = GetName(pFullCommand, pComandString.length());
            DeleteTable(lTableNme, pDatabaseInfo, pDatabaseInsert);
            break;
            return;
        }
        else if (pComandString == lCommands[4])
        {
            PrintOutDatabaseInfo(pFullCommand, pNumberCurrentItemString, pDatabaseInfo, pDatabaseInsert);
            break;
            return;
        }
        else if (pComandString == lCommands[5])
        {
            system("pause");
        }
    }
}
//Анализируем команды 
void AnaliseComand(char buffer[255], DatabaseInfo* pDatabaseInfo, DatabaseInsert* pDatabaseInsert, map <string, int>* TablesLinesInfo)//функцыя анализа нашей строки
{
    string lFullCommand = "", pFullCommand = "", lTolowerComand;
    int lNumberCurrentItemString = 0, pNumberCurrentItem = 0, pIndex = 0;
    char lCurrentItem;              // поточный елемент в строке
    char lTolowerCommandElementChar;// перменная в которую мы записиваем елемент команды
    string lTolowerCommand;         // Строка в которую записиваем значения переведенной команды

    while (buffer[lNumberCurrentItemString] && true)                           // пока не конец буфера проверяем нашу строку на возможные команды
    {
        if (buffer[lNumberCurrentItemString] != ' ')//пока не встретим пробел записуем елемент в строку и потом проверяем является ли ето одной из возможных команд
        {
            lTolowerComand = lTolowerComand + buffer[lNumberCurrentItemString];
            if (buffer[lNumberCurrentItemString + 1] == ' ' || buffer[lNumberCurrentItemString + 1] == ';')
            {
                for (int index = 0; index < lTolowerComand.length(); ++index)         // меняем регистр именно для команды
                {
                    //в окрему функцію яка робить щось
                    while (lTolowerComand[pNumberCurrentItem])                        // пока не конец строки в которую записали команду
                    {
                        lCurrentItem = lTolowerComand[pNumberCurrentItem];
                        lTolowerCommandElementChar = tolower(lCurrentItem);
                        lTolowerCommand = lTolowerCommand + lTolowerCommandElementChar;//переводим всю строку в маленькие буквы для 100% коректного понимания програмы того что мы ввели
                        pNumberCurrentItem++;
                        if (pNumberCurrentItem == lTolowerComand.length())
                        {
                            lCurrentItem = lTolowerComand[pNumberCurrentItem];
                            lTolowerCommandElementChar = tolower(lCurrentItem);
                            lFullCommand = lTolowerCommand;                             // переводим команду в маленькие буквы для 100 % коректного понимания програмы того что мы ввели
                            std::cout << lTolowerCommand << endl;
                            pIndex = lNumberCurrentItemString + 1;
                            break;
                        }
                    }
                    if (pNumberCurrentItem == lTolowerComand.length()) break;
                }
                for (pIndex; pIndex <= strlen(buffer); pIndex++)
                {
                    pFullCommand = pFullCommand + buffer[pIndex];
                }
                lNumberCurrentItemString++;
                std::regex twoormorespaces(" [ ]+");
                lFullCommand = lFullCommand + " " + pFullCommand;
                cout << "Наша команда после обработки: " << endl << std::regex_replace(lFullCommand, twoormorespaces, " ") << endl;
                lNumberCurrentItemString = size(lTolowerCommand);
                check(lTolowerCommand, lFullCommand, lNumberCurrentItemString, pDatabaseInfo, pDatabaseInsert, TablesLinesInfo);//добавить к толовер команд значения что идут после в буфере, что бы оно могло норм обработать их
                break;
            }
        }
        lNumberCurrentItemString++;
    }
}