


#include "all.hpp"
#include <fcntl.h>
static void build_true(vector<vector3_t>* v,vector<vector3_t>* vn,const char* obj_file_name,array3_t<bool*>* view)
{


}
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
game_map_t::game_map_t(const char* obj_file_name)
{
	vector<vector3_t>* v =  vector3_t::load_obj_file_v(obj_file_name);
	vector<vector3_t>* vn =  vector3_t::load_obj_file_vn(obj_file_name);
	int max_x=0,max_y=0,max_z=0;
	for(auto it = v->begin();it!=v->end();++it)
	{
		if(it->x>max_x)
		{
			max_x = it->x;
		}
		if(it->y>max_y)
		{
			max_y = it->y;
		}
		if(it->z>max_z)
		{
			max_z = it->z;
		}
	}
	int m = max(max(max_x,max_y),max_z);
	max_x = m;max_y = m; max_z = m;
	x_len = max_x;
	y_len = max_y;
	z_len  = max_z;
	for(int i=0;i<8;i++)
	{
			usleep(1000);
		view[i] = new array3_t<bool*>(max_x,max_y,max_z);
		for(int x=0;x<x_len;x++)
			for(int y=0;y<y_len;y++)
				for(int z=0;z<z_len;z++)
				{
					view[i]->set(x,y,z,new bool(false));
				}
	}
	//--------------build true
	int obj_fd = open(obj_file_name,O_RDONLY);
	assert(obj_fd>=0);
	size_t size = lseek(obj_fd,0,SEEK_END);
	lseek(obj_fd,0,SEEK_SET);
	char* buf = (char*)malloc(size+1);
	assert(read(obj_fd,buf,size)==size);
	close(obj_fd);
	buf[size] = 0;
	char* p =buf;
	while((p = strchr(p,'f'))!=nullptr)
	{
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
	}

	free(buf);

	//-----------------------------

	delete v;
	delete vn;
}
game_map_t::~game_map_t()
{
	for(int i=0;i<8;i++)
	{
		for(int x=0;x<x_len;x++)
			for(int y=0;y<y_len;y++)
				for(int z=0;z<z_len;z++)
				{
					delete view[i]->get(x,y,z);
				}
		delete view[i];
	}
}
