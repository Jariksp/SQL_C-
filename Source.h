
#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>
#include "sqlite3/sqlite3.h"
#include <sstream>

std::string generate_random_string();
void generate_csv_file(const std::string& filename);
bool is_vowel(char c);
void replace_odd_digits(std::string& str);
void filter_csv_file(const std::string& input_filename, const std::string& output_filename);
void create_table(sqlite3* db);
void insert_data_into_table(sqlite3* db, const std::string& input_filename);
void delete_data_from_table(sqlite3* db);

#endif