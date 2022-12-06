#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include "std.h"
typedef struct json json_t;
enum 
{
	INVALID_REQUEST_EXCEPTION=1,

};
json_t* command_handler_data_arrive(json_t* massage,
		jmp_buf exception);
#endif
