#include "Employee.h";
#include "Reporter.h";
using namespace std;
int main(int argc, char* argv[]) {

	std::string binaryFile = argv[1];
	std::string reportFile = argv[2];
	int temp = atoi(argv[3]);
	Reporter::createReport(binaryFile, reportFile,temp);
	}