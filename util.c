#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "util.h"
void* calloc_2d(size_t width,size_t height)
{
	void**p = calloc(1,sizeof(void*)*(width*height+width));
	void*** one = (void*)p;
	p+=width;
	for(size_t i=0;i<width;i++)
	{
		one[i] = p;
		p+=height;
	}
	return one;
};
void free_2d(void* p)
{
	free(p);
};
void* calloc_3d(size_t x,size_t y, size_t z)
{
	void** p = calloc(1,sizeof(void*)*(x+x*y+x*y*z));
	void**** one = (void*)p;
	p+=x;

	for(size_t i=0;i<x;i++)
	{
		one[i] = (void*)p;
		p+=y;
	}
	for(size_t i=0;i<x;i++)
		for(size_t j=0;j<y;j++)
		{
			one[i][j] = p;
			p+=z;
		}
	return one;
}
void free_3d(void* p)
{
	free(p);
}
void clear_spaces(char* string)
{
	long len = strlen(string);
	char new_csv_string[len+1];
	long off=0;
	for(long i=0;i<len;i++)
	{
		if(string[i]==' ' || string[i]=='\t')
		{
		}else
		{
			new_csv_string[off++] = string[i];
		}
	}
	new_csv_string[off] = '\0';
	strcpy(string,new_csv_string);
}
static
void count_csv(const char* csv_string,int* columns,int* rows)
{
	int num_comma = 0;
	for(const char* p = csv_string;*p!='\n';p++)
	{
		if(*p==',')
		{
			num_comma++;
		}
	}
	*columns = num_comma+1;

	int num_line_sep = 0;
	for(const char*p = csv_string;*p!='\0';p++)
	{
		if(*p=='\n') num_line_sep++; }
	*rows = num_line_sep;

}
void clear_slash_enter(char*string)
{
	int len=strlen(string);
	char buf[len+1];
	int off=0;
	for(int i=0;i<len+1;i++)
	{
		if(string[i]=='\r')
		{
			continue;
		}
		buf[off] = string[i];
		off++;
	}
	strcpy(string,buf);
}
static
int parse_csv0(const char* csv_string,char*** sheets,int columns,int rows)
{
	long len = strlen(csv_string);
	char new_csv_string[len+1];
	strcpy(new_csv_string,csv_string);
	clear_spaces(new_csv_string);
	clear_slash_enter(new_csv_string);
	enum{SCAN_STRING,START,END,SKIP,FIND_NEXT_SHEET,IS_NULL}state=START;
	char* p=new_csv_string;
	int i=0/*column n*/,j=0;/*row n*/
	while(true)
	{
		switch(state)
		{
			case SCAN_STRING:;
					 int len=0;
					 char* p1=p;
					 while(!(*p1==','||*p1=='\0'||*p1=='\n'))//TODO \r
					 {
						 len++;
						 p1++;
					 }
					 char* str_new = malloc(len+1);
					 strncpy(str_new,p,len);
					 sheets[i][j] = str_new;
					 p=p1;
					 state=FIND_NEXT_SHEET;
					 break;
			case FIND_NEXT_SHEET:
					 if(*p==',')
					 {
						 p++;
						 i++;
						 state=IS_NULL;
						 break;
					 }else if(*p=='\n')
						 {
							 j++;
							 i=0;
							 p++;
							 state=IS_NULL;
							 break;
						 }else if(*p=='\0')
						 {
							 state=END;
							 break;
						 }
			case IS_NULL:
					 if(*p==',')//This sheet is null.
					 {
						 sheets[i][j]=NULL;
						 state=FIND_NEXT_SHEET;
						 break;
					 }else if(*p=='\0')
					 {
						 state=END;
						 break;
					 }
					 else
					 {
						 state=SCAN_STRING;
						 break;
					 }
			case START:
					 if(*p==',')
					 {
						 state=FIND_NEXT_SHEET;
						 break;
					 }else
					 {
						 state=SCAN_STRING;
						 break;
					 }
			case END:;goto end;
			default:;
		}
	}end:;
	return 0;
};
void parse_csv_string(const char* csv_str,csv_t* csv)
{
	int c,r;
	count_csv(csv_str,&c,&r);
	char*** map = calloc_2d(c,r);
	parse_csv0(csv_str,map,c,r);
	csv->columns = c;csv->rows=  r;
}
void destroy_csv(csv_t* csv)
{
	for(int i=0;i<csv->columns;i++)
		for(int j=0;j<csv->rows;j++)
		{
			if(csv->map[i][j]!=NULL)
			{
				free(csv->map[i][j]);
			}
		}
	free_2d(csv->map);
}


char* find_char(const char* string,char c)
{
	const char* p=string;
	while(true)
	{
		if(*p==c)
		{
			break;
		}
		p++;
	}
	return (char*)p;

}
char* next_line(const char* string)
{
	find_char(string,'\n');
	string++;
	return (char*)string;
}
bool get_value_by_key(const char* string,const char* key,char* value)
{
	int len = strlen(string);
	char new_str[len+1];
	strcpy(new_str,string);
	clear_spaces(new_str);
	clear_slash_enter(new_str);
	char* p = new_str;
	while(true)
	{
		if(*p=='\0')
			return false;
		if(strncmp(p,key,strlen(key))==0)
		{
			p = find_char(p,'=');
			p++;
			char* end = find_char(p,'\n');
			strncpy(value,p,end-p);
			value[end-p]='\0';
			return true;
		}
		p = next_line(p);
	}
}
int read_entire_file(const char* file_name,char* buffer)
{
	int fd = open(file_name,O_RDONLY);
	if(fd<0)
		return -1;
	size_t len =lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	read(fd,buffer,len);
	buffer[len] = '\0';
	close(fd);
	return len;
}
int readn(int fd , void* buf, int size)
{
	while(size>0)
	{
		int len =read(fd, buf, size);
		size-=len;
		buf+=len;
		if(size<0)
			return -1;
	}

	return 1;
}
