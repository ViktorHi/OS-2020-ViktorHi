#include <iostream>
#include <thread>
#include <Windows.h>

using namespace std;
int main() {
	while (true) {
		Sleep(1000);
		cout << "hI from 2 proc" << endl;
	}
}