#include "all.h"
//static json_t* self_massage;
static bool login(const char* ,const char*);
json_t* command_handler_data_arrive(json_t* massage,jmp_buf exception)
{
	assert(massage!=NULL);
	json_t* return_massage = NULL;//TODO
	char* json_str = json_string(massage);
	cJSON* cjson = cJSON_Parse(json_str);
	assert(cjson!=NULL);
	cJSON* command_str= cJSON_GetObjectItem(cjson,"command");
	assert(command_str!=NULL);
	if(strcmp(command_str->valuestring,"login")==0)
	{
		if(false)
		{ exc:;
			free(json_str);
			cJSON_free(cjson);
			longjmp(exception,INVALID_REQUEST_EXCEPTION);
		}
		char* account = cJSON_GetObjectItem(cjson,"account")->valuestring;
		if(account==NULL)
			goto exc;
		char* password = cJSON_GetObjectItem(cjson,"password")->valuestring;
		if(password==NULL)
			goto exc;
		if(login(account,password))
		{
			return_massage = json_new("{\"version\":\"1.0\",\"command\":\"login_return\",\"is_login\":true}");

		}
		else
		{
			return_massage = json_new("{\"version\":\"1.0\",\"command\":\"login_return\",\"is_login\":false}");
		}

	}
	else if(strcmp(command_str->valuestring,"register")==0)
	{

	}
	else if(strcmp(command_str->valuestring,"")==0)
	{
	}
	else if(strcmp(command_str->valuestring,"")==0)
	{
	}
	else if(strcmp(command_str->valuestring,"")==0)
	{
	}

	//TODO

	cJSON_free(cjson);
	free(json_str);
	assert(return_massage!=NULL);
	return return_massage;
}
bool login(const char* account,const char* password)
{
	//TODO
	assert(false);
	return false;
}
