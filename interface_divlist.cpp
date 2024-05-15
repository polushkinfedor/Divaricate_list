#include "interface_divlist.h"

using namespace std;

//-------------------- программные единицы  взаимодействия с пользователем --------------------

void menu() {
    divaricate_list list;

    bool end_programm = false;

    while (not(end_programm)) {
        system("cls");
        set_color("You are working in a structure divaricate list interaction interface.\n\n", colors::white);
        set_color("struct data:\n\n", colors::blue);
        list.struct_check();
        cout << "\n\n";

        cout << "To enter struct data from console "; set_color("enter -- 1\n", colors::green);
        cout << "To delete data from struct enter "; set_color("enter -- 2\n", colors::green);
        cout << "To enter struct data from file "; set_color("enter -- 3\n", colors::green);
        cout << "To copy struct data to file "; set_color("enter -- 4\n\n", colors::green);
        cout << "To sort struct by flight numbers "; set_color("enter -- 5\n", colors::green);
        cout << "To find all flight numbers, that contain searching airport name "; set_color("enter -- 6\n\n", colors::green);
        cout << "To exit "; set_color("enter -- 0\n\n", colors::green);


        int user_enter;
        bool exit_flag = false;
        while (!exit_flag) {
            exit_flag = true;
            user_enter = _getch();

            switch (user_enter) {
            case one: // was entered 1
                struct_enter(list); // ввод данных в структуру
                break;
            case two: // was entered 2
                struct_delete(list); // удаление данных из структуры
                break;
            case three: // was entered 3
                struct_enter_ff(list); // заполнение структуры из файла
                break;
            case four: // was entered 4
                struct_load_tf(list); // загрузка структуры в файл
                break;
            case five: // was entered 5
                struct_sort(list); // сортировка структуры методом пузырька
                break;
            case six:
                struct_grouped(list); // сортировка структуры методом пузырька
                break;
            case zero: // was entered 0
                exit_message(end_programm); // подтверждение выхода из программы
                break;
            default:
                exit_flag = false;
            }
        }
    }
}

void struct_enter(divaricate_list& exemple) {
    int user_input, fl_number;
    bool is_data_entering = true;
    while (is_data_entering) {
        system("cls");

        const string incorrect_name = "0";
        const string back_str = "-1";
        const int incorrect_fl_num = -1;
        const int back = -2;

        set_color("struct data:\n\n", colors::blue);
        exemple.struct_check();
        cout << "\n\n";

        set_color("To input flight number enter only numbers\n", colors::purple);
        set_color("To input airport name enter 3 or 4 english letters (\"AAA\") or (\"AAAA\")\n\n", colors::purple);

        cout << "--To add element without branch -- enter 1\n"; // "and flight number\n";
        cout << "--To add element with branch -- enter 2\n"; // \", flight number, airport name1 and airport name2\n";
        cout << "--To add branch element -- enter 3\n"; // \", then flight number and then airport name\n";
        cout << "--To replace element -- enter 4\n"; // \", then flight number, which you need replace and flight number, to be replaced by\n";
        cout << "--To replace branch element -- enter 5\n\n"; // \", then flight number, which contains branch\nelement which you need replace, than this branch element and branch element, to be replaced by\n\n";
        set_color("To return to menu -- enter 0\n\n", colors::green);

        user_input = _getch();
        string name1, name2;
        switch (user_input) {
        case one: // was entered 1 (добавление элемента в конец структуры)
            fl_number = get_flight_number(); // получение номера полета
            if (!exemple.contain(fl_number)) { // номер полета не должен повторяться
                if (fl_number != incorrect_fl_num and fl_number != back) {
                    exemple.add(fl_number); // добавление в структуру коренного элемента
                }
            }
            else {
                el_repeat_message();
            }
            break;
        case two: // was entered 2 (добавление ветки в конец структуры)
            fl_number = get_flight_number();
            if (!exemple.contain(fl_number)) { // номер полета не должен повторяться
                if (fl_number != incorrect_fl_num and fl_number != back) { // если номер корректен
                    name1 = get_airport_name();
                    if (name1 != incorrect_name and name1 != back_str) { // если имя корректно
                        name2 = get_airport_name();
                        if (name2 != incorrect_name and name2 != back_str) {
                            exemple.add_branch(fl_number, name1, name2); // добавление ветки целиком
                        }
                    }
                }
            }
            else {
                el_repeat_message();
            }
            break;
        case three: // was entered 3 (добавление элемента в конец ветки для существующего элемента в структуре)
            fl_number = get_flight_number();
            if (exemple.contain(fl_number)) { // если в структуре существует элемент с искомым номером полета
                name1 = get_airport_name();
                if (name1 != incorrect_name) {
                    exemple(fl_number).add_child(name1); // добавить к существующему элементу дочерний элемент
                }
            }
            else if (fl_number != back and fl_number != incorrect_fl_num) {
                no_element_message();
            }
            break;
        case four: // was entered 4 (заменить fl_number для элемента структуры)
            user_input = get_flight_number();
            if (exemple.contain(user_input)) { // если структура содержит искомый элемент
                fl_number = get_flight_number();
                if (!exemple.contain(fl_number)) { // номер полета не должен повторяться
                    if (fl_number != incorrect_fl_num) {
                        exemple(user_input) = fl_number;
                    }
                }
                else if (fl_number != back and fl_number != incorrect_fl_num) {
                    el_repeat_message();
                }
            }
            else if (user_input != back and user_input != incorrect_fl_num) { // если в структуре нет искомого элемента
                no_element_message();
            }
            break;
        case five: // was entered 5 (заменить airport name для элемента ветки структуры)
            fl_number = get_flight_number();
            if (exemple.contain(fl_number)) { // если существует элемент с искомым номером полета
                name1 = get_airport_name();
                if (exemple(fl_number).contain(name1)) { // и существует элемент ветки с искомым именем аэропорта
                    name2 = get_airport_name();
                    if (name2 != incorrect_name and name2 != back_str) exemple(fl_number)(name1) = name2;
                }
            }
            else if (fl_number != back and fl_number != incorrect_fl_num) {
                no_element_message();
            }
            break;
        case zero:
            is_data_entering = false;
        }
    }
}

void struct_delete(divaricate_list& exemple) {
    int user_input, fl_number;
    bool is_data_entering = true;
    while (is_data_entering) {
        system("cls");

        const string incorrect_name = "0";
        const string back_str = "-1";
        const int incorrect_fl_num = -1;
        const int back = -2;

        set_color("struct data:\n\n", colors::blue);
        exemple.struct_check();
        cout << "\n\n";

        set_color("To input flight number enter only numbers\n", colors::purple);
        set_color("To input airport name enter 3 or 4 english letters (\"AAA\") or (\"AAAA\")\n\n", colors::purple);

        cout << "--To delete element with branch -- enter 1\n"; // \" and flight number of element\n";
        cout << "--To delete branch -- enter 2\n"; // \" and flight number of element, which contains deleting branch\n";
        cout << "--To delete element of branch -- enter 3\n"; // \", then flight number and then airport name\n";
        cout << "--To delete last element -- enter 4\n";
        cout << "--To delete last element from branch -- enter 5\n\n"; // \" and flight number of element,\nwhich contains deleting branch\n";
        set_color("--To erase struct -- enter 6\n\n", colors::red);
        set_color("To return to menu -- enter 0\n\n", colors::green);

        user_input = _getch();
        string second_input;
        switch (user_input) {
        case one: // was entered 1 (удаление элемента по значению с его веткой)
            fl_number = get_flight_number();
            if (exemple.contain(fl_number)) {
                exemple.del_element(fl_number);
            }
            else if (fl_number != back and fl_number != incorrect_fl_num) {
                no_element_message();
            }
            break;
        case two: // was entered 2 (удаление ветки для выбранного fl_number)
            fl_number = get_flight_number();
            if (exemple.contain(fl_number)) {
                exemple(fl_number).del_childs();
            }
            else if (fl_number != back and fl_number != incorrect_fl_num) {
                no_element_message();
            }
            break;
        case three: // was entered 3 (удаление элемента ветки для выбранного airport name)
            fl_number = get_flight_number();
            if (exemple.contain(fl_number)) {
                string airport_name = get_airport_name();
                if (exemple(fl_number).contain(airport_name)) {
                    exemple(fl_number).del_child(airport_name);
                }
                else if (airport_name != incorrect_name and airport_name != back_str) {
                    no_element_message();
                }
            }
            else if (fl_number != back and fl_number != incorrect_fl_num) {
                no_element_message();
            }
            break;
        case four: // was entered 4 (удаление последнего элемента)
            exemple.del_last();
            break;
        case five: // was entered 5 (удаление последнего элемента вектки для выбранного fl_number)
            fl_number = get_flight_number();
            if (exemple.contain(fl_number)) {
                exemple(fl_number).del_last();
            }
            else if (fl_number != back and fl_number != incorrect_fl_num) {
                no_element_message();
            }
            break;
        case six: // was entered 6 (удаление данных для структуры)
            exemple.clear();
        case zero:
            is_data_entering = false;
        }
    }
}

void struct_enter_ff(divaricate_list& exemple) {
    bool need_exit = false;
    while (not(need_exit)) {
        std::string file_name;
        system("cls");
        cout << "\n";
        cout << "Enter the name of the file located in the directories: ";
        set_color(filesystem::current_path().string() + "\n", colors::blue);
        set_color("To back to menu -- enter 0\n\n", colors::yellow);
        cout << "Please enter file name with file extension: ";
        getline(cin, file_name);
        delete_extraspace_string(file_name);
        if (file_name == "0") need_exit = true;
        else if (check_file_name(file_name)) { // если имя файла корректно
            if (check_file_exist(file_name)) { // если файл существует

                ifstream file(file_name, ios::app);
                if (correct_file(file)) exemple.inputFF(file); // заполнение структуры из корректного файла
                else incorrect_file_message();
                file.close();

                need_exit = true;
            }
            else { // если файл не существует
                system("cls");
                cout << "the program cannot find the specified file, make sure it exists and\n"
                    "is located in the same directory with the program"
                    "to continue, enter any key";
                _getch();
            }
        }
        else { // если имя файла не корректно
            system("cls");
            cout << "the entered file name is incorrect, make sure it does not contain the following characters or not empty:\n"
                " < >    :    \"    \\    /    |    ?    *\n"
                "to continue, enter any key";
            _getch();
        }
    }
}

void struct_load_tf(divaricate_list& exemple) {
    bool need_exit = false;
    while (not(need_exit)) {
        std::string file_name;
        system("cls");
        cout << "\n";
        cout << "Specify the name of the file where you are going to save the data from the structure,\n";
        cout << "It will be located in the directories: ";
        set_color(filesystem::current_path().string() + "\n", colors::blue);
        set_color("To back to menu -- enter 0\n\n", colors::yellow);
        cout << "Please enter file name with file extension: ";
        getline(cin, file_name); // получение названия файла
        if (file_name == "0") need_exit = true; // специальный код выхода, указанный в окне взаимодействия

        delete_extraspace_string(file_name);


        if (check_file_name(file_name)) { // если имя файла корректно
            check_file_exist(file_name); // получение полного имени для файла

            ofstream file(file_name, ios::app);
            exemple.outputTF(file); // заполнение структуры из файла
            file.close();

            need_exit = true;
        }
        else { // если имя файла не корректно
            system("cls");
            cout << "the entered file name is incorrect, make sure it does not contain the following characters:\n"
                " < >    :    \"    \\    /    |    ?    *\n"
                "to continue, enter any key";
            _getch();
        }
    }
}

void struct_sort(divaricate_list& exemple) {
    bool need_exit = false;
    while (not(need_exit)) {
        need_exit = true;
        system("cls");
        cout << "--To sort struct from lower to higher -- enter 1\n";
        cout << "--To sort struct from higher to lower -- enter 2\n";
        cout << "To exit "; set_color("enter -- 0\n\n", colors::green);

        int user_input = _getch();

        switch (user_input) {
        case one: // was entered 1 (сортировка от меньшего к большему)
            exemple.sort(true);
            sort_message();
            break;
        case two: // was entered 2 (сортировка от большего к меньшему)
            exemple.sort(false);
            sort_message();
            break;
        case zero: // was entered 0 (выход в меню)
            need_exit = true;
            break;
        default:
            need_exit = false;
        }
    }
}

void struct_grouped(divaricate_list& exemple) { // grouped
    bool need_exit = false;
    while (not(need_exit)) {
        need_exit = true;
        system("cls");
        cout << "--To grouped struct enter searching airport name.\n";
        cout << "(Airport name contain 3 or 4 english letters (\"AAA\") or (\"AAAA\")\n\n";

        exemple.struct_check();

        cout << "\n";

        string user_input = get_airport_name();

        if (user_input != "-1") {
            system("cls");
            cout << "grouped struct data:\n\n";
            exemple.struct_check(user_input);
            cout << "\nto continue -- enter any key";
            _getch();
            need_exit = false;
        }
    }
}

//-------------------------- информационные программные единицы --------------------------------

void exit_message(bool& exit_flag) {
    // меню подтверждения выхода из программы
    bool message_reload = true;
    while (message_reload) {
        message_reload = false;

        system("cls");
        cout << "Are you shure, that you want exit?\n";
        cout << "Y/N\n";

        char user_answer = _getch();

        if (user_answer == 'Y' or user_answer == 'y') exit_flag = true;
        else if (user_answer == 'N' or user_answer == 'n') exit_flag = false;
        else message_reload = true;
    }
}

void no_element_message() {
    // информационное сообщение об отсутствии элемента в структуре
    cout << "\nstruct doesn't contains this element\n";
    cout << "to continue, enter any key    ";
    _getch();
}

void sort_message() {
    // информационное сообщение об отсутствии элемента в структуре
    cout << "Sorting ended successfully\n";
    cout << "to continue, enter any key    ";
    _getch();
}

void incorrect_data_message(bool type) { // true -- ошибка в fl_num, false -- ошибка в airport_name
    // информационное сообщение о некорректности введенных данных
    set_color("you entered incorrect data\n", colors::purple);
    if (type) set_color("to input flight number enter only numbers\n", colors::purple);
    else set_color("to input airport name enter 3 or 4 english letters (\"AAA\") or (\"AAAA\")\n", colors::purple);

    _getch();
}

void el_repeat_message() {
    // информационное сообщение о повторяющемся элементе
    cout << "entered flight number repeated\n";
    cout << "to continue, enter any key    ";
    _getch();
}

void incorrect_file_message() {
    // информационное сообщение о не корректности файла
    cout << "file contains incorrect data, please change file data.\n";
    cout << "to continue, enter any key    ";
    _getch();
}

//-------------------------- логические программные единицы --------------------------------

bool flight_num_check(const std::string& fl_num) {
    for (const auto& symbol : fl_num) if (!isdigit(symbol)) return false; // если символ -- не цифра
    return true;
}

bool airport_name_check(std::string& airport_name) {
    if (not(airport_name.length() == 3 or airport_name.length() == 4)) return false; // проверка длины строки (доступные значения 3 и 4)

    for (auto& el : airport_name) { // корректности данных внутри строки (символы латинского алфавита)
        if (el <= 'z' and el >= 'a') el = toupper(el); // смена регистра
        else if (not(el <= 'Z' and el >= 'A')) { // если не символ латинского алфавита
            return false;
        }
    }

    return true;
}

bool check_file_name(const std::string& file_name) {
    // проверка на некорректные символы в названии файла
    vector<char> bad_symbols = {
            '<',
            '>',
            ':',
            '\"',
            '/' ,
            '\\',
            '|' ,
            '?',
            '*',
    };
    if (file_name == "") return false;
    for (auto sym : bad_symbols) if (file_name.find(sym) != -1) return false;
    return true;

}

bool check_file_exist(std::string& file_name) {
    // функция преобразует названия до полного пути до файла и возвращает статус существования
    if (file_name.find(".txt") != file_name.length()-4 or file_name.length()<4) file_name+=".txt";
    string path = filesystem::current_path().string();
    path += ("\\" + file_name);
    file_name = path;

    return filesystem::exists(path);
}

bool correct_file(std::ifstream& ifile) {
    std::string input_str;
    set<string> fl_numbers; // множество использованных номеров полетов, для невозможности повторения
    while (!ifile.eof()) {
        std::getline(ifile, input_str);
        if (input_str == "") break;

        string fl_number = input_str.substr(0, input_str.find(' ')); // парсинг строки вида "FL_NUM AN1 AN2"
        if (!flight_num_check(fl_number) or fl_numbers.find(fl_number) != fl_numbers.end()) { // если файл содержит некорректную информацию,  возвращаем некорректность файла
            ifile.seekg(0);
            return false;
        }
        fl_numbers.insert(fl_number);

        if (input_str.find(' ') != -1) {
            input_str.erase(0, input_str.find(' ') + 1);
            while (input_str.find(' ') != -1) {
                std::string test = input_str.substr(0, input_str.find(' '));
                if (!airport_name_check(test)) { // если файл содержит некорректную информацию,  возвращаем некорректность файла
                    ifile.seekg(0);
                    return false;
                }
                input_str.erase(0, input_str.find(' ') + 1);
            }
            if (!(airport_name_check(input_str))) { // если файл содержит некорректную информацию,  возвращаем некорректность файла
                ifile.seekg(0);
                return false;
            }
        }
    }
    ifile.clear();
    ifile.seekg(0);
    return true;
}

bool correct_file(std::fstream& ifile) {
    std::string input_str;
    set<string> fl_numbers; // множество использованных номеров полетов, для невозможности повторения
    while (!ifile.eof()) {
        std::getline(ifile, input_str);
        if (input_str == "") break;

        string fl_number = input_str.substr(0, input_str.find(' ')); // парсинг строки вида "FL_NUM AN1 AN2"
        if (!flight_num_check(fl_number) or fl_numbers.find(fl_number) == fl_numbers.end()) { // если файл содержит некорректную информацию,  возвращаем некорректность файла
            ifile.seekg(0);
            return false;
        }
        fl_numbers.insert(fl_number);

        if (input_str.find(' ') != -1) {
            input_str.erase(0, input_str.find(' ') + 1);
            while (input_str.find(' ') != -1) {
                std::string test = input_str.substr(0, input_str.find(' '));
                if (!airport_name_check(test)) { // если файл содержит некорректную информацию,  возвращаем некорректность файла
                    ifile.seekg(0);
                    return false;
                }
                input_str.erase(0, input_str.find(' ') + 1);
            }
            if (!(airport_name_check(input_str))) { // если файл содержит некорректную информацию,  возвращаем некорректность файла
                ifile.seekg(0);
                return false;
            }
        }
    }
    ifile.clear();
    ifile.seekg(0);
    return true;
}

//-------------------------- функцианальные программные единицы --------------------------------

int get_flight_number() {
    std::string input;
    std::cout << "Enter flight number, or 0 to exit ";
    std::getline(std::cin, input);

    const int incorrect_fl_num = -1;
    const int menu_back = -2;

    delete_extraspace_string(input);
    if (input == "") return incorrect_fl_num;
    else if (input == "0") return menu_back; // код выхода из позиции меню
    else if (flight_num_check(input)) {
        return std::stoi(input);
    }
    else {
        incorrect_data_message(true); // вывод информационного сообщения
        return incorrect_fl_num; // выводим код некорректного ввода числа
    }

}

std::string get_airport_name() {
    std::string input;
    std::cout << "Enter airport name, or 0 to exit ";

    std::getline(std::cin, input);
    delete_extraspace_string(input);

    if (input == "0") return "-1";
    if (!airport_name_check(input)) {
        incorrect_data_message(false);
        return "0";
    }

    return input;
}

void set_color(std::string text, int color) { // выбор цвета (в значение color подставлять из перечисления serv_colors)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    std::cout << text;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void delete_extraspace_string(std::string& str) {
    bool only_space = true;
    for (auto letter : str) {
        if (letter != ' ') only_space = false;
    }
    if (only_space) {
        str="";
        return;
    }

    int index = 0;
    for (int i = str.length() - 1; i >= 0; i--) { // удаление пробелов в конце строки
        if (str[str.length() - 1] != ' ') break; // если последний символ не пробел, то выходим из цикла
        if (str[i] != ' ') {   // встречаем пробел и удаляем все после него
            index = i + 1;        // после чего выходим из цикла
            str.erase(index, str.length() - index);
            break;
        }
    }

    index = 0;
    if (str[0] != ' ') return; // если первый символ не пробел, то выходим функции
    for (int i = 0; i < str.length(); i++) {
        if (str[i] != ' ') {    // встречаем пробел и удаляем все до него
            index = i;           // после чего выходим из функции
            str.erase(0, index);
            break;
        }
    }
}
