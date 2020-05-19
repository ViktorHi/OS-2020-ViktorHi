#include "Message.h"
#include "PipeHelper.h"
#include "employee.h"

#include <windows.h>
#include <conio.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>


using namespace std;


CRITICAL_SECTION cs_data;

class TaskHelper
{
public:
	
	typedef struct MyData {
		int* array;
		int array_size;
	} MYDATA, * PMYDATA;
	
	class RecordInfo
	{
	public:
		enum RecordStatus
		{
			free,
			read,
			write
		};
		
		RecordStatus status;
		int amount = 0;
		
		RecordInfo()
		{
			amount = 0;
			status = RecordStatus::free;
		}

		void release()
		{
			if(status == RecordStatus::write)
			{
				status= RecordStatus::free;
			}

			if(status == RecordStatus::read)
			{
				amount--;
				if(amount == 0)
				{
					status = RecordStatus::free;
				}
			}
			
		}

		void set_read()
		{
			status = RecordStatus::read;
			amount++;
		}

		void set_write()
		{
			status = RecordStatus::write;
		}
	};

	static string file_name;
	static int process_amount;

	static map<int, int> dictionary;
	static map<int, RecordInfo> record_statuses;
	
	static vector<HANDLE> threads;


	TaskHelper(int proc_amount, string afile_name)
	{
		
		file_name = afile_name;
		process_amount = proc_amount;
		threads = vector<HANDLE>(proc_amount);
	}


	int main()
	{

		create_file();

		cout << "Enter amounts of records ";
		int record_amounts;
		cin >> record_amounts;

		for (int i = 0; i < record_amounts; i++)
		{
			insert_new_record();
		}


		read_file();

		InitializeCriticalSection(&cs_data);
		for (int i = 0; i < process_amount; i++)
		{
			PMYDATA param = nullptr;
			threads[i] = create_new_thread(param, thread_function);
		}

		
		WaitForMultipleObjects(threads.size(), threads.data(), TRUE, INFINITE);
		
		DeleteCriticalSection(&cs_data);
		
		read_file();

		system("pause");
		return 0;
	}


	//region work with file
	void create_file()
	{
		ofstream fout(file_name.c_str(), ios_base::out | ios_base::trunc);
		fout.close();

	}

	static void read_file()
	{
		employee my_employee;
		std::ifstream binary_fin(file_name, std::ios::binary);

		while (binary_fin.read((char*)&my_employee, sizeof(employee))) {
			cout
				<< "\nId: \t" << my_employee.num
				<< "\nName: \t" << my_employee.name
				<< "\nHours: \t" << my_employee.hours << "\n";
		}

		binary_fin.close();
	}

	void insert_new_record()
	{
		ofstream fout(file_name, std::ios::app | std::ios::ate);

		auto employee = fill_struct();

		dictionary.insert(make_pair(employee.num, fout.tellp()));

		record_statuses.insert(make_pair(employee.num, RecordInfo()));

		fout.write(reinterpret_cast<char*>(&employee), sizeof(employee));

		fout.close();
	}

	static employee get_record_by_id(int record_id)
	{
		std::ifstream fin(file_name);

		fin.seekg(dictionary.at(record_id));

		employee emp{};

		fin.read(reinterpret_cast<char*>(&emp), sizeof(emp));

		return emp;
	}

	static void update_record_by_id(int record_id, employee emp)
	{

		std::ofstream fout(file_name, std::ios::binary||std::ios::app);

		fout.seekp(dictionary.at(record_id));

		fout.write(reinterpret_cast<char*>(&emp), sizeof(emp));

		fout.close();

	}
	//endregion


	employee fill_struct()
	{
		employee ans{};

		std::cout << "Enter employee ID ";
		std::cin >> ans.num;
		std::cout << "Enter employee name ";
		std::cin >> ans.name;
		std::cout << "Enter employee worked hours ";
		std::cin >> ans.hours;

		return ans;
	}

	HANDLE create_new_thread(MYDATA* param, void(*f)(LPVOID))
	{
		HANDLE h_thread = CreateThread(
			NULL,
			0,
			LPTHREAD_START_ROUTINE(f),
			param,
			0,
			NULL);

		if (h_thread == NULL)
		{
			cout << "Thread can not be created" << endl;
			exit(1);
		}

		return h_thread;
	}

	static int create_new_process(int thread_id)
	{

		const string path = "D:\\studies\\4 sem\\OS\\lab06\\again\\Server\\Debug\\Client.exe";

		STARTUPINFO si;
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION pi;

		if (CreateProcessA(NULL, (LPSTR)path.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		{
			cout << "Process Creator from thread_" << thread_id << " was created successfully with id " << pi.dwProcessId << endl;
		}
		else {
			cout << "Process Creator wasn't created..." << endl;
			cout << "Close application..." << endl;
			system("pause");
			exit(-1);
		}

		int ans = pi.dwProcessId;

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return ans;
	}



	static void thread_function(const LPVOID lp_parameter)
	{
		auto data = static_cast<PMYDATA>(lp_parameter);

		int process_id = create_new_process((int)GetCurrentThreadId());

		PipeHelper pipe = PipeHelper(process_id);

		Message msg{}, msg_to_send{};
		RecordInfo record_info;

		while (true)
		{
			msg = pipe.read_pipe();

			EnterCriticalSection(&cs_data);
			if(msg.access_id != -1)
				record_info = record_statuses.at(msg.access_id);
			LeaveCriticalSection(&cs_data);

			//cout <<"Before server"<<record_statuses.at(msg.access_id).status << "  :  " << record_statuses.at(msg.access_id).amount << "\n";
			
			msg_to_send.access_id = msg.access_id;

			switch (msg.status)
			{
			case (MessageStatus::read_request):
				
				msg_to_send.access_id = msg.access_id;
				
				switch (record_info.status)
				{
				case (RecordInfo::RecordStatus::free):
				case (RecordInfo::RecordStatus::read):
					
					EnterCriticalSection(&cs_data);
					if(record_statuses.at(msg.access_id).status == RecordInfo::RecordStatus::read||
						record_statuses.at(msg.access_id).status == RecordInfo::RecordStatus::free)
					{
						record_statuses[msg.access_id].set_read();
						msg_to_send.record = get_record_by_id(msg.access_id);
						msg_to_send.status = MessageStatus::access_allowed;
					}else
					{
						msg_to_send.status = MessageStatus::access_denied;
					}
					LeaveCriticalSection(&cs_data);

					break;

				case (RecordInfo::RecordStatus::write):
					
					msg_to_send.status = MessageStatus::access_denied;

					break;
				}

				pipe.write_pipe(msg_to_send);

				break;


			case (MessageStatus::write_request):

				msg_to_send.access_id = msg.access_id;
				
				switch (record_info.status)
				{
				case(RecordInfo::RecordStatus::read):
					msg_to_send.status = MessageStatus::access_denied;

					break;
				case(RecordInfo::RecordStatus::write):
					msg_to_send.status = MessageStatus::access_denied;

					break;
				case(RecordInfo::RecordStatus::free):
					
					EnterCriticalSection(&cs_data);
					if (record_statuses.at(msg.access_id).status == RecordInfo::RecordStatus::free)
					{
						msg_to_send.status = MessageStatus::access_allowed;
						msg_to_send.record = get_record_by_id(msg.access_id);
						record_statuses[msg.access_id].set_write();
					}
					else
					{
						msg_to_send.status = MessageStatus::access_denied;
					}
					LeaveCriticalSection(&cs_data);
					
					break;
				}

				pipe.write_pipe(msg_to_send);

				break;

			case (MessageStatus::update_record):
				update_record_by_id(msg.access_id, msg.record);

				break;
			case (MessageStatus::release_record):
				
				EnterCriticalSection(&cs_data);
				record_statuses[msg.access_id].release();
				LeaveCriticalSection(&cs_data);
				break;

			case (MessageStatus::say_goodbye):
				pipe.close_pipe();
				end_session();
				return;
				break;
			}

			//cout<<"after server" <<record_statuses.at(msg.access_id).status << "  :  "<<record_statuses.at(msg.access_id).amount << "\n";

		}

	}

	static void end_session()
	{

		cout << "by from thread" << static_cast<int>(GetCurrentThreadId()) << '\n';;
	}
};


map<int, TaskHelper::RecordInfo> TaskHelper::record_statuses = map<int, TaskHelper::RecordInfo>();
int TaskHelper::process_amount = 0;
string TaskHelper::file_name = string();
map<int, int> TaskHelper::dictionary = map<int, int>();
vector<HANDLE> TaskHelper::threads = vector<HANDLE>();


int main()
{

	string file_name;
	int process_amount = 0;

	cout << "Enter file name ";
	cin >> file_name;

	cout << "Enter process amounts ";
	cin >> process_amount;


	TaskHelper helper(process_amount, file_name);

	helper.main();

}

