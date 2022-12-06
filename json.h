#ifndef JSON_H
#define JSON_H
#include "std.h"
typedef struct json json_t;
json_t* json_new(const char*json_string);
void json_delete(json_t*);
char* json_string(json_t*);
//void json_delete_string(json_t*,char* json_string);
//char* json_get(const char* key);
//void json_delete_get(char*value);
//void json_set(const char* key, const char* value);
#endif
