#ifndef DUAL_LIST_LAB_DIVARICATE_LIST_H
#define DUAL_LIST_LAB_DIVARICATE_LIST_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

enum status_signals {
    last_el, middle_el, first_el, first_and_last, empty_struct, no_element
};

struct divaricate_list;
struct _base_divaricate_list;
struct _child_divaricate_list;

struct divaricate_list {
    // методы
    _base_divaricate_list& operator()(int fl_num);
    _base_divaricate_list& find_last();
    _base_divaricate_list& operator[](int index);
    int len();

    void struct_check(const std::string& check_an = "");

    void inputFF(std::ifstream& ifile);
    void inputFF(std::fstream& ifile);

    void outputTF(std::ofstream& ofile);
    void outputTF(std::fstream& ofile);

    void add(int fl_num);
    void add_branch(int fl_num, const std::string& name1, const std::string& name2);

    void clear();
    void del_last();
    void del_element(int fl_num);

    void swap(_base_divaricate_list* el1, int status);
    void sort(bool ltor);

    bool contain(int fl_num);
    bool is_empty();

    ~divaricate_list();
private:
    // поля
    int len_depth = 0;
    _base_divaricate_list* first_el_ptr = nullptr;

    // методы
    void swap(_base_divaricate_list& el1, _base_divaricate_list& el2);
    _base_divaricate_list* find_ptr_before(int fl_num, int& status);
};

struct _base_divaricate_list {
    friend divaricate_list; // дружественная структура по отношению к divaricate_list

    // методы
    _base_divaricate_list& operator=(int fl_num);
    _child_divaricate_list& operator()(const std::string& airport_name);
    _child_divaricate_list& find_last();

    _child_divaricate_list& operator[](int index);

    int len();
    int get_data();

    void add_child(const std::string& airport_name);
    void add_childs(const std::vector<std::string>& airports_arr);

    void del_child(const std::string& airport_name);
    void del_last();
    void del_childs();

    bool contain(const std::string& airport_name);

private:
    // поля
    int len_widght = 0;
    int flight_number = -1;
    _base_divaricate_list* next_el_ptr = nullptr;
    _child_divaricate_list* child_el_ptr = nullptr;

    // методы
    void get_branch();
    _child_divaricate_list* find_ptr_before(const std::string& airport_name, int& status);
};

struct _child_divaricate_list {
    // методы
    std::string get_data();
    _child_divaricate_list& operator=(const std::string& airport_name);
    friend _base_divaricate_list; // дружественная структура по отношению к _base_divaricate_list
    friend divaricate_list; // дружественная структура по отношению к divaricate_list
private:
    // поля
    std::string airport_name = "";
    _child_divaricate_list* brother_el_ptr = nullptr;
};

//---------------------------------------------------------------------------

int get_flight_number();
void delete_extraspace_string(std::string& str);

#endif //DUAL_LIST_LAB_DIVARICATE_LIST_H
