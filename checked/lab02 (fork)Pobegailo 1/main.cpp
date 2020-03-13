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

void create_fork(void (*f)(string const &, string const &, int), const string& first_par, const string& second_par, int third_par);

void output_binary_file(const string &binary_file_name, int hourly_payment, ostream &os);

void process_reporter(const string &report_file_name, const string &binary_file_name, int hourly_payment);

void fill_employee(employee &temp);

void process_creator(const string &binary_file_name, const string &report_file_name, int record_amounts);


int main() {

    string binary_file_name, report_file_name;
    int record_amounts, payment = -1;

    cout << "Enter binary file name " << endl;
    getline(cin, binary_file_name);
    cout << "Enter numbers of records " << endl;
    cin >> record_amounts;
    cin.get();

    create_fork(process_creator,
                binary_file_name,
                report_file_name,
                record_amounts);

    output_binary_file(binary_file_name, payment, cout);

    cout << "Enter report file name " << endl;
    getline(cin, report_file_name);
    cout << "Enter payment per hour records " << endl;
    cin >> payment;

    create_fork(process_reporter,
                report_file_name,
                binary_file_name,
                payment);

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


void create_fork(void f(string const&, string const&, int), const string& first_par, const string& second_par, int third_par) {
    pid_t pid = fork();
    if (pid < 0) {
        cout << "Problem with creating new process\nApplication will be closed";
        exit(1);
    }
    if (pid == 0) {
        f(first_par, second_par, third_par);
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

void process_reporter(const string &report_file_name, const string &binary_file_name, int hourly_payment) {
    std::ofstream fout(report_file_name);
    fout << "Title: File report \"" << report_file_name << "\"\n";
    output_binary_file(binary_file_name, hourly_payment, fout);
    fout.close();
}

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
