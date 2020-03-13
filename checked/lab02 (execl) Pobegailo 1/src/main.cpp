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

int count_salary(int a, int b);

void output_report_file(const string &report_file_name);

void wait_child_process();

void create_fork(const string& first_par, const string& second_par, int third_par, bool is_creator);

void output_binary_file(const string &binary_file_name, int hourly_payment, ostream &os);

int main() {

    string binary_file_name, report_file_name;
    int record_amounts, payment = -1;

    cout << "Enter binary file name " << endl;
    getline(cin, binary_file_name);
    cout << "Enter numbers of records " << endl;
    cin >> record_amounts;
    cin.get();

    create_fork(binary_file_name,
                report_file_name,
                record_amounts,
                true);

    output_binary_file(binary_file_name, payment, cout);

    cout << "Enter report file name " << endl;
    getline(cin, report_file_name);
    cout << "Enter payment per hour records " << endl;
    cin >> payment;

    create_fork(report_file_name,
                binary_file_name,
                payment,
                false);

    output_report_file(report_file_name);
    return 0;
}


void wait_child_process() {
    int status = 0;
    if (waitpid(-1, &status, 0) == -1) {
        cout << "in waiting for process creator...\n";
    } else {
        if (WIFSIGNALED(status)) {
            cout << "child was terminated with code " << WIFSIGNALED(status) << '\n';
        } else if (WIFEXITED(status)) {
            cout << "child exited normally with code " << WIFEXITED(status) << '\n';
        }
    }
}


void output_report_file(const string &report_file_name) {
    std::ifstream fin(report_file_name);
    while (fin.peek() != EOF) {
        std::string temp;
        getline(fin, temp);
        cout << temp << endl;
    }
}


void create_fork(const string& first_par, const string& second_par, int third_par,bool is_creator) {
    pid_t pid = fork();
    if (pid < 0) {
        cout << "Problem with creating new process\nApplication will be closed";
        exit(1);
    }
    if (pid == 0) {
        string temp=first_par+' '+second_par+' '+to_string(third_par);
        if(is_creator){
            if (execl("/home/viktor/os/lab01/creator.out",first_par.c_str() ,second_par.c_str(), to_string(third_par).c_str() , NULL) == -1) {
                fprintf(stderr, "Unable to exec\n");
            }
        }else{
            if (execl("/home/viktor/os/lab01/reporter.out",first_par.c_str() ,second_par.c_str(), to_string(third_par).c_str(), NULL) == -1) {
                fprintf(stderr, "Unable to exec\n");
            }
        }

        exit(0);
    } else {
        wait_child_process();
    }
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
