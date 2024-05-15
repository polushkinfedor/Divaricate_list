#include "divaricate_list.h"


// методы структуры divaricate_list
_base_divaricate_list& divaricate_list::operator()(int fl_num) {
    _base_divaricate_list* find_el_ptr = first_el_ptr; // указатель на корневую структуры
    while (find_el_ptr != nullptr) { // пока не последний элемент
        if (find_el_ptr->flight_number == fl_num) break; // если элемент содержит искомый fl_num, то возвращаем указатель на экземпляр содержащей его структуры
        else find_el_ptr = find_el_ptr->next_el_ptr;
    }
    return (*find_el_ptr);
}

_base_divaricate_list& divaricate_list::find_last() {
    _base_divaricate_list* find_el_ptr = first_el_ptr; // указатель на корневую структуры
    if (find_el_ptr == nullptr) return *find_el_ptr; // структура изначально пуста
    while (find_el_ptr->next_el_ptr != nullptr) { // находим последний доступный корневой элемент
        find_el_ptr = find_el_ptr->next_el_ptr;
    }
    return *find_el_ptr;
}

_base_divaricate_list* divaricate_list::find_ptr_before(int fl_num, int& status) {
    _base_divaricate_list* ptr_on_element = first_el_ptr; // ищем указатель на элемент, перед удаляемым
    if (ptr_on_element == nullptr) { // если первый элемент не существует, то структура пуста
        status = status_signals::empty_struct;
        return ptr_on_element; // возвращаем пустой указатель и сигнал с интерпретацией результата
    }
    else if (ptr_on_element->flight_number == fl_num) { // если первый же элемент содержит искомое значение, то перед ним не существует элементов
        if (ptr_on_element->next_el_ptr == nullptr) status = status_signals::first_and_last; // определим, является ли элемент так же и последним
        else status = status_signals::first_el;
        return  ptr_on_element; // вернем указатель на первый элемент
    }

    while (ptr_on_element->next_el_ptr != nullptr) { // пока следующий элемент существует
        if (ptr_on_element->next_el_ptr->flight_number == fl_num) { // если следующий элемент содержит искомую информацию
            if (ptr_on_element->next_el_ptr->next_el_ptr == nullptr) status = status_signals::last_el;
            else status = status_signals::middle_el; // получим интерпретацию работы метода
            return ptr_on_element; // вернем указатель на элемент перед элементом для удаления
        }
        ptr_on_element = ptr_on_element->next_el_ptr; // итерация по циклу
    }
    status = status_signals::no_element; // получим обратную связь об отсутствии элемента с искомой датой в структуре
    return nullptr;

}

_base_divaricate_list& divaricate_list::operator[](int index) {
    // возвращает значение по индексу
    int counter = 0; // счетчик элемента
    _base_divaricate_list* find_el_ptr = first_el_ptr;
    while (counter != index) { // пока индекс не равен счетчику
        counter++;
        find_el_ptr = find_el_ptr->next_el_ptr;
    }
    return *find_el_ptr; // возвращаем экземпляр по индексу
}

int divaricate_list::len() {
    return len_depth;
}

void divaricate_list::add(int fl_num) {
    if (len_depth == 0) { // если структура пуста, создаем первый корневой элемент
        first_el_ptr = new _base_divaricate_list;
        first_el_ptr->flight_number = fl_num;
    }
    else { // иначе находим последний элемент и добавляем для него следующий корневой элемент
        _base_divaricate_list& last_el = find_last();
        last_el.next_el_ptr = new _base_divaricate_list;
        last_el.next_el_ptr->flight_number = fl_num;
    }
    len_depth += 1; // при добавлении увеличиваем глубину структуры
}

void divaricate_list::add_branch(int fl_num, const std::string& name1, const std::string& name2) {
    add(fl_num);
    find_last().add_childs({ name1, name2 });
}

void divaricate_list::clear() {
    _base_divaricate_list* tmp_ptr_prev = nullptr; // инициализируем вспомогательные указатели
    if (first_el_ptr == nullptr) return;

    _base_divaricate_list* tmp_ptr_next = first_el_ptr->next_el_ptr;

    delete first_el_ptr; // удаляем указатель с базового узла на ветку
    if (tmp_ptr_next != nullptr) {
        while (tmp_ptr_next->next_el_ptr != nullptr) {
            tmp_ptr_prev = tmp_ptr_next;
            tmp_ptr_next = tmp_ptr_prev->next_el_ptr;
            tmp_ptr_prev->del_childs(); // для каждого элемента ствола удаляем все элементы веток
            delete tmp_ptr_prev; // удаляем все элементы ствола
        }
        tmp_ptr_next->del_childs();
        delete tmp_ptr_next; // удаляем последний элемент
    }
    first_el_ptr = nullptr;
    len_depth = 0; // устанавливаем глубину дерева на ноль
}

void divaricate_list::del_last() {
    if (len_depth == 0) return; // если структура уже пуста, то нельзя удалить последний элемент
    else if (len_depth == 1) { // если структура состоит из одного элемента, очищаем структуру
        delete first_el_ptr;
        first_el_ptr = nullptr;
        len_depth--; //
        return;
    }
    _base_divaricate_list* temp_ptr = first_el_ptr;
    int counter = 1;

    while (counter != len_depth - 1) { // находим последний элемент
        temp_ptr = temp_ptr->next_el_ptr;
        counter += 1;
    }
    delete temp_ptr->next_el_ptr; // удаляем последний элемент
    temp_ptr->next_el_ptr = nullptr; // для предыдущего элемента устанавливаем указатель на след. структуру nullptr
    len_depth--; // уменьшаем глубину структуры
}

void divaricate_list::del_element(int fl_num) {
    int status; // статус найденного элемента
    _base_divaricate_list* ptr_before = find_ptr_before(fl_num, status); // находим предыдущий элемент для удаляемого
    _base_divaricate_list* tmp_ptr = nullptr; // промежуточный указатель

    switch (status) {
    case status_signals::first_el: // если удаляемый элемент первый, то ptr_before указывает на него
        first_el_ptr = first_el_ptr->next_el_ptr; // переставляем указатель на начало структуры на следующий элемент
        delete ptr_before; // очищаем удаляемый элемент
        break;

    case status_signals::middle_el: // если элемент в середине структуры
        tmp_ptr = ptr_before->next_el_ptr; // сохраняем указатель на удаляемый элемент
        ptr_before->next_el_ptr = tmp_ptr->next_el_ptr; // заменяем указатель для предыдущего на структуру после удаляемой
        delete tmp_ptr;
        break;

    case status_signals::last_el: // если элемент в стуртуре последний
        delete ptr_before->next_el_ptr; // удаляем элемент
        ptr_before->next_el_ptr = nullptr; // выставляем указатель для предыдущего в значение nullptr
        break;

    case status_signals::first_and_last: // если элемент в стуртуре первый и последний
        delete ptr_before;
        first_el_ptr = nullptr;
        break;

    case status_signals::empty_struct: // если структура пуста
    case status_signals::no_element:   // или элемента для удаления не найдено
        len_depth++; // компенсируем уменьшение глубины от последующего вычетания
        break;
    }

    len_depth--; // уменьшаем глубину структуры на единицу
}

void divaricate_list::struct_check(const std::string& check_an) {
    // вывод структуры в консоль
    if (this->is_empty() == true) {
        std::cout << "Struct is empty.\n";
        return;
    }
    _base_divaricate_list* el = first_el_ptr;
    int counter = 0;
    while (el != nullptr) {
        if (check_an == "") {
            std::cout << " " << el->flight_number;
            el->get_branch();
            el = el->next_el_ptr;
            if (el != nullptr) std::cout << " |\n |\n V\n";
        }
        else {
            if (el->contain(check_an)) {
                counter++;
                std::cout << el->flight_number;
                el->get_branch();
                std::cout << "\n";
            }
            el = el->next_el_ptr;
        }
    }

    if (check_an != "" and counter == 0) {
        std::cout << "Struct doesn't contain " << check_an << "\n";
        return;
    }
}

void divaricate_list::inputFF(std::ifstream& ifile) {
    // заполнение из файла
    std::string input_str;
    while (!ifile.eof()) {
        std::getline(ifile, input_str);
        if (input_str == "") break;

        int fl_number = std::stoi(input_str.substr(0, input_str.find(' '))); // парсинг строки вида "FL_NUM AN1 AN2"
        this->add(fl_number);

        if (input_str.find(' ') != -1) {
            input_str.erase(0, input_str.find(' ') + 1);
            while (input_str.find(' ') != -1) {
                this->operator()(fl_number).add_child(input_str.substr(0, input_str.find(' ')));
                input_str.erase(0, input_str.find(' ') + 1);
            }
            this->operator()(fl_number).add_child(input_str);
        }
    }
    ifile.clear();
    ifile.seekg(0);
}

void divaricate_list::inputFF(std::fstream& ifile) {
    // перегрузка заполнения из файла для экземпляра класса fstream
    std::string input_str;
    while (!ifile.eof()) {
        std::getline(ifile, input_str);
        if (input_str == "") break;

        int fl_number = std::stoi(input_str.substr(0, input_str.find(' '))); // парсинг строки вида "FL_NUM AN1 AN2"
        this->add(fl_number);

        if (input_str.find(' ') != -1) {
            input_str.erase(0, input_str.find(' ') + 1);
            while (input_str.find(' ') != -1) {
                this->operator()(fl_number).add_child(input_str.substr(0, input_str.find(' ')));
                input_str.erase(0, input_str.find(' ') + 1);
            }
            this->operator()(fl_number).add_child(input_str);
        }
    }
    ifile.clear();
    ifile.seekg(0);
}

void divaricate_list::outputTF(std::ofstream& ofile) {
    // вывод информации в файл
    _base_divaricate_list* ptr_on_curr_el = first_el_ptr;
    if (first_el_ptr == nullptr) return;
    for (int i = 0; i < len_depth; i++) { // проход по всей структуре

        ofile << ptr_on_curr_el->flight_number;
        _child_divaricate_list* ptr_on_branch = ptr_on_curr_el->child_el_ptr;
        if (ptr_on_branch == nullptr) ofile << "\n";
        else {
            while (ptr_on_branch != nullptr) { // проход по ветке для корневого узла
                ofile << " ";
                ofile << ptr_on_branch->airport_name;
                ptr_on_branch = ptr_on_branch->brother_el_ptr;
            }
            ofile << "\n";
        }
        ptr_on_curr_el = ptr_on_curr_el->next_el_ptr;
    }
    ofile.clear();
}

void divaricate_list::outputTF(std::fstream& ofile) {
    // перегрузка вывода информации в файл для экземпляра класса fstream
    _base_divaricate_list* ptr_on_curr_el = first_el_ptr;
    if (first_el_ptr == nullptr) return;
    for (int i = 0; i < len_depth; i++) { // проход по всей структуре

        ofile << ptr_on_curr_el->flight_number;
        _child_divaricate_list* ptr_on_branch = ptr_on_curr_el->child_el_ptr;
        if (ptr_on_branch == nullptr) ofile << "\n";
        else {
            while (ptr_on_branch != nullptr) { // проход по ветке для корневого узла
                ofile << " ";
                ofile << ptr_on_branch->airport_name;
                ptr_on_branch = ptr_on_branch->brother_el_ptr;
            }
            ofile << "\n";
        }
        ptr_on_curr_el = ptr_on_curr_el->next_el_ptr;
    }
    ofile.clear();
    ofile.seekg(0);
}

void divaricate_list::swap(_base_divaricate_list* el, int status) {
    // меняет два элемента после указанного
    _base_divaricate_list* tmp_ptr1 = nullptr, * tmp_ptr2 = nullptr;
    if (status == first_el) {
        first_el_ptr = el->next_el_ptr;
        el->next_el_ptr = first_el_ptr->next_el_ptr;
        first_el_ptr->next_el_ptr = el;
    } else if (status == middle_el) {
        tmp_ptr1 = el->next_el_ptr;
        tmp_ptr2 = el->next_el_ptr->next_el_ptr;
        tmp_ptr1->next_el_ptr = tmp_ptr2->next_el_ptr;
        tmp_ptr2->next_el_ptr = tmp_ptr1;
        el->next_el_ptr = tmp_ptr2;
    }
}

void divaricate_list::sort(bool ltor) {
    system("cls");
    for (int i = 1; i < len(); i++) {
        bool was_change = 0;
        _base_divaricate_list* check_el = first_el_ptr;
        auto comparator = [ltor](int a, int b) { if(ltor) return a>b; else return a<b; };

        for (int j = 0; j < len() - i; j++) {
            if (j == 0 and comparator(check_el->get_data(), check_el->next_el_ptr->get_data())) {
                was_change = true;
                swap(check_el, first_el);
                check_el = first_el_ptr;
            }
            else if (comparator(check_el->next_el_ptr->get_data(), check_el->next_el_ptr->next_el_ptr->get_data())) {
                    swap(check_el, middle_el);
                    was_change = true;
            }
            if (j != 0) check_el = check_el->next_el_ptr;
        }
        if (was_change == false) break;
    }
}

bool divaricate_list::contain(int fl_num) {
    // существует ли искомый элемент в структуре
    _base_divaricate_list* find_el_ptr = first_el_ptr;
    while (find_el_ptr != nullptr) { // проход по всем элементам
        if (find_el_ptr->flight_number == fl_num) return true;
        else find_el_ptr = find_el_ptr->next_el_ptr;
    }
    return false;
}

bool divaricate_list::is_empty() {
    // проверка на пустоту
    if (len_depth == 0) return true;
    else return false;
}

divaricate_list::~divaricate_list() {
    // деструктор для класса
    this->clear();
}


// методы структуры _base_divaricate_list
_base_divaricate_list& _base_divaricate_list::operator=(int fl_num) {
    this->flight_number = fl_num;
    return *this;
}

_child_divaricate_list& _base_divaricate_list::operator()(const std::string& airport_name) {
    _child_divaricate_list* find_el_ptr = child_el_ptr; // указатель на структуры ветки
    while (find_el_ptr != nullptr) { // пока не последний элемент
        if (find_el_ptr->airport_name == airport_name) break; // если элемент содержит искомый airport_name, то возвращаем указатель на экземпляр содержащей его структуры
        else find_el_ptr = find_el_ptr->brother_el_ptr;
    }
    return (*find_el_ptr);
}

_child_divaricate_list& _base_divaricate_list::find_last() {
    _child_divaricate_list* find_el_ptr = child_el_ptr; // указатель на структуры ветки
    if (find_el_ptr == nullptr) return *find_el_ptr; // структура изначально пуста
    while (find_el_ptr->brother_el_ptr != nullptr) { // пока не последний элемент
        find_el_ptr = find_el_ptr->brother_el_ptr;
    }
    return *find_el_ptr; // возвращаем последний элемент ветки
}

_child_divaricate_list* _base_divaricate_list::find_ptr_before(const std::string& airport_name, int& status) {
    _child_divaricate_list* ptr_on_element = child_el_ptr; // ищем указатель на элемент, перед удаляемым
    if (ptr_on_element == nullptr) { // если первый элемент не существует, то структура пуста
        status = status_signals::empty_struct;
        return ptr_on_element; // возвращаем пустой указатель и сигнал с интерпретацией результата
    }
    else if (ptr_on_element->airport_name == airport_name) { // если первый же элемент содержит искомое значение, то перед ним не существует элементов
        if (ptr_on_element->brother_el_ptr == nullptr) status = status_signals::first_and_last; // определим, является ли элемент так же и последним
        else status = status_signals::first_el;
        return  ptr_on_element; // вернем указатель на первый элемент
    }

    while (ptr_on_element->brother_el_ptr != nullptr) { // пока следующий элемент существует
        if (ptr_on_element->brother_el_ptr->airport_name == airport_name) { // если следующий элемент содержит искомую информацию
            if (ptr_on_element->brother_el_ptr->brother_el_ptr == nullptr) status = status_signals::last_el;
            else status = status_signals::middle_el; // получим интерпретацию работы метода
            return ptr_on_element; // вернем указатель на элемент перед элементом для удаления
        }
        ptr_on_element = ptr_on_element->brother_el_ptr; // итерация по циклу
    }
    status = status_signals::no_element; // получим обратную связь об отсутствии элемента с искомой датой в структуре
    return nullptr;

}

_child_divaricate_list& _base_divaricate_list::operator[](int index) {
    // нахождение значения по индексу
    int counter = 0;
    _child_divaricate_list* find_el_ptr = child_el_ptr;
    while (counter != index) { // пока не искомый индекс, итерируемся по списку
        find_el_ptr = find_el_ptr->brother_el_ptr;
        counter++;
    }
    return *find_el_ptr; // возвращаем значение по индексу
}

int _base_divaricate_list::len() {
    return len_widght;
}

int _base_divaricate_list::get_data() {
    return flight_number;
}

void _base_divaricate_list::get_branch() {
    // вывод всех элементов строки в консоль
    _child_divaricate_list* ptr_on_branch = child_el_ptr;
    while (ptr_on_branch != nullptr) {
        std::cout << "-->" << ptr_on_branch->airport_name;
        ptr_on_branch = ptr_on_branch->brother_el_ptr;
    }
    std::cout << "\n";
}

void _base_divaricate_list::add_child(const std::string& airport_name) {
    if (len_widght == 0) { // если структура пуста, создаем первый элемент ветки
        child_el_ptr = new _child_divaricate_list;
        child_el_ptr->airport_name = airport_name;
    }
    else { // иначе находим последний элемент и добавляем для него следующий элемент ветки
        _child_divaricate_list& last_el = find_last();
        last_el.brother_el_ptr = new _child_divaricate_list;
        last_el.brother_el_ptr->airport_name = airport_name;
    }
    len_widght += 1; // при добавлении увеличиваем глубину структуры
}

void _base_divaricate_list::add_childs(const std::vector<std::string>& airports_arr) {
    for (const auto& airport_name : airports_arr) { // добавление нескольких названий аэропорта
        add_child(airport_name);
    }
}

void _base_divaricate_list::del_childs() {
    _child_divaricate_list* tmp_ptr_prev = nullptr; // инициализируем вспомогательные указатели
    if (child_el_ptr == nullptr) return;
    _child_divaricate_list* tmp_ptr_next = child_el_ptr->brother_el_ptr;

    delete child_el_ptr; // удаляем указатель с базового узла на ветку
    if (tmp_ptr_next != nullptr) {
        while (tmp_ptr_next->brother_el_ptr != nullptr) {
            tmp_ptr_prev = tmp_ptr_next;
            tmp_ptr_next = tmp_ptr_prev->brother_el_ptr;
            delete tmp_ptr_prev; // удаляем все элементы ветки
        }

        delete tmp_ptr_next; // удаляем последний элемент
        len_widght = 0; // устанавливаем глубину ветки на ноль
    }
    child_el_ptr = nullptr;
}

void _base_divaricate_list::del_child(const std::string& airport_name) {
    int status; // статус найденного элемента
    _child_divaricate_list* ptr_before = find_ptr_before(airport_name, status); // находим предыдущий элемент для удаляемого
    _child_divaricate_list* tmp_ptr = nullptr; // промежуточный указатель

    switch (status) {
    case status_signals::first_el: // если удаляемый элемент первый, то ptr_before указывает на него
        child_el_ptr = child_el_ptr->brother_el_ptr; // переставляем указатель на начало структуры на следующий элемент
        delete ptr_before; // очищаем удаляемый элемент
        break;

    case status_signals::middle_el: // если элемент в середине структуры
        tmp_ptr = ptr_before->brother_el_ptr; // сохраняем указатель на удаляемый элемент
        ptr_before->brother_el_ptr = tmp_ptr->brother_el_ptr; // заменяем указатель для предыдущего на структуру после удаляемой
        delete tmp_ptr;
        break;

    case status_signals::last_el: // если элемент в стуртуре последний
        delete ptr_before->brother_el_ptr; // удаляем элемент
        ptr_before->brother_el_ptr = nullptr; // выставляем указатель для предыдущего в значение nullptr
        break;

    case status_signals::first_and_last: // если элемент в стуртуре первый и последний
        delete ptr_before;
        child_el_ptr = nullptr;
        break;

    case status_signals::empty_struct: // если структура пуста
    case status_signals::no_element:   // или элемента для удаления не найдено
        len_widght++; // компенсируем уменьшение глубины от последующего вычитания
        break;
    }

    len_widght--; // уменьшаем глубину структуры на единицу
}

void _base_divaricate_list::del_last() {
    if (len_widght == 0) return; // если структура уже пуста, то нельзя удалить последний элемент
    else if (len_widght == 1) { // если структура состоит из одного элемента, очищаем структуру
        delete child_el_ptr;
        child_el_ptr = nullptr;
        len_widght--;
        return;
    }
    _child_divaricate_list* temp_ptr = child_el_ptr;
    int counter = 1;

    while (counter != len_widght - 1) { // находим последний элемент
        temp_ptr = temp_ptr->brother_el_ptr;
        counter += 1;
    }
    delete temp_ptr->brother_el_ptr; // удаляем последний элемент
    temp_ptr->brother_el_ptr = nullptr; // для предыдущего элемента устанавливаем указатель на след. структуру nullptr
    len_widght--; // уменьшаем глубину структуры
}

bool _base_divaricate_list::contain(const std::string& airport_name) {
    // существует ли искомый элемент в структуре
    _child_divaricate_list* find_el_ptr = child_el_ptr;
    while (find_el_ptr != nullptr) { // проход по всем элементам
        if (find_el_ptr->airport_name == airport_name) return true;
        else find_el_ptr = find_el_ptr->brother_el_ptr;
    }
    return false;
}


// методы структуры _child_divaricate_list
std::string _child_divaricate_list::get_data() {
    return airport_name;
}

_child_divaricate_list& _child_divaricate_list::operator=(const std::string& airport_name) {
    this->airport_name = airport_name;
    return *this;
}

//----------------------------------------------------------------------------------------------------------------------