#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <Windows.h>
#include "Employee.h"
#pragma warning (disable: 4996)
using namespace std;

int main(int argc, char* argv[]) {

	string creatorArgs, binaryFileName;

	cout << "Enter binary file name " << endl;
	getline(cin, binaryFileName);

	cout << "Enter numbers of records " << endl;
	int recordNumber;
	cin >> recordNumber;

	cin.get();
	creatorArgs = "D:\\studies\\4 sem\\OS\\lab02\\MainPoject\\Debug\\CreatorProject.exe " + binaryFileName + " " + to_string(recordNumber);


	LPSTR commandLineCreatorArg = new char[256];
	strcpy(commandLineCreatorArg, creatorArgs.c_str());
	
	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	if (CreateProcess(NULL, commandLineCreatorArg, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &cif, &pi))
	{
		cout << "Process Creator was created successfully..." << endl;
	}
	else {
		cout << "Process Creator wasn't created..." << endl;
		cout << "Close application..." << endl;
		system("pause");
		return(1);
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	employee myEmployee;
	std::ifstream binaryFin(binaryFileName, std::ios::binary);
	while (binaryFin.read((char*)&myEmployee, sizeof(employee))) {
		cout << "Line: " << myEmployee.num << "\tname: " << myEmployee.name << "\thours: "
			<< myEmployee.hours << "\tworked hour: " << myEmployee.hours << "\n";
	}


	string reportFileName;
	int payment;

	cout << "Enter report file name " << endl;
	getline(cin, reportFileName);

	cout << "Enter payment per hour records " << endl;;
	cin >> payment;

	string reportArgs = "D:\\studies\\4 sem\\OS\\lab02\\MainPoject\\Debug\\ReporterProject.exe " + binaryFileName + " " + reportFileName + " " + to_string(payment);
	LPSTR commandLineReporterArg = new char[256];
	strcpy(commandLineReporterArg, reportArgs.c_str());


	if (CreateProcess(NULL, commandLineReporterArg, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &cif, &pi) == TRUE)
	{
		cout << "Process Reporter was created successfully..." << endl;
	}
	else {
		cout << "Process Reporter wasn't created..." << endl;
		cout << "Close application..." << endl;
		system("pause");
		return(1);
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	std::ifstream fin(reportFileName);
	while (fin.peek() != EOF) {
		std::string temp;
		getline(fin, temp);
		cout << temp << endl;
	}

	cout << "End of main program" << endl;
	return 0;
}