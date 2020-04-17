#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const string event_name = "Event_";
bool is_filled = false;

void send_event(int order);

int user_action();

void write_message(const string& file_name);

HANDLE open_mutex();

bool is_ready_to_write(const string& file_name, const int max_amount);


int main(int argc, char* argv[]) {

	cout << "Hello, your can send message by using that process\nGood luck!\n"  << "\n";
	const auto binary_flie_name = argv[1];
	const auto amount_of_mess = atoi(argv[2]);
	const auto order = atoi(argv[3]);
		
	send_event(order);
	const auto h_mutex = open_mutex();
	
	while(true)
	{
		const auto choice = user_action();
		switch (choice)
		{
		case 2:
			WaitForSingleObject(h_mutex, INFINITE);
			is_filled=!is_ready_to_write(binary_flie_name, amount_of_mess);
			if(!is_filled)
				write_message(binary_flie_name);
			ReleaseMutex(h_mutex);
			break;
		case 1:
			CloseHandle(h_mutex);
			return 0;
		default:
			cout << "Unknown command\n";
		}
	}

}


void send_event(const int order)
{
	char lp_event_name[256];
	const auto name = event_name + to_string(order);
	strcpy_s(lp_event_name, 256, name.c_str());

	const auto h_in_event = OpenEvent(EVENT_MODIFY_STATE, FALSE, (lp_event_name));
	if (h_in_event == NULL)
	{
		cout << "Open event11 failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
	}
	SetEvent(h_in_event);
	CloseHandle(h_in_event);
}

int user_action()
{
	cout << "Choice one option, enter \n";
	cout << "1\tto exit from program\n";
	if (is_filled)
	{
		cout << "Sorry, but your can't send message because file is filled\n";
	}else
	{
		cout << "2\tto write message in file\n";
	}

	int choice;
	cin >> choice;
	return choice;
}

HANDLE open_mutex(){
	
	const auto h_mutex = OpenMutex(SYNCHRONIZE, FALSE, "ShareFileMutex");
	if (h_mutex == NULL)
	{
		cout << "Open mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		exit(-1);
	}
	return h_mutex;
}
void write_message(const string& file_name)
{
	ofstream fout(file_name, std::ios::app);
	
	cout << "Enter your message: ";
	char mess[20];
	scanf_s("%19s", &mess,19);
	
	fout.write(reinterpret_cast<char*>(&mess), 20);
	fout.close();
}

bool is_ready_to_write(const string& file_name, const int max_amount)
{
	char mess[20];
	ifstream binary_fin(file_name, std::ios::in);
	auto current_amount = 0;
	
	while (binary_fin.read(reinterpret_cast<char*>(&mess), 20)) {
		current_amount++;
	}
	
	return max_amount > current_amount ? true : false;
}