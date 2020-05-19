#include <iostream>
#include <Windows.h>


#include "../Server/Message.h"
#include "../Server/PipeHelper.h"


using namespace std;

class Client
{
	enum class WorkMode
	{
		read,
		modify,
		out
	};

	static PipeHelper pipe_helper;

	//region client
	static bool work_with_request()
	{
		while (true)
		{
			WorkMode mode = user_choice();

			switch (mode)
			{
			case (WorkMode::modify):
			{
				int request_id = get_record_id();

				if (send_request(request_id, MessageStatus::write_request))
				{
					employee new_record = update_struct();

					new_record.num = request_id;

					pull_changes(new_record);
					
					release_record(request_id);
				}

				break;
			}

			case (WorkMode::read):
			{
				int record_id = get_record_id();

				if(send_request(record_id, MessageStatus::read_request))
				{
					release_record(record_id);
				}


				break;
			}

			case (WorkMode::out):
			{
				say_goodbye();
				return true;
				break;
			}
			}
		}

	}

	static WorkMode user_choice()
	{
		bool isUncorrect = false;
		WorkMode mode;
		do
		{
			cout << "choice one way:\n";
			cout << "(1)\ttry to modify record by id\n";
			cout << "(2)\ttry to read record by id\n";
			cout << "(3)\t finish work\n";

			int choice = 0;
			cin >> choice;

			if (choice == 1) return WorkMode::modify;

			if (choice == 2) return WorkMode::read;

			if (choice == 3) return WorkMode::out;

			isUncorrect = true;

		} while (isUncorrect);
	}

	static int get_record_id()
	{
		int id;

		cout << "enter record id ";
		cin >> id;

		return id;
	}

	static bool send_request(int record_id, MessageStatus status)
	{
		Message to_send;
		to_send.access_id = record_id;
		to_send.status = status;

		pipe_helper.write_pipe(to_send);

		Message msg = pipe_helper.read_pipe();

		switch (msg.status)
		{
		case (MessageStatus::access_allowed):

			read_message(msg);
			return true;

			break;
		case(MessageStatus::access_denied):

			cout << "access denied, please try latter ";
			return false;
			break;
		}
		return false;
	}

	static void pull_changes(employee record)
	{
		cout << "Enter any key to pull changes ";
		char any_key;
		cin >> any_key;

		Message msg;
		msg.status = MessageStatus::update_record;
		msg.record = record;
		msg.access_id = record.num;

		pipe_helper.write_pipe(msg);
	}

	static void release_record(int record_id)
	{
		cout << "Enter any key to release record ";
		char any_key;
		cin >> any_key;

		Message msg;
		msg.status = MessageStatus::release_record;
		msg.access_id = record_id;

		pipe_helper.write_pipe(msg);
	}
	//endregion client

	//region work with messages
	static void say_goodbye()
	{
		Message msg;
		msg.status = MessageStatus::say_goodbye;
		msg.access_id = -1;
		
		pipe_helper.write_pipe(msg);
	}

	static void read_message(Message msg)
	{
		cout << "record by id from server\n";
		cout
			<< "\nId: \t" << msg.record.num
			<< "\nName: \t" << msg.record.name
			<< "\nHours: \t" << msg.record.hours << "\n";
	}

	static employee update_struct()
	{
		employee ans;

		std::cout << "Enter employee name ";
		std::cin >> ans.name;
		std::cout << "Enter employee worked hours ";
		std::cin >> ans.hours;

		return ans;
	}
	//endregion work with messages

public:

	static int main()
	{
		cout << "hello from client process ";

		cout << to_string(GetCurrentProcessId()) << endl;

		if (!pipe_helper.open_pipe(GetCurrentProcessId()))
		{
			cout << "close app\n";
			return 1;
		};

		if (!work_with_request())
		{
			cout << "close app\n";
			return 2;
		};

		if (!pipe_helper.close_pipe())
		{
			cout << "close app\n";
			return 3;
		}

		system("pause");
		return 0;
	}

};

PipeHelper Client::pipe_helper = PipeHelper();

int main()
{
	return Client::main();
}
