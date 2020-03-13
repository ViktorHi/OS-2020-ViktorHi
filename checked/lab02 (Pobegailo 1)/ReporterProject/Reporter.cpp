#include "Reporter.h"
#include "Employee.h"
void Reporter::createReport(std::string& binaryFileName, std::string& reportFileName , int hourlyPayment)
{
	std::ofstream fout(reportFileName);
	std::ifstream fin(binaryFileName, std::ios::binary);
	employee myEmployee;
	fout << "Title: File report \""<< reportFileName<<"\"\n";

	while (fin.read((char*)&myEmployee,sizeof(employee))) {
		fout << "Line: " << myEmployee.num << "\tname: " << myEmployee.name << "\thours: " 
			<< myEmployee.hours << "\tpayment: " << myEmployee.hours * hourlyPayment <<"\n";
	}

	fin.close();
	fout.close();
}
