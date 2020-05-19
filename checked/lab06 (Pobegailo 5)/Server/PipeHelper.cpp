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

	// создаем именованный канал дл€ чтени€
	HANDLE hNamedPipe = CreateNamedPipeA(
		path,	// им€ канала
		PIPE_ACCESS_DUPLEX,		// читаем из канала и пишем в канал
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,	// синхронна€ передача сообщений
		1,		// максимальное количество экземпл€ров канала 
		0,		// размер выходного буфера по умолчанию
		0,		// размер входного буфера по умолчанию
		INFINITE,	// клиент ждет св€зь 500 мс
		(LPSECURITY_ATTRIBUTES)NULL	// защита по умолчанию
	);

	// провер€ем на успешное создание
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Creation of the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish server: ";
		system("pause");
		return (0);
	}
	pipe_handle = hNamedPipe;
	// ждем пока клиент св€жетс€ с каналом
	
	wait_for_connection();


	return true;

}

bool PipeHelper::open_pipe(int id)
{

	char path[256];

	strcpy_s(path, 256, string(pipe_path).append(to_string(id)).c_str());
	//strcpy_s(path, 256, string(pipe_path).c_str());
	cout << "open pipe with name" << path << "\n";

	// св€зываемс€ с именованным каналом
	HANDLE 	hNamedPipe = CreateFile(
		path,			// им€ канала
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// разрешаем только запись в канал
		(LPSECURITY_ATTRIBUTES)NULL,	// защита по умолчанию
		OPEN_EXISTING,			// открываем существующий канал
		0,
		(HANDLE)NULL			// дополнительных атрибутов нет
	);

	// провер€ем св€зь с каналом
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
		pipe_handle,		// дескриптор канала
		&msg,		// данные
		sizeof(Message),	// размер данных
		NULL,	// количество записанных байт
		(LPOVERLAPPED)NULL	// синхронна€ запись
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
	char 	lpszInMessage[80];		// дл€ сообщени€ от клиента
	DWORD 	dwBytesRead;				// дл€ числа прочитанных байт

	if (!ReadFile(
		pipe_handle,			// дескриптор канала
		&msg,			// адрес буфера дл€ ввода данных
		sizeof(Message),		// число читаемых байт
		NULL,			// число прочитанных байт
		(LPOVERLAPPED)NULL		// передача данных синхронна€
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
		pipe_handle,		// дескриптор канала
		(LPOVERLAPPED)NULL	// св€зь синхронна€
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