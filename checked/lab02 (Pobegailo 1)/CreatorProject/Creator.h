#pragma once
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include "Employee.h"
class Creator
{
public:
	static void createBinaryFile(std::string& binaryFileName, int recordNumber);
};

