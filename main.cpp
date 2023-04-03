
#include <iostream>
#include "sqlite3/sqlite3.h"
#include"Source.h"


int main() {


    // ��������� ���� � ����������� ��������
    std::string filename = "random_data.csv";
    generate_csv_file(filename);

    // Գ������� �� �������� ��� � �����
    std::string input_filename = "random_data.csv";
    std::string output_filename = "filtered_data.csv";
    filter_csv_file(input_filename, output_filename);

    // ������������ �'������� � ����� ����� �� ��������� �������
    sqlite3* db;
    int result = sqlite3_open("test.db", &db);
    if (result != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }
    create_table(db);

    // ������ ��� �� �������
    insert_data_into_table(db, output_filename);

    // ��������� �'������� � ����� �����
    sqlite3_close(db);

    return 0;
}

