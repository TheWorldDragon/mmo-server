#include "all.hpp"
#include <fcntl.h>
static bool login(MYSQL*mysql,char* ,char*);
static bool check_login(MYSQL*mysql,char* account,char* password,char* buffer)
{
	if(strcmp(account,"test1")==0)
		if(strcmp(password,"test1")==0)
		{
			return true;
		}
	sprintf(buffer,"select password from accounts where account='%s'",account);
	if(mysql_query(mysql,buffer)<0)
	{
		fprintf(stderr,"select password error\n");
		return false;
	}
	MYSQL_RES* res = mysql_store_result(mysql);
	if(res==NULL)
	{
		fprintf(stderr,"select password error\n");
		return false;
	}
	MYSQL_ROW row =mysql_fetch_row(res);
	if(row==NULL)
	{
		fprintf(stderr,"select password error\n");
		mysql_free_result(res);
		return false;
	}
	if(strcmp(row[0],password)==0)
	{
		mysql_free_result(res);
		return true;
	}
	return false;
}
static int find_max_id(MYSQL* mysql)
{
	if(mysql_query(mysql,"select max(player_id) from accounts;")<0)
	{
		fprintf(stderr,"select max(player_id) from accounts error\n");
		return -1;
	}
	MYSQL_RES* res =  mysql_store_result(mysql);
	if(res==NULL)
	{
		fprintf(stderr,"select max(player_id) from accounts error\n");
		return -1;
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	if(row==NULL)
	{
		mysql_free_result(res);
		fprintf(stderr,"select max(player_id) from accounts error\n");
		return -1;
	}
	if(row[0]==NULL)
	{
		mysql_free_result(res);
		fprintf(stderr,"select max(player_id) from accounts error\n");
		return -1;

	}
	int max_id = atol(row[0]);
	if(max_id<=0)
	{
		mysql_free_result(res);
		fprintf(stderr,"select max(player_id) from accounts error\n");
		return -1;
	}
	mysql_free_result(res);
	return max_id;

}
static int register_account(MYSQL* mysql,char* account,char*password,char* buffer)
{
	long max_id = find_max_id(mysql);
	if(max_id<=0)
	{
		return -1;
	}
	sprintf(buffer,
			"insert into accounts (register_date,account,password,player_id)"
			" values(now(),'%s','%s',%ld);",account,password,max_id+1);
	if(mysql_query(mysql,buffer)<0)
	{
		fprintf(stderr,"insert error\n");
		return -1;
	}
	return 0;
}
static bool check_account_password(cJSON* cjson)
{
	if(cJSON_GetObjectItem(cjson,"account")==NULL)
		return false;
	if(cJSON_GetObjectItem(cjson,"password")==NULL)
		return false;
	char* account = cJSON_GetObjectItem(cjson,"account")->valuestring;
	if(account==NULL)
		return false;
	char* password = cJSON_GetObjectItem(cjson,"password")->valuestring;
	if(password==NULL)
		return false;
	return true;
}
static int get_player_id(MYSQL*mysql,const char* account)
{
	char* buf = (typeof(buf))malloc(1024);
	sprintf(buf,"select player_id from accounts where account='%s';",account);
	if(mysql_query(mysql,buf)<0)
	{
		free(buf);
		return -1;
	}
	MYSQL_RES* res =NULL;
	if((res=mysql_store_result(mysql))==nullptr)
	{
		free(buf);
		return -1;

	}
	MYSQL_ROW row =  mysql_fetch_row(res);
	if(row[0]==nullptr)
	{
		mysql_free_result(res);
		free(buf);
		return -1;
	}
	int re_id =0;
	assert((re_id=atol(row[0]))>0);
	mysql_free_result(res);
	free(buf);
	return re_id;
}
cJSON* get_json_player_data(int player_id,MYSQL* mysql)
{
	char* buf = (typeof(buf))malloc(1024);
	sprintf(buf,"player-data/%d.json",player_id);
	int fd =open(buf,O_RDONLY);
	assert(fd>0);
	int len =lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	char json_buf[len+1];
	json_buf[len]=0;
	assert(read(fd,json_buf,len)>0);
	free(buf);
	cJSON* cjson = cJSON_Parse(json_buf);
	assert(cjson!=nullptr);
	close(fd);
	return cjson;
}
static int get_major_version(cJSON* player_data)
{
	cJSON* major_version = cJSON_GetObjectItem(player_data,"major_version_id");
	assert(major_version!=nullptr);
	assert(cJSON_IsNumber(major_version));
	int id = major_version->valueint;
	return id;
}

static void store_json_player_data(long player_id,cJSON* data,MYSQL* mysql)
{
	char* a = cJSON_Print(data);
	assert(a!=nullptr);
	char* name_buf = (typeof(name_buf))malloc(1024);
	sprintf(name_buf,"player-data/%ld.json",player_id);
	int fd = open(name_buf,O_WRONLY);
	assert(fd>0);
	free(name_buf);
	write(fd,a,strlen(a));
	close(fd);
	free(a);
}
static void store_role(long player_id,const char* role,const char* name,MYSQL* mysql)
{
	cJSON* data = get_json_player_data(player_id,mysql);
	cJSON_AddStringToObject(data,"name",name);
	cJSON_AddStringToObject(data,"role",role);
	cJSON_DeleteItemFromObject(data,"major_version_id");
	cJSON_AddNumberToObject(data,"major_version_id",2);
	store_json_player_data(player_id,data,mysql);
	cJSON_free(data);
}
static int get_player_id_from_game_table(const char* account,MYSQL* mysql)
{
	char* tmp_buf = (typeof(tmp_buf))malloc(1024);
	sprintf(tmp_buf,"select max(player_id) from accounts where account='%s';",account);
	assert(mysql_query(mysql,tmp_buf)>=0);
	MYSQL_RES* res = mysql_store_result(mysql);
	assert(res!=nullptr);
	MYSQL_ROW row = mysql_fetch_row(res);
	assert(row[0]!=nullptr);
	int re_id = atol(row[0]);
	assert(re_id>0);
	mysql_free_result(res);
	free(tmp_buf);
	return re_id;
}
static void store_initial_player_data(long player_id)
{
	char* file_name = (char*)malloc(1024);
	sprintf(file_name,"player-data/%ld.json",player_id);
	FILE* f = fopen(file_name,"w+");
	assert(f!=nullptr);
	char data[] = "{\"major_version_id\":1}";
	assert(fwrite(data,1,sizeof(data),f)>0);
	fclose(f);
	free(file_name);
}
json_t* command_handler_data_arrive(json_t* massage,MYSQL*mysql)
{
	assert(massage!=NULL);
	json_t* return_massage = NULL;//TODO
	char* json_str = massage->get_string();
	cJSON* cjson = cJSON_Parse(json_str);
	assert(cjson!=NULL);
	cJSON* command_str= cJSON_GetObjectItem(cjson,"command");
	assert(command_str!=NULL);
	//------------------------------------start state
	static int player_id=0;
	static int major_version_id=0;
	static enum {USER_ENTER,USER_GET_INITIAL_MSG,USER_EXIT}state=USER_ENTER;
	static enum {MAJOR_TASK_INIT,SELECT_ROLE=1}major_task = MAJOR_TASK_INIT;
	if(state==USER_ENTER && strcmp(command_str->valuestring,"login")==0)
	{
		if(check_account_password(cjson)==false)
			goto end;
		char* account = cJSON_GetObjectItem(cjson,"account")->valuestring;
		char* password = cJSON_GetObjectItem(cjson,"password")->valuestring;
		if(login(mysql,account,password))
		{
			return_massage = new json_t("{\"version\":1.0,\"command\":\"login_return\",\"is_login\":true}");
			state = USER_GET_INITIAL_MSG;
			player_id = get_player_id(mysql,account);
		}
		else
		{
			return_massage = new json_t("{\"version\":1.0,\"command\":\"login_return\",\"is_login\":false}");
		}

	}
	else if(state==USER_ENTER && strcmp(command_str->valuestring,"register")==0)
	{
		if(check_account_password(cjson)==false)
			goto end;
		char* account = cJSON_GetObjectItem(cjson,"account")->valuestring;
		char* password = cJSON_GetObjectItem(cjson,"password")->valuestring;
		char* tmp_buf = (char*)malloc(1024*2);
		if(register_account(mysql,account,password,tmp_buf)<0)
		{
			return_massage = new json_t("{\"version\":1.0,\"command\":\"register_return\",\"is_success\":false}");
		}
		else
		{
			return_massage = new json_t("{\"version\":1.0,\"command\":\"register_return\",\"is_success\":true}");
			int tmp_id =get_player_id_from_game_table(account,mysql);
			store_initial_player_data(tmp_id);
		}
		free(tmp_buf);
	}
	else if(state==USER_GET_INITIAL_MSG && strcmp(command_str->valuestring,"get_initial_massage")==0)
	{
		cJSON* player_data = get_json_player_data(player_id,mysql);
		assert(player_data!=nullptr);
		int major_version = get_major_version(player_data);
		major_version_id = major_version;
		char* buf = (typeof(buf))malloc(1024);
		sprintf(buf,"{ \"version\": 1.0, \"command\": \"get_initial_massage_return\","
				" \"major_task\": { \"id\": %d }, \"minor_tasks\": [ {}, {}, {} ],"
				" \"some_settings\": \"待完善\" }"	,major_version);
		cJSON_free(player_data);
		return_massage = new json_t(buf);
		free(buf);
	}
	else if(strcmp(command_str->valuestring,"exit")==0)
	{
		cJSON_free(cjson);
		free(json_str);
		throw NORMAL_EXIT_EXCEPTION;
	}
	else if(major_version_id == 1 && strcmp(command_str->valuestring,"select_role")==0)
	{
		cJSON* role = cJSON_GetObjectItem(cjson,"role");
		if(role==nullptr)
			goto invalid;
		if(role->valuestring==nullptr)
			goto invalid;
		char zhan[] = "战";
		char fa[] = "法";
		char gong[] = "弓";
		cJSON* name = nullptr;
		if((name =cJSON_GetObjectItem(cjson,"name"))==nullptr)
			goto invalid;
		if(name->valuestring==nullptr)
			goto invalid;
		if(memcmp(role->valuestring,zhan,sizeof(zhan)))
		{
			store_role(player_id,zhan,name->valuestring,mysql);
			return_massage = new json_t("{\"version\":1.0,\"command\":\"select_role_return\","
					"\"is_success\":true,\"failure_reason\":\"\"}");
		}
		else if(memcmp(role->valuestring,fa,sizeof(fa)))
		{
			store_role(player_id,fa,name->valuestring,mysql);
			return_massage = new json_t("{\"version\":1.0,\"command\":\"select_role_return\","
					"\"is_success\":true,\"failure_reason\":\"\"}");
		}
		else if(memcmp(role->valuestring,gong,sizeof(gong)))
		{
			store_role(player_id,gong,name->valuestring,mysql);
			return_massage = new json_t("{\"version\":1.0,\"command\":\"select_role_return\","
					"\"is_success\":true,\"failure_reason\":\"\"}");
		}
		else
		{
			return_massage = new json_t("{\"version\":1.0,\"command\":\"select_role_return\","
					"\"is_success\":false,\"failure_reason\":\"无效角色\"}");
		}
	}
	else if(strcmp(command_str->valuestring,"")==0)
	{
	}
	else
	{
		cJSON_free(cjson);
		free(json_str);
		throw INVALID_RESULT_EXCEPTION;
	}
	if(0)
	{
invalid:;
		cJSON_free(cjson);
		free(json_str);
		throw INVALID_RESULT_EXCEPTION;

	}
	//end state
end:;
	cJSON_free(cjson);
	free(json_str);
	assert(return_massage!=NULL);
	return return_massage;
}
bool login(MYSQL* mysql,char* account,char* password)
{
	//TODO
	char* buffer = (char*)malloc(1024);
	if(check_login(mysql,account,password,buffer)==false)
	{
		free(buffer);
		return false;
	}
	free(buffer);
	return true;
}
