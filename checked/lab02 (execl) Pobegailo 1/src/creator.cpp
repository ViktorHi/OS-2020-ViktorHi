//
// Created by viktor on 6.03.20.
//
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <unistd.h>
#include <wait.h>

using namespace std;

struct employee {

    int num;
    char name[10];
    double hours;
};

void fill_employee(employee &temp);

void process_creator(const string &binary_file_name, const string &report_file_name, int record_amounts);


int main(int argc, char *argv[]) {
    std::cout << "Hello world: process creator" << std::endl;
    int temp = atoi(argv[2]);
    process_creator(argv[0], argv[1], temp);

};

void process_creator(const string &binary_file_name, const string &report_file_name, int record_amounts) {
    std::ofstream fout(binary_file_name, std::ios::binary);

    for (int i = 0; i < record_amounts; i++) {
        employee my_employee{};
        fill_employee(my_employee);
        fout.write((char *) &my_employee, sizeof(employee));
    }
}

void fill_employee(employee &temp) {
    std::cout << "Enter employee ID ";
    std::cin >> temp.num;
    std::cout << "Enter employee name ";
    std::cin >> temp.name;
    std::cout << "Enter employee worked hours ";
    std::cin >> temp.hours;
}

