#pragma once

#include <map>
#include <iostream>
#include "employee.h"
#include <fstream>
#include <string>
#include <Windows.h>
#include <map>
#include <vector>


class Message;
using namespace std;


class PipeHelper
{
private:
	HANDLE pipe_handle;
	LPCTSTR pipe_path = "\\\\.\\pipe\\mydemopipe";

public:
	PipeHelper(int pipe_id);

	explicit PipeHelper();

	bool create_new_pipe(int id);

	bool open_pipe(int id);

	bool close_pipe();

	bool write_pipe(Message msg);

	Message read_pipe();

	void wait_for_connection();

};

