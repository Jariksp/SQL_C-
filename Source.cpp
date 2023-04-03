#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>
#include "sqlite3/sqlite3.h"
#include <sstream>
#include"Source.h"


// Генерує рядок випадкових (псевдовипадкових) символів
std::string generate_random_string() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 35);
    const std::string symbols = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string result;
    for (int i = 0; i < 8; ++i) {
        result += symbols[dis(gen)];
    }
    return result;
}

// Генерує csv файл з 1024 записів по 6 стовпців
void generate_csv_file(const std::string& filename) {
    std::ofstream file(filename);
    for (int i = 0; i < 1024; ++i) {
        file << generate_random_string() << "\t"
            << generate_random_string() << "\t"
            << generate_random_string() << "\t"
            << generate_random_string() << "\t"
            << generate_random_string() << "\t"
            << generate_random_string() << "\n";
    }
    file.close();
}

// Перевіряє чи символ є голосною літерою
bool is_vowel(char c) {
    static const std::string vowels = "AEIOUY";
    return std::find(vowels.begin(), vowels.end(), c) != vowels.end();
}

// Замінює непарні цифри символом #
void replace_odd_digits(std::string& str) {
    for (char& c : str) {
        if (isdigit(c) && (c - '0') % 2 != 0) {
            c = '#';
        }
    }
}

// Зчитує csv файл та відфільтровує записи
void filter_csv_file(const std::string& input_filename, const std::string& output_filename) {
    std::ifstream input_file(input_filename);
    std::ofstream output_file(output_filename);
    std::string line;
    int count = 1;

    while (std::getline(input_file, line)) {
        //std::cout << line << std::endl;
        if (is_vowel(line[0]))
            ++count;

        if (count % 7 == 0) {
            count = 1;
            continue;
        }
        replace_odd_digits(line);
        output_file << line << "\n";
        //std::cout << line << std::endl;
    }
    input_file.close();
    output_file.close();
}




void create_table(sqlite3* db) {
    const char* sql = "CREATE TABLE mytable ("
        "id INTEGER PRIMARY KEY,"
        "col1 TEXT NOT NULL,"
        "col2 TEXT NOT NULL,"
        "col3 TEXT NOT NULL,"
        "col4 TEXT NOT NULL,"
        "col5 TEXT NOT NULL,"
        "col6 TEXT NOT NULL);";

    char* error_message = nullptr;
    int result = sqlite3_exec(db, sql, nullptr, nullptr, &error_message);
    if (result != SQLITE_OK) {
        std::cerr << "Error creating table: " << error_message << std::endl;
        sqlite3_free(error_message);
        exit(EXIT_FAILURE);
    }
}



void insert_data_into_table(sqlite3* db, const std::string& input_filename) {
    char* error_message = nullptr;
    const char* sql = "INSERT INTO mytable (col1, col2, col3, col4, col5, col6) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* statement = nullptr;
    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input_file(input_filename);
    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        std::string col1, col2, col3, col4, col5, col6;
        iss >> col1 >> col2 >> col3 >> col4 >> col5 >> col6;
        sqlite3_bind_text(statement, 1, col1.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 2, col2.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 3, col3.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 4, col4.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 5, col5.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 6, col6.c_str(), -1, SQLITE_TRANSIENT);

        result = sqlite3_step(statement);
        if (result != SQLITE_DONE) {
            std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
            exit(EXIT_FAILURE);
        }
        sqlite3_reset(statement);
    }
    input_file.close();
    sqlite3_finalize(statement);
}

void delete_data_from_table(sqlite3* db) {
    const char* sql = "DELETE FROM mytable WHERE col2 LIKE '[0-9]%';";

    char* error_message = nullptr;
    int result = sqlite3_exec(db, sql, nullptr, nullptr, &error_message);
    if (result != SQLITE_OK) {
        std::cerr << "Error deleting data: " << error_message << std::endl;
        sqlite3_free(error_message);
        exit(EXIT_FAILURE);
    }
}
