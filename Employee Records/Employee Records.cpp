#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

struct staffPersonal
{
    string surname;
    string initials;
    string job;
    int hireYear;
    int income;
};


/**
 * @brief Этот метод проверяет, правильно ли введено пользователем количество методов, которое может принять команда.
 *
 * @param validParamCount - количество параметров, которые может принять команда.
 * @param rightSyntax - напоминание пользователю о правильном написании команды.
 * @param params - вектор введённых параметров.
 * @return Возвращает True, если количество параметров на входе не соответствует количеству ожидаемых параметров.
 */
bool CheckParamsCount(const int& validParamCount, const string& rightSyntax, const vector<string>& params)
{
    if (params.size() != validParamCount)
    {
        cout << "\nВведено неверное количество параметров! Обратите внимание на синтаксис этой команды: " << rightSyntax << endl;
        return true;
    }
    return false;
}

/**
 * @brief Этот метод выводит ошибку о том, что пользователь ввёл неверный параметр.
 * 
 * @param rightSyntax - напоминание пользователю о правильном написании команды.
 */
void printParamInputError(const string& rightSyntax)
{
    cout << "\nВведён неверный параметр. Обратите внимание на синтаксис этой команды: " << rightSyntax << endl;
    return;
}

int main()
{
    setlocale(LC_ALL, "Russian"); // Установка русской локали

    ifstream CheckStaff("data/staff-db-current.txt");

    if(CheckStaff.is_open())
    {
        cout << "База данных сотрудников открыта!" << endl;
        cout << "Вывод информации о штате:\n" << endl;
        string staffLine;
        int startFromLine = 3;
        // Создаём массив для хранения двух линий, которые представляют собой заголовок и разделитель
        // (С третьей линии начинается информация о сотрудниках)
        string* staffHead = new string[startFromLine - 1]; 

        // Считывание заголовка таблицы
        for (int i = 0; i < startFromLine - 1; i++)
        {
            getline(CheckStaff, staffHead[i]);
            cout << staffHead[i] << endl;
        }

        // Считывание значимых строк в таблице и их запись в вектор
        vector<staffPersonal> worker;

        while (getline(CheckStaff, staffLine))
        {
            // Создаём вспомогательную переменную и массив
            int clauseNum = 0;
            string aboutEmployee[5];

            for (int i = 0; i < staffLine.length(); i++)
            {
                if (staffLine[i] != ' ') aboutEmployee[clauseNum] += staffLine[i];
                else if (staffLine[i] == ' ' && staffLine[i - 1] != ' ') clauseNum++;
            }

            worker.push_back({ aboutEmployee[0], aboutEmployee[1], aboutEmployee[2], stoi(aboutEmployee[3]), stoi(aboutEmployee[4]) });
        }

        // Обработка вывода текущего штата сотрудников
        for (size_t i = 0; i < worker.size(); i++)
        {
            string surnameStr = worker[i].surname;
            string initialsStr = worker[i].initials;
            string jobStr = worker[i].job;
            string hireYearStr = to_string(worker[i].hireYear);
            string incomeStr = to_string(worker[i].income);

            surnameStr.append(16 - surnameStr.length(), ' ');
            initialsStr.append(16 - initialsStr.length(), ' ');
            jobStr.append(16 - jobStr.length(), ' ');
            hireYearStr.append(16 - hireYearStr.length(), ' ');
            incomeStr.append(16 - incomeStr.length(), ' ');

            cout << surnameStr;
            cout << initialsStr;
            cout << jobStr;
            cout << hireYearStr;
            cout << incomeStr;
            cout << ":" << i + 1 << endl; // Порядок сотрудника
        }

        // ---------
        // Перечисление доступных команд
        enum Commands
        {
            _pep,
            _help,
            _edit,
            _add,
            _sortby,
            _check,
            _save,
            _cancel,
            _exit,
        };

        Commands commandPanel;
        // ---------

        while (1)
        {
            // Здесь будет храниться введённая команда и параметры, если прилагаются к команде
            string command;
            vector<string> params;

            // Ввод команды
            string commandInput;
            cout << "\nЧтобы узнать список доступных команд, напишите _help." << endl;
            cout << "Ввод > ";
            getline(cin, commandInput);

            if (commandInput.back() != ')') commandInput += " ()";

            // --------- Использование регулярного выражения для извлечения команды и параметров
            regex commandRegex(R"_(^(\S+)\s+\((\S+(?:\s+\S+)*)?\))_");

            // Хранение команды и параметров для утверждения соответствия
            smatch match;
            if (regex_search(commandInput, match, commandRegex))
            {
                // Извлечение команды
                command = match[1].str();

                // --- Debug ---
                // cout << match[0].str();
                // cout << match[1].str();
                // cout << match[2].str();

                // Извлечение параметров из скобок
                // * В match[0] хранится вся введённая строка
                // * В match[1] хранится отдельно команда перед скобками
                // * В match[2] хранится строка параметров, которая была введена в скобках
                string param;

                for (int i = 0; i < match[2].str().length(); ++i) {
                    if (match[2].str()[i] != ' ')
                    {
                        param += match[2].str()[i];
                    }
                    else if (match[2].str()[i] == ' ' && match[2].str()[i - 1] != ' ')
                    {
                        params.push_back(param);
                        param.clear();
                    }

                    if (i == match[2].str().length() - 1)
                    {
                        params.push_back(param);
                        param.clear();
                    }
                }

                // --- Debug ---
                // Вывод извлеченных параметров
                // cout << "Извлеченные параметры:" << endl;
                // for (const string& param : params)
                // {
                //     cout << param << endl;
                // }
            }
            // --------- Использование регулярного выражения для извлечения команды и параметров

            // Процедура совмещения правильных команд с пунктами enum'а
            if (command == "_help") commandPanel = Commands::_help;
            if (command == "_edit") commandPanel = Commands::_edit;
            if (command == "_add") commandPanel = Commands::_add;
            if (command == "_sortby") commandPanel = Commands::_sortby;
            if (command == "_check") commandPanel = Commands::_check;
            if (command == "_save") commandPanel = Commands::_save;
            if (command == "_cancel") commandPanel = Commands::_cancel;
            if (command == "_exit") commandPanel = Commands::_exit;

            switch (commandPanel)
            {
                case Commands::_help:
                    if (CheckParamsCount(0, "_help", params)) break;

                    cout << "\nСписок доступных команд:\n" << endl;
                    cout << "_edit (num) - редактировать информацию о сотруднике по его номеру в списке (num).\n" << endl;
                    cout << "_add (surname initials job hireYear income) - добавить сотрудника, указав информацию о нём.\n" << endl;
                    cout << "_sortby (surname / income / hireYear) - сортироавть список сотрудников по одному из выбранных параметров.\n" << endl;
                    cout << "_check (surname) - вывести информацию о сотруднике по его фамилии.\n" << endl;
                    cout << "_save (new) - Сохранить все изменения файла, перед этим выведя их в итоговом виде. Параметр (new) не является обязательным. С ним будет записан новый файл с текущей датой, без него главный файл будет перезаписан итоговой информацией.\n" << endl;
                    cout << "_cancel - сбросить все проделанные изменения.\n" << endl;
                    cout << "_exit - выйти из программы.\n" << endl;
                    break;

                case Commands::_edit:
                    if (CheckParamsCount(1, "_edit (num)", params)) break;

                    cout << "_edit посещён!" << endl;

                    break;

                case Commands::_add:
                    if (CheckParamsCount(5, "_add (surname initials job hireYear income)", params)) break;

                    cout << "_add посещён!" << endl;

                    break;

                case Commands::_sortby:
                    if (CheckParamsCount(1, "_sortby (surname / income / hireYear)", params)) break;

                    for (const string& param : params)
                    {
                        cout << "pep" << endl;

                        if (param == "surname")
                        {
                            cout << "_sortby посещён! Выбрана сортировка по фамилии." << endl;
                        }
                        else if (param == "income")
                        {
                            cout << "_sortby посещён! Выбрана сортировка по окладу." << endl;
                        }
                        else if (param == "hireYear")
                        {
                            cout << "_sortby посещён! Выбрана сортировка по стажу." << endl;
                        }
                        else
                        {
                            printParamInputError("_sortby (surname / income / hireYear)");
                            break;
                        }

                        break;
                    }

                    break;

                case Commands::_check:
                    if (CheckParamsCount(1, "_check (surname)", params)) break;

                    cout << "_check посещён!" << endl;

                    break;

                case Commands::_save:
                    // Если не был найден параметр new, программа просит подтвердить сохранение в актуальный файл
                    if (params.size() == 0)
                    {
                        int saveChoice;

                        cout << "Вы точно хотите сохранить таблицу в актуальный файл?" << endl;
                        cout << "1. Да, перезаписать актуальный файл." << endl;
                        cout << "2. Нет, не сохранять." << endl;

                        cout << "Ввод > ";
                        cin >> saveChoice;
                        cin.ignore();

                        if (saveChoice == 1)
                        {
                            cout << "\n_save посещён! Сохранение перезаписано." << endl;
                        }
                        else if (saveChoice == 2)
                        {
                            cout << "\n_save посещён! Файл не был сохранён." << endl;
                        }
                        else
                        {
                            cout << "\nВведён неверный вариант. Файл не сохранён." << endl;
                        }

                        break;
                    }
                    else if (CheckParamsCount(1, "_save (new)", params)) break;

                    // Если был найден параметр new, идёт сохранение таблицы в новый файл
                    for (const string& param : params)
                    {
                        if (param == "new")
                        {

                        }
                        else
                        {
                            printParamInputError("_save (new)");
                            break;
                        }
                    }

                    cout << "_save посещён!" << endl;

                    break;

                case Commands::_cancel:
                    if (CheckParamsCount(0, "_cancel", params)) break;

                    cout << "_cancel посещён!" << endl;

                    break;

                case Commands::_exit:
                    if (CheckParamsCount(0, "_exit", params)) break;

                    cout << "\nВыход из программы." << endl;
                    cout << "Спасибо, что посетили меня!" << endl;

                    return 0;

                    break;

                default:
                    cout << "\nКоманда введена неверно! Попробуйте ещё раз." << endl;
                    break;
            }
        }
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
