#include "SelectTable.h"
#include "PrintTable.h"
void ColumnChecker(map<int, string> pDatabaseColumnsName, string pTableName, const DatabaseInfo& pDatabaseInfo, bool* pCheck)
{
    map<int, string>::iterator lIterator;
    for (lIterator = pDatabaseColumnsName.begin(); lIterator != pDatabaseColumnsName.end(); lIterator = next(lIterator))
    {
        for (const Knot* p = pDatabaseInfo.begin(); p != NULL; p = p->next)
        {
            if ((p->TableName == pTableName) && (p->ColumnName == lIterator->second))
            {
                *pCheck = 1;
                break;
            }
        }
    }
}
void AnaliseFrom(string pAllComand, string* pComand, string* pTableName)
{
    string Word = "";
    int count = 0;

    if (pAllComand[0] != '\0')
    {
        if (pAllComand[0] != ' ') { ++count; }
        for (int i = 0; pAllComand[i] != '\0'; i++)
        {
            Word = Word + pAllComand[i];
            if (pAllComand[i + 1] == ' ' || pAllComand[i + 1] == '\0')
            {
                std::cout << Word << std::endl;
                if (count == 1) { *pComand = Word; }
                else { *pTableName = Word; break; }
                Word.clear();
                i++;
                ++count;
            }
        }
    }
    std::cout << "Count: " << count << '\n';
}
//SELECT cat_id, cat_owner_id FROM cats WHERE name = “Murzik”;
//Комнда для запису наших данних по WHERE
void WHEREComand(vector< pair<string, string> > *lAllpartsOfCommand, string pCommand,int pCounter, string pFullCommand)
{
    lAllpartsOfCommand->push_back(pair<string, string>("Comand", "WHERE"));
    pCommand.clear();
    pCounter++;
    for (pFullCommand[pCounter]; pFullCommand[pCounter] != ' '; pCounter++)
    {
        pCommand = pCommand + pFullCommand[pCounter];
        if (pFullCommand[pCounter + 1] == ' ' || pFullCommand[pCounter + 1] == '=')
        {
            lAllpartsOfCommand->push_back(pair<string, string>("ColumnName", pCommand));
            pCommand.clear();
            if (pFullCommand[pCounter + 1] == ' ')
            {
                if (pFullCommand[pCounter + 3] == ' ')
                {
                    pCommand = pFullCommand[pCounter + 2];
                    pCounter = pCounter + 4;
                    lAllpartsOfCommand->push_back(pair<string, string>("Sign", pCommand));
                }
                else if (pFullCommand[pCounter + 4] == ' ')
                {
                    pCommand = pFullCommand[pCounter + 2] + pFullCommand[pCounter + 3];
                    pCounter = pCounter + 5;
                    lAllpartsOfCommand->push_back(pair<string, string>("Sign", "!="));
                }
            }
            else if (pFullCommand[pCounter + 1] == '=')
            {
                if (pFullCommand[pCounter + 2] == ' ')
                {
                    pCommand = pFullCommand[pCounter + 1];
                    pCounter = pCounter + 3;
                    lAllpartsOfCommand->push_back(pair<string, string>("Sign", pCommand));
                }
                else if (pFullCommand[pCounter + 3] == ' ')
                {
                    pCommand = pFullCommand[pCounter + 1] + pFullCommand[pCounter + 2];
                    pCounter = pCounter + 4;
                    lAllpartsOfCommand->push_back(pair<string, string>("Sign", "!="));
                }
            }
            pCommand.clear();
            for (pFullCommand[pCounter]; pFullCommand[pCounter] != ';'; pCounter++)
            {
                if (pFullCommand[pCounter] == '\"')
                {
                    pCounter++;
                    for (pFullCommand[pCounter]; pFullCommand[pCounter] != '\"'; pCounter++)
                    {
                        pCommand = pCommand + pFullCommand[pCounter];
                        if(pFullCommand[pCounter + 1] == '\"')
                        { 
                            lAllpartsOfCommand->push_back(pair<string, string>("ColumnData", pCommand));
                            return;
                        }
                    }
                }
            }
        }
    }
}
//Комнда для запису наших данних по ON 
//SELECT owner_name, cat_name FROM owners FULL_JOIN cats ON owner_id = cat_owner_id;
void ONComand(vector< pair<string, string> >* lAllpartsOfCommand, string pCommand, int pCounter, string pFullCommand)
{
    lAllpartsOfCommand->push_back(pair<string, string>("Comand", "ON"));
    pCommand.clear();
    for (pFullCommand[pCounter]; pFullCommand[pCounter] != ' '; pCounter++)
    {
        pCommand = pCommand + pFullCommand[pCounter];
        if (pFullCommand[pCounter + 1] == ' ' || pFullCommand[pCounter + 1] == '=')
        {
            lAllpartsOfCommand->push_back(pair<string, string>("ColumnName_1", pCommand));
            if      (pFullCommand[pCounter + 1] == ' ')
            {
                if (pFullCommand[pCounter + 3] == ' ')
                {
                    pCommand = pFullCommand[pCounter + 2];
                    pCounter = pCounter + 4;
                    lAllpartsOfCommand->push_back(pair<string, string>("Sign", pCommand));
                }
            }
            else if (pFullCommand[pCounter + 1] == '=')
            {
                if (pFullCommand[pCounter + 2] == ' ')
                {
                    pCommand = pFullCommand[pCounter + 1];
                    pCounter = pCounter + 3;
                    lAllpartsOfCommand->push_back(pair<string, string>("Sign", pCommand));
                }
            }
            pCommand.clear();
            for (pFullCommand[pCounter]; pFullCommand[pCounter] != '\"'; pCounter++)
            {
                if (pFullCommand[pCounter] != ';')
                {
                    pCommand = pCommand + pFullCommand[pCounter];
                    if (pFullCommand[pCounter+1] == ';' || (pFullCommand[pCounter + 1] == ' ' && pFullCommand[pCounter + 2] == ';'))
                    {
                        lAllpartsOfCommand->push_back(pair<string, string>("ColumnName_2", pCommand));
                        return;
                    }
                }
            }
        }
    }
}
vector< pair<string, string> > AnaliseHelpper(string pFullCommand, int pNumberCurrentItemString)
{
    vector< pair<string, string> > lAllpartsOfCommand;
    string lCommand = "";
    int lCounter = 0;
    for (pFullCommand[pNumberCurrentItemString]; pFullCommand[pNumberCurrentItemString] != ';' || pFullCommand[pNumberCurrentItemString] != ' '; pNumberCurrentItemString++ )
    {
        lCommand = lCommand + pFullCommand[pNumberCurrentItemString];
        if (pFullCommand[pNumberCurrentItemString + 1] == ' ')
        {
            if      (pFullCommand[pNumberCurrentItemString + 2] == 'W' && pFullCommand[pNumberCurrentItemString + 3] == 'H' && pFullCommand[pNumberCurrentItemString + 4] == 'E' && pFullCommand[pNumberCurrentItemString + 5] == 'R' && pFullCommand[pNumberCurrentItemString + 6] == 'E')
            {
                lCounter = pNumberCurrentItemString + 7;
                lAllpartsOfCommand.push_back(pair<string, string>("NameTable", lCommand));
                WHEREComand(&lAllpartsOfCommand, lCommand, lCounter, pFullCommand);
                return lAllpartsOfCommand;
            }
            //SELECT owner_name, cat_name FROM owners FULL_JOIN cats ON owner_id = cat_owner_id;
            else if (pFullCommand[pNumberCurrentItemString + 2] == 'F' && pFullCommand[pNumberCurrentItemString + 3] == 'U' && pFullCommand[pNumberCurrentItemString + 4] == 'L' && pFullCommand[pNumberCurrentItemString + 5] == 'L' && pFullCommand[pNumberCurrentItemString + 6] == '_' && pFullCommand[pNumberCurrentItemString + 7] == 'J' && pFullCommand[pNumberCurrentItemString + 8] == 'O' && pFullCommand[pNumberCurrentItemString + 9] == 'I' && pFullCommand[pNumberCurrentItemString + 10] == 'N')
            {
                lAllpartsOfCommand.push_back(pair<string, string>("NameTable", lCommand));
                lAllpartsOfCommand.push_back(pair<string, string>("Comand", "FULL_JOIN"));
                lCommand.clear();
                lCounter = pNumberCurrentItemString + 12;
                for (pFullCommand[lCounter]; pFullCommand[lCounter] != ';'; lCounter++)
                {
                    lCommand = lCommand + pFullCommand[lCounter];
                    if (pFullCommand[lCounter + 1] == ' ' && pFullCommand[lCounter + 2] == 'W' && pFullCommand[lCounter + 3] == 'H' && pFullCommand[lCounter + 4] == 'E' && pFullCommand[lCounter + 5] == 'R' && pFullCommand[lCounter + 6] == 'E')
                    {
                        lCounter = pNumberCurrentItemString + 7;
                        lAllpartsOfCommand.push_back(pair<string, string>("NameTable", lCommand));
                        WHEREComand(&lAllpartsOfCommand, lCommand, lCounter, pFullCommand);
                        return lAllpartsOfCommand;
                    }
                    else if (pFullCommand[lCounter + 1] == ' ' && pFullCommand[lCounter + 2] == 'O' && pFullCommand[lCounter + 3] == 'N')
                    {
                        lCounter = lCounter + 5;
                        lAllpartsOfCommand.push_back(pair<string, string>("NameTable", lCommand));
                        ONComand(&lAllpartsOfCommand, lCommand, lCounter, pFullCommand);
                        return lAllpartsOfCommand;
                    }
                }
            }
        }
    }
}
void AnalizArgumentsOfComandISelect(string pComand,DatabaseInfo *gDatabaseInfo , DatabaseInsert *gDatabaseInsert)
{
    string  NameVariable = ""; //то куда мы записываем имя столбца
    int NumberColumn = 1, lNCISC = 0;//lNumberCurrentItemStringCommand
    bool NameColumns = 1, pCheck = 0;
    map<int, string> DatabaseColumnsName;
    string  TypeOfData; //то куда мы записываем тип данных таблицы
    while (pComand[lNCISC + 1])// пока не конец строки
    {
        if ((pComand[lNCISC + 1] == 'f' && pComand[lNCISC + 2] == 'r' && pComand[lNCISC + 3] == 'o' && pComand[lNCISC + 4] == 'm') || (pComand[lNCISC + 1] == 'F' && pComand[lNCISC + 2] == 'R' && pComand[lNCISC + 3] == 'O' && pComand[lNCISC + 4] == 'M'))
        {
            string lFullCommand = "", lComand = "";
            NumberColumn = 0; 
            bool lCheckTableName;
            vector< pair<string, string> > AllpartsOfCommand = AnaliseHelpper(pComand, lNCISC + 6);
            if (AllpartsOfCommand.size() == 7)
            {
                ColumnChecker(DatabaseColumnsName, AllpartsOfCommand[0].second, *gDatabaseInfo, &pCheck);
                if (next(DatabaseColumnsName.begin())->second == "*")
                { 
                    map<int, string> ::iterator lIter = DatabaseColumnsName.begin();
                    lIter = DatabaseColumnsName.erase(lIter);
                    pCheck = 1; 
                }
                else { ColumnChecker(DatabaseColumnsName, AllpartsOfCommand[2].second, *gDatabaseInfo, &pCheck); }
                lCheckTableName = CheckForAvailability(AllpartsOfCommand[0].second, *gDatabaseInfo);
                lCheckTableName = CheckForAvailability(AllpartsOfCommand[2].second, *gDatabaseInfo);
            }
            else
            {
                ColumnChecker(DatabaseColumnsName, AllpartsOfCommand[0].second, *gDatabaseInfo, &pCheck);
                if (next(DatabaseColumnsName.begin())->second == "*")
                {
                    map<int, string> ::iterator lIter = DatabaseColumnsName.begin();
                    lIter = DatabaseColumnsName.erase(lIter);
                    pCheck = 1;
                }
                lCheckTableName = CheckForAvailability(AllpartsOfCommand[0].second, *gDatabaseInfo);
            }
            if (pCheck == 1)
            {
                PrintOutDatabaseInfoHellper(AllpartsOfCommand, lCheckTableName, DatabaseColumnsName, gDatabaseInfo, gDatabaseInsert);
                return;
            }
            else { return; }
        }
        if (pComand[lNCISC] != ' ' && pComand[lNCISC] != ',')
        {
            NameVariable = NameVariable + pComand[lNCISC];
            if (pComand[lNCISC + 1] == ' ' || pComand[lNCISC + 1] == ',')
            {
                if (NameColumns == 1) { DatabaseColumnsName.insert(make_pair(NumberColumn, NameVariable)); }
                NumberColumn++;
                NameVariable.clear();
            }
        }
        lNCISC++;
    }
}
void AnaliseHelpperFrom(string pFullCommand, int pNumberCurrentItemString, string* lFullCommand)
{
    for (pFullCommand[pNumberCurrentItemString]; pFullCommand[pNumberCurrentItemString] != ';'; pNumberCurrentItemString++)
    {
        *lFullCommand = *lFullCommand + pFullCommand[pNumberCurrentItemString];
    }
    return;
}
void SelectTable(string pFullCommand, int pNumberCurrentItemString, DatabaseInfo *gDatabaseInfo, DatabaseInsert *gDatabaseInsert)
{
    string  lNameTable = "";
    bool lCheck = 0, lCheckTableName, pCheck = 0;
    int lQuantityOfDatabaseElements = 0;
    string  lComand;// различные переменные которые используем для проверки команды на правильную структуру
    map<int, map<int, string>> lValuesComand;
    map<int, string> lDatabaseColumnsName;
    while (pFullCommand[pNumberCurrentItemString])                           // пока не конец буфера
    {
        bool lLastSymphol = CheckForTheLastCharacter(pFullCommand);
        if (lLastSymphol == 0) { return; }
        if (pFullCommand[pNumberCurrentItemString] != ' ')
        {
            map<int, string> lTemporaryMapForValues;
            if (pFullCommand[pNumberCurrentItemString] == '*' && pFullCommand[pNumberCurrentItemString + 1] == ' ')
            {
                string lFullCommand = "", lComand = "";
                AnaliseHelpperFrom(pFullCommand, pNumberCurrentItemString + 2, &lFullCommand);
                AnaliseFrom(lFullCommand, &lComand, &lNameTable);
                vector< pair<string, string> > lNameTableVector;
                lNameTableVector.push_back(pair<string, string>("NameTable", lNameTable));
                std::regex twoormorespaces(" [ ]+");
                cout << "Наша команда после обработки: " << endl << std::regex_replace(lFullCommand, twoormorespaces, " ") << endl;
                if (lComand == "from") { PrintOutDatabaseInfoHellper(lNameTableVector, lCheck, lDatabaseColumnsName, gDatabaseInfo, gDatabaseInsert); }
                else if (lComand == "FROM")
                {
                    AnalizArgumentsOfComandISelect(pFullCommand,  gDatabaseInfo, gDatabaseInsert);
                }
                return;
            }
            else
            {
                int lCounter = pNumberCurrentItemString;
                string lFullCommand = "", lComand = "";
                for (pFullCommand[lCounter]; pFullCommand[lCounter] != '\0'; lCounter++)
                {
                    lFullCommand = lFullCommand + pFullCommand[lCounter];
                }
                AnalizArgumentsOfComandISelect(lFullCommand, gDatabaseInfo, gDatabaseInsert);
                return;
            }
        }
        pNumberCurrentItemString++;
    }
}