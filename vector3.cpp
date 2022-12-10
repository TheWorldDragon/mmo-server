#include "all.hpp"
vector<vector3_t>* vector3_t::load_obj_file_v(const char* file_name)
{
	int fd = open(file_name,O_RDONLY);
	assert(fd>=0);
	int len = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	char* buf = (char*)malloc(len+1);
	buf[len] = 0;
	assert(read(fd,buf,len)>=0);
	char* p = buf;
	vector<vector3_t>* vec = new vector<vector3_t>();
	while(true)
	{
		if(*p=='v' && p[1]==' ')
		{
			p++;
			float x,y,z;
			sscanf(p,"%f %f %f",&x,&y,&z);
			vector3_t point  = (vector3_t){.x=x,.y=y,.z=z};
			vec->push_back(point);
			while(*p!='\n')
				p++;
		}
		else if(*p=='\0')
			break;
		else
			p++;

	}
	free(buf);
	return vec;
}
vector<vector3_t>* vector3_t::load_obj_file_vn(const char* file_name)
{
	int fd = open(file_name,O_RDONLY);
	assert(fd>=0);
	int len = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	char* buf = (char*)malloc(len+1);
	buf[len] = 0;
	assert(read(fd,buf,len)>=0);
	char* p = buf;
	vector<vector3_t>* vec = new vector<vector3_t>();
	while(true)
	{
		if(*p=='v' && p[1]=='t')
		{
			p++;
			float x,y,z;
			sscanf(p,"%f %f %f",&x,&y,&z);
			vector3_t point  = (vector3_t){.x=x,.y=y,.z=z};
			vec->push_back(point);
			while(*p!='\n')
				p++;
		}
		else if(*p=='\0')
			break;
		else
			p++;

	}
	free(buf);
	return vec;
}
float vector3_t::operator*(vector3_t& o)
{
	return x*o.x+y*o.y+z*o.z;
}
