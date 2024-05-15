#ifndef DUAL_LIST_LAB_INTERFACE_DIVLIST_H
#define DUAL_LIST_LAB_INTERFACE_DIVLIST_H

#include <filesystem>
#include <conio.h>
#include <set>
#include <Windows.h>
#include "divaricate_list.h"

enum {
    zero = 48, one, two, three, four, five, six
};

enum colors
{
    green = 10, purple = 13, blue = 11, red = 12, yellow = 14, white = 15
};

//-------------------- программные единицы  взаимодействия с пользователем --------------------

void menu();
void struct_enter(divaricate_list& exemple);
void struct_delete(divaricate_list& exemple);
void struct_enter_ff(divaricate_list& exemple);
void struct_load_tf(divaricate_list& exemple);
void struct_sort(divaricate_list& exemple);
void struct_grouped(divaricate_list& exemple); // grouped

//-------------------------- информационные программные единицы --------------------------------

void exit_message(bool& exit_flag);
void no_element_message();
void sort_message();
void incorrect_data_message(bool type);
void el_repeat_message();
void incorrect_file_message();

//-------------------------- логические программные единицы --------------------------------

bool flight_num_check(const std::string& fl_num);
bool airport_name_check(std::string& airport_name);
bool check_file_exist(std::string& file_name);
bool check_file_name(const std::string& file_name);
bool correct_file(std::ifstream& ifile);
bool correct_file(std::fstream& ifile);

//-------------------------- функцианальные программные единицы --------------------------------

void set_color(std::string text, int color);
int get_flight_number();
std::string get_airport_name();
void delete_extraspace_string(std::string& str);

#endif //DUAL_LIST_LAB_INTERFACE_DIVLIST_H
