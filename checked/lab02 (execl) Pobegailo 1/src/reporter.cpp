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

void output_binary_file(const string &binary_file_name, int hourly_payment, ostream &os);

int count_salary(int a, int b);

void process_reporter(const string &report_file_name, const string &binary_file_name, int hourly_payment);


int main (int argc, char *argv[]){
    std::cout<<"Hello world: process reporter"<<std::endl;
    int temp = atoi(argv[2]);
    process_reporter(argv[0], argv[1], temp);
};


void process_reporter(const string &report_file_name, const string &binary_file_name, int hourly_payment) {
    std::ofstream fout(report_file_name);
    fout << "Title: File report \"" << report_file_name << "\"\n";
    output_binary_file(binary_file_name, hourly_payment, fout);
    fout.close();
}

void output_binary_file(const string &binary_file_name, int hourly_payment, ostream &os) {
    employee myEmployee{};
    std::ifstream binary_fin(binary_file_name, std::ios::binary);
    while (binary_fin.read((char *) &myEmployee, sizeof(employee))) {
        os << "Line: " << myEmployee.num << "\tname: " << myEmployee.name << "\tworked hour: " << myEmployee.hours;
        if (hourly_payment >= 0) {
            os << "\tpayment: " << count_salary(myEmployee.hours, hourly_payment);
        }
        os << '\n';
    }
    binary_fin.close();
}

int count_salary(int a, int b) {
    return a * b;
}



