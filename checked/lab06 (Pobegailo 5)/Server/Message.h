#pragma once

#include "employee.h"

enum MessageStatus
{
	access_denied,
	access_allowed,

	read_request,
	write_request,
	release_record,
	update_record,
	say_goodbye
};

class Message
{
public:
	int access_id;
	MessageStatus status;
	employee record;
};

