#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;
#define MESSAGE_SIZE 20
string event_name = "Event_";

int create_new_process(const string& binary_file_name, int amount_of_record, int order);

int create_new_file(const string& file_name);

HANDLE create_new_event(int order);

HANDLE create_new_mutex();

int user_action();

void read_file(const string& file_name);

int main()
{
	cout << "Hello, with that process your can create and read message from file.\nGood luck!\n";
	string bin_file_name;
	auto events = new vector<HANDLE>;;
	const auto h_mutex=create_new_mutex();
	int amount_of_records, amount_of_process;
	
	cout << "Please, input binary file name: ";
	cin >> bin_file_name;
	cout << "Please, input amount of records in binary file: ";
	cin >> amount_of_records;

	cout << "Enter amount of process to create: ";
	cin >> amount_of_process;

	for (int i = 0; i < amount_of_process; i++)
	{
		create_new_process(bin_file_name, amount_of_records, i);
		events->push_back(create_new_event(i));
	}
	create_new_file(bin_file_name);
	WaitForMultipleObjects(events->size(), events->data(), TRUE, INFINITE);

	while (true)
	{
		int choice = user_action();
		switch (choice)
		{
		case 1:
			WaitForSingleObject(h_mutex, INFINITE);
			read_file(bin_file_name);
			ReleaseMutex(h_mutex);
			break;
		case 2:
			delete events;
			CloseHandle(h_mutex);
			return 0;
		default:
			cout << "Unknown command\n";
		}
	}
}

int create_new_process(const string& binary_file_name, const int amount_of_record, int order)
{

	const string path = "D:\\studies\\4 sem\\OS\\lab05\\Project1\\Debug\\sender.exe " + binary_file_name + " " +
		to_string(amount_of_record)+" "+to_string(order);

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;

	if (CreateProcessA(NULL, (LPSTR)path.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "Process Creator_"<<order<<" was created successfully..." << endl;
	}
	else {
		cout << "Process Creator wasn't created..." << endl;
		cout << "Close application..." << endl;
		system("pause");
		exit(-1);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return (0);
}

int create_new_file(const string& file_name)
{
	ofstream fout(file_name.c_str(), ios_base::out | ios_base::trunc);
	fout.close();
	return 0;
}


HANDLE create_new_event(const int order)
{
	const auto name = event_name+to_string(order);

	const auto h_in_event = CreateEvent(NULL, FALSE, FALSE, (name.c_str()));
	if (h_in_event == NULL)
	{
		cout << "Event don't create\n";
		exit(-2);
	}
	return h_in_event;
}

int user_action()
{
	cout << "Hello, choice one option, enter \n";
	cout << "1\tto check file with message\n";
	cout << "2\tto exit from program\n";
	int choice;
	cin >> choice;
	return choice;
}

HANDLE create_new_mutex()
{
	const auto h_mutex = CreateMutex(NULL, FALSE, "ShareFileMutex");
	if (h_mutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return nullptr;
	}
	return h_mutex;
}

void read_file(const string& file_name)
{
	char mess[20];
	ifstream binary_fin(file_name, std::ios::in);
	cout << "**********************************************************************" << endl;

	cout << "Read message from file" << endl;
	
	while (binary_fin.read(reinterpret_cast<char*>(&mess), 20 )) {
		cout << mess << "\n";
	}
	cout << "**********************************************************************\n\n" << endl;
}