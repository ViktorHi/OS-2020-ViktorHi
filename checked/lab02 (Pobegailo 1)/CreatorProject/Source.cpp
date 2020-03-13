#include <iostream>
#include "Creator.h"
using namespace std;
void main(int argc, char* argv[]) {

	//std::cout <<"Binary file name"<< argv[1] <<"\tNumber of records"<< argv[2] << endl;
	std::string binaryFileName = argv[1];
	int temp = atoi(argv[2]);
	Creator::createBinaryFile(binaryFileName, temp);
}