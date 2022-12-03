#ifndef UTIL_H
#define UTIL_H
#include <stddef.h>

void* calloc_2d(size_t width,size_t height);
void free_2d(void* p);
void* calloc_3d(size_t x,size_t y, size_t z);
void free_3d(void* p);
void clear_spaces(char* string);
void clear_slash_enter(char*string);
typedef struct 
{
	char*** map;
	int columns,rows;

}csv_t;
void parse_csv_string(const char* csv_str,csv_t* csv);
void destroy_csv(csv_t* csv);
char* find_char(const char* string,char c);
char* next_line(const char* string);
bool get_value_by_key(const char* string,const char* key,char* value);
//return string length on success or -1 failure
int read_entire_file(const char* file_name,char* buffer);
int readn(int fd,void* buf,int buf_size);
//
#endif
