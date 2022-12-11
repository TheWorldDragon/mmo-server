


#include "all.hpp"
#include <fcntl.h>
static void build_true(vector<vector3_t>* v,vector<vector3_t>* vn,const char* obj_file_name,array3_t<bool*>* view)
{


}
#if 0
bool game_map_t::has_cube(int x, int y, int z)
{
	if(x>0 && y >0 && z > 0)//1
	{
		return *view[0]->get(x-1,y-1,z-1);

	}
	else if(x<0 && y >0 && z > 0)//2
	{
		x = abs(x);y = abs(y); z = abs(z);
		return *view[1]->get(x-1,y-1,z-1);
	}
	else if(x<0 && y <0 && z > 0)//3
	{
		x = abs(x);y = abs(y); z = abs(z);
		return *view[2]->get(x-1,y-1,z-1);
	}
	else if(x>0 && y <0 && z > 0)//4
	{
		x = abs(x);y = abs(y); z = abs(z);
		return *view[3]->get(x-1,y-1,z-1);
	}
	else if(x>0 && y >0 && z < 0)//5
	{
		x = abs(x);y = abs(y); z = abs(z);
		return *view[4]->get(x-1,y-1,z-1);
	}
	else if(x<0 && y >0 && z < 0)//6
	{
		x = abs(x);y = abs(y); z = abs(z);
		return *view[5]->get(x-1,y-1,z-1);
	}
	else if(x<0 && y <0 && z < 0)//7
	{
		x = abs(x);y = abs(y); z = abs(z);
		return *view[6]->get(x-1,y-1,z-1);
	}
	else if(x>0 && y <0 && z < 0)//8
	{
		x = abs(x);y = abs(y); z = abs(z);
		return *view[7]->get(x-1,y-1,z-1);
	}
	else //row point
		assert(false);
}
#endif
static vector<vector3_t>* get_points_by_this_line(vector<vector3_t>* v, char* line)
{
	if(*line!='f')
		return nullptr;
	auto re = new vector<vector3_t>();
	while(true)
	{
		while(*line!='\n' && *line!=0 && *line!=' ')
			line++;
		if(*line==0 || *line=='\n')
		{
			if(re->size()==0)
			{
				delete re;
				return nullptr;
			}
			return re;
		}
		line++;
		assert(*line!=' ');
		char* p = strchr(line,'/');
		int len = p - line;
		char buf[len+1];
		buf[len]=0;
		memcpy(buf,line,len);
		int id = atoi(buf);
		assert(id>0);
		re->push_back(v->at(id-1));
	}
}
static vector3_t* get_fa_by_this_line(vector<vector3_t>* vn,char* line)
{
	while(*line!='\n' && *line!=0 && *line!=' ')
		line++;
	if(*line==0 || *line=='\n')
		assert(false);
	line = strchr(line,'/');
	line++;
	line = strchr(line,'/');
	line++;
	char* p = strchr(line,' ');
	int len = p-line;
	char buf[len+1];
	buf[len] = 0;
	memcpy(buf,line,len);
	int id = atoi(buf);
	assert(id>0);
	vector3_t* re =  new vector3_t(vn->at(id-1));
	return re;
}
char* next_line(char* p)
{
	assert(*p!=0);
	assert(*p!='\n');
	while(*p!=0 && *p!='\n')
		p++;
	if(*p=='\n')
		p++;
	return p;
}

static bool is_in_object(vector<vector3_t*>* fa_vec,
		vector<vector<vector3_t>*>* point_vec,int x,int y,int z)
{
	bool is_in = true;
	for(int i=0;i<fa_vec->size();i++)
	{
		vector3_t* fa = fa_vec->at(i);
		vector<vector3_t>* points = point_vec->at(i);
		vector3_t v = {.x= x - points->at(0).x,
			.y = y - points->at(0).y,
			.z = z - points->at(0).z };
		float result = v* (*fa);
		if(result<0)
		{
			printf("false");
			is_in = false;
			break;
		}
	}
	return is_in;
}
#if 0
void set_true(array3_t<bool*>* view[8],int x,int y, int z)
{
	if(x>0 && y >0 && z > 0)//1
	{
		*view[0]->get(x,y,z) = true;

	}
	else if(x<0 && y >0 && z > 0)//2
	{
		x = abs(x);y = abs(y); z = abs(z);
		*view[1]->get(x,y,z) = true;
	}
	else if(x<0 && y <0 && z > 0)//3
	{
		x = abs(x);y = abs(y); z = abs(z);
		*view[2]->get(x,y,z) = true;
	}
	else if(x>0 && y <0 && z > 0)//4
	{
		x = abs(x);y = abs(y); z = abs(z);
		*view[3]->get(x,y,z) = true;
	}
	else if(x>0 && y >0 && z < 0)//5
	{
		x = abs(x);y = abs(y); z = abs(z);
		*view[4]->get(x,y,z) = true;
	}
	else if(x<0 && y >0 && z < 0)//6
	{
		x = abs(x);y = abs(y); z = abs(z);
		*view[5]->get(x,y,z) = true;
	}
	else if(x<6 && y <0 && z < 0)//7
	{
		x = abs(x);y = abs(y); z = abs(z);
		*view[6]->get(x,y,z) = true;
	}
	else if(x>0 && y <0 && z < 0)//8
	{
		x = abs(x);y = abs(y); z = abs(z);
		*view[7]->get(x,y,z) = true;
	}
	else //row point
	{
	}
}
#endif
static vector<vector3_t>* load_obj_file_v(const char* obj_str)
{
	const char* p = obj_str;
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
	return vec;
}
static vector<vector3_t>* load_obj_file_vn(const char* obj_str)
{
	const char* p = obj_str;
	vector<vector3_t>* vec = new vector<vector3_t>();
	while(true)
	{
		if(*p=='v' && p[1]=='n')
		{
			p++;
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
	return vec;
}
char* load_file(const char* name)
{
	int obj_fd = open(name,O_RDONLY);
	assert(obj_fd>=0);
	size_t size = lseek(obj_fd,0,SEEK_END);
	lseek(obj_fd,0,SEEK_SET);
	char* buf = (char*)malloc(size+1);
	assert(read(obj_fd,buf,size)==size);
	close(obj_fd);
	buf[size] = 0;
	return buf;
}
static vector<vector3_t>* get_points_lines(vector<vector3_t>* v,char* obj_str)
{
	vector<vector3_t>* re = new vector<vector3_t>();
	assert(*obj_str=='f');
	while (*obj_str=='f')
	{
		obj_str+=2;
		char* end = strchr(obj_str,'/');
		int len = end-obj_str;
		char buf[len+1];
		buf[len] =0;
		memcpy(buf,obj_str,len);
		long id = atol(buf);
		assert(id>0);
		re->push_back( v->at(id-1));
		char* next = nullptr;
		if((next =strchr(obj_str,'\n'))!=nullptr)
		{
			obj_str = next;
			obj_str++;
		}
		else if((next=strchr(obj_str,0))!=nullptr)
		{
			return re;
		}
		else{assert(false);}
	}
	return re;


}
vector<vector3_t>* get_fa_lines(vector<vector3_t>* vn, char* obj_str)
{
	vector<vector3_t>* re = new vector<vector3_t>;
	assert(*obj_str=='f');
	while (*obj_str=='f')
	{
		obj_str =  strchr(obj_str,'/');
		obj_str++;
		obj_str =  strchr(obj_str,'/');
		obj_str++;
		char* end = strchr(obj_str,'/');
		int len = end-obj_str;
		char buf[len+1];
		buf[len] =0;
		memcpy(buf,obj_str,len);
		long id = atol(buf);
		assert(id>0);
		re->push_back( vn->at(id-1));
		char* next = nullptr;
		if((next =strchr(obj_str,'\n'))!=nullptr)
		{
			obj_str = next;
			obj_str++;
		}
		else if((next=strchr(obj_str,0))!=nullptr)
		{
			return re;
		}
		else{assert(false);}
	}
	return re;
}
bool is_in_object2(vector<vector3_t>* fa_lines,vector<vector3_t>* first_point_lines,int x,int y,int z)
{
	int times = first_point_lines->size();
	bool is_in = true;
	for(int i=0;i<times;i++)
	{
		int id = i+1;
		vector3_t fa = fa_lines->at(i);
		const vector3_t point = first_point_lines->at(i);
		vector3_t v = {.x=point.x -x,.y=point.y-y,.z=point.z-z};
		float a = v*fa;
		if(a<0)
		{
			is_in=false;
		break;
		}
	}
	return is_in;
}
void  set_true(bool_space_t* bool_space,int x,int y,int z)
{
	assert(x!=0 && y!=0 && z!=0);
	int a1 = x/abs(x),a2 = y/abs(y),a3 = z/abs(z);
	x = abs(x);y = abs(y);z = abs(z);
	bool_space->set(a1*(x),a2*y,a3*z,true);
	bool_space->set(a1*(x+1),a2*y,a3*z,true);
	bool_space->set(a1*x,a2*(y+1),a3*z,true);
	bool_space->set(a1*(x+1),a2*(y+1),a3*z,true);
	bool_space->set(a1*x,a2*y,a3*(z+1),true);
	bool_space->set(a1*(x+1),a2*y,a3*(z+1),true);
	bool_space->set(a1*x,a2*(y+1),a3*(z+1),true);
}
game_map_t::game_map_t(const char* obj_file_name)
{
	char*const  obj_str = load_file(obj_file_name);
	vector<vector3_t>* v =  load_obj_file_v(obj_str);
	vector<vector3_t>* vn =  load_obj_file_vn(obj_str);
	int max_x=0,max_y=0,max_z=0;
	for(auto it = v->begin();it!=v->end();++it)
	{
		if(abs(it->x)>max_x)
		{
			max_x = abs(it->x);
		}
		if(abs(it->y)>max_y)
		{
			max_y = abs(it->y);
		}
		if(abs(it->z)>max_z)
		{
			max_z = abs(it->z);
		}
	}
	//int m = max(max(max_x,max_y),max_z);
	//max_x = m;max_y = m; max_z = m;
	x_len = max_x;
	y_len = max_y;
	z_len  = max_z;
	bool_space = new bool_space_t(x_len,y_len,z_len);
	//--------------build true
	char* p =obj_str;
	while((p = strchr(p,'f'))!=nullptr)
	{
		if(p[1]=='f')
		{
			p+=2;
			continue;
		}
		vector<vector3_t>* first_point_lines = get_points_lines(v,p);
		vector<vector3_t>* fa_lines = get_fa_lines(vn,p);
		assert(first_point_lines->size()==fa_lines->size());
#if 0
		vector<vector3_t>* points=nullptr;
		vector3_t* fa = nullptr;
		vector<typeof(points)>* point_vec = new vector<typeof(points)>();
		auto fa_vec = new vector<typeof(fa)>();
		while((points = get_points_by_this_line(v,p))!=nullptr)
		{
			point_vec->push_back(points);
			fa = get_fa_by_this_line(vn,p);
			fa_vec->push_back(fa);
			p = next_line(p);
		}
		assert(fa_vec->size()==point_vec->size());
#endif
		this->print(1);
		//printf("-------------------------------------\n");
		for(int x=-(x_len-1);x<=x_len-1;x++)
		{
			if(x==0)
				continue;
			for(int y=-(y_len-1);y<=(y_len-1);y++)
			{
				if(y==0)
					continue;
				for(int z=-(z_len-1);z<=z_len-1;z++)
				{
					if(z==0)
						continue;
					if(is_in_object2(fa_lines,first_point_lines,x,y,z))
					{
						//printf("%d\n",x);
						set_true(bool_space,x,y,z);//8

					}
				}
			}
		}
#if 0
		int max_len = x_len-1;
		for(int x = -max_len;x<=max_len;x++)
			for(int y = -max_len;y<=max_len;y++)
				for(int z = -max_len;z<=max_len;z++)
				{
					if(is_in_object(fa_vec,point_vec,x,y,z))
					{
						set_true(view,x,y,z);//8
					}
				}
		for(auto it = point_vec->begin();it!=point_vec->end();++it)
		{
			delete (*it);
		}
		for(auto it = fa_vec->begin();it!=fa_vec->end();++it)
		{
			delete (*it);
		}

		delete point_vec;
		delete fa_vec;
#endif	
		delete first_point_lines;
		delete fa_lines;
		while(true)
		{
			if(p[0]=='\n')
			{
				if(p[1]!='f')
					break;
			}
		p++;
		}
	}
	free(obj_str);
	delete v;
	delete vn;
}
void game_map_t::print(int z)
{
	assert(z!=0 && abs(z)<=z_len);
	printf("(max_x = %d,max_y=%d,max_z=%d && z=%d)\n",x_len,y_len,z_len,z);
	for(int y = -(y_len);y<=y_len;y++)
	{
		if(y==0)
			continue;
		for(int x = -(x_len);x<=x_len;x++)
		{
			if(x==0)
				continue;
			printf("%d",bool_space->get(x,y,z));
		}
		printf("\n");
	}
}
game_map_t::~game_map_t()
{
	delete bool_space;
}
