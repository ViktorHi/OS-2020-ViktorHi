#include "Creator.h"

void Creator::createBinaryFile(std::string& binaryFileName, int recordNumber)
{
	std::ofstream fout(binaryFileName, std::ios::binary);
	for (int i = 0; i < recordNumber; i++) {
		employee myEmployee;
		std::cout << "Enter employee ID ";
		std::cin >> myEmployee.num;
		std::cout << "Enter employee name ";
		std::cin >> myEmployee.name;
		std::cout << "Enter employee worked hours ";
		std::cin >> myEmployee.hours;
		fout.write((char*)& myEmployee, sizeof(employee));
	}
}
