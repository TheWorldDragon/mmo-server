#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP
class json_t;
class MYSQL;
#define INVALID_RESULT_EXCEPTION -1
#define NORMAL_EXIT_EXCEPTION -2
json_t* command_handler_data_arrive(json_t* massage,MYSQL* mysql);
#endif
