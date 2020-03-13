#include <iostream> 
#include <thread>
#include <Windows.h>


using namespace std;
int main() {

	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	if (CreateProcess("D:\\studies\\4 sem\\OS\\ProjectForLAB01\\Debug\\ProjectForLAB01.exe", NULL,NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &cif, &pi) == TRUE)
	{

		cout << "Hi, frome process " << pi.hProcess << endl;
		Sleep(1000);			
	}
	else {
		cout << "Something goes wrong" << endl;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
