#include "PipeHelper.h"

#include "Message.h"

PipeHelper::PipeHelper(int pipe_id)
{
	create_new_pipe(pipe_id);
}

PipeHelper::PipeHelper()
{

}

bool PipeHelper::create_new_pipe(int id)
{
	//todo update out and in buffer size

	char path[256];

	strcpy_s(path, 256, string(pipe_path).append(to_string(id)).c_str());
	//strcpy_s(path, 256, string(pipe_path).c_str());
	cout << "open pipe with name" << path << "\n";
	printf("Creating pipe...");

	// ������� ����������� ����� ��� ������
	HANDLE hNamedPipe = CreateNamedPipeA(
		path,	// ��� ������
		PIPE_ACCESS_DUPLEX,		// ������ �� ������ � ����� � �����
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,	// ���������� �������� ���������
		1,		// ������������ ���������� ����������� ������ 
		0,		// ������ ��������� ������ �� ���������
		0,		// ������ �������� ������ �� ���������
		INFINITE,	// ������ ���� ����� 500 ��
		(LPSECURITY_ATTRIBUTES)NULL	// ������ �� ���������
	);

	// ��������� �� �������� ��������
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Creation of the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish server: ";
		system("pause");
		return (0);
	}
	pipe_handle = hNamedPipe;
	// ���� ���� ������ �������� � �������
	
	wait_for_connection();


	return true;

}

bool PipeHelper::open_pipe(int id)
{

	char path[256];

	strcpy_s(path, 256, string(pipe_path).append(to_string(id)).c_str());
	//strcpy_s(path, 256, string(pipe_path).c_str());
	cout << "open pipe with name" << path << "\n";

	// ����������� � ����������� �������
	HANDLE 	hNamedPipe = CreateFile(
		path,			// ��� ������
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// ��������� ������ ������ � �����
		(LPSECURITY_ATTRIBUTES)NULL,	// ������ �� ���������
		OPEN_EXISTING,			// ��������� ������������ �����
		0,
		(HANDLE)NULL			// �������������� ��������� ���
	);

	// ��������� ����� � �������
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		system("pause");
		return (0);
	}

	cout << "Connection with the named pipe: passed\n";
	pipe_handle = hNamedPipe;
}

bool PipeHelper::write_pipe(Message msg)
{
	if (!WriteFile(
		pipe_handle,		// ���������� ������
		&msg,		// ������
		sizeof(Message),	// ������ ������
		NULL,	// ���������� ���������� ����
		(LPOVERLAPPED)NULL	// ���������� ������
	))
	{
		cerr << "Writing to the named pipe failed: " << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		CloseHandle(pipe_handle);

		system("pause");

		exit(2);
	}
	return true;
}

Message PipeHelper::read_pipe()
{

	Message msg;
	char 	lpszInMessage[80];		// ��� ��������� �� �������
	DWORD 	dwBytesRead;				// ��� ����� ����������� ����

	if (!ReadFile(
		pipe_handle,			// ���������� ������
		&msg,			// ����� ������ ��� ����� ������
		sizeof(Message),		// ����� �������� ����
		NULL,			// ����� ����������� ����
		(LPOVERLAPPED)NULL		// �������� ������ ����������
	))
	{
		cerr << "Data reading from the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(pipe_handle);
		system("pause");
		cout << "Press any char to finish the server: ";

		exit(2);
	}

	return msg;
}

void PipeHelper::wait_for_connection()
{
	cout << "The server is waiting for connection with a client." << endl;
	if (!ConnectNamedPipe(
		pipe_handle,		// ���������� ������
		(LPOVERLAPPED)NULL	// ����� ����������
	))
	{
		cerr << "The connection failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(pipe_handle);
		cout << "Press any char to finish the server: ";
		system("pause");
		exit(2);
	}
	cout << "Success" << endl;
}

bool PipeHelper::close_pipe()
{
	printf("Disconnecting from stream...");

	BOOLEAN isAllGood = CloseHandle(pipe_handle);

	if (isAllGood == FALSE) {
		printf("Mistake!\n");
		system("pause");
		return false;
	}

	printf("\nSuccess!\n");

	return true;
}