#include "all.hpp"
static bool*** allocate_3d(int max_x,int max_y,int max_z)
{
	bool*** bool_map1 = new bool**[max_x];
	for(int i=0;i<max_x;i++)
	{
		bool_map1[i] = new bool*[max_y];
		for(int j=0;j<max_y;j++)
		{
			bool* a =  new bool[max_z];
			for(int k=0;k<max_z;k++)
				a[k] =false;
			bool_map1[i][j] =a ;

		}
	}
	return bool_map1;
}
static void free_3d(bool*** bool_map1,int max_x,int max_y,int max_z)
{
	for(int i=0;i<max_x;i++)
	{
		for(int j=0;j<max_y;j++)
		{
			delete[] bool_map1[i][j];
		}
		delete[] bool_map1[i];
	}
	delete[] bool_map1;
}
bool_space_t::bool_space_t(int max_x,int max_y,int max_z)
{
	for(int i=0;i<8;i++)
	{
		bool_map[i] = allocate_3d(max_x,max_y,max_z);
	}
	this->max_x = max_x;this->max_y = max_y;this->max_z = max_z;
}
bool_space_t::~bool_space_t()
{
	for(int i=0;i<8;i++)
	{
		free_3d(bool_map[i],max_x,max_y,max_z);
	}

}
bool bool_space_t::get(int x, int y, int z)
{
	assert(x!=0 && y!=0 && z!=0);
	if(x>0 && y >0 && z > 0)//1
	{
		x--;y--;z--;
		return bool_map[0][x][y][z];
	}
	else if(x<0 && y >0 && z > 0)//2
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		return bool_map[1][x][y][z];
	}
	else if(x<0 && y <0 && z > 0)//3
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		return bool_map[2][x][y][z];
	}
	else if(x>0 && y <0 && z > 0)//4
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		return bool_map[3][x][y][z];
	}
	else if(x>0 && y >0 && z < 0)//5
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		return bool_map[4][x][y][z];
	}
	else if(x<0 && y >0 && z < 0)//6
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		return bool_map[5][x][y][z];
	}
	else if(x<0 && y <0 && z < 0)//7
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		return bool_map[6][x][y][z];
	}
	else if(x>0 && y <0 && z < 0)//8
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		return bool_map[7][x][y][z];
	}
	else
		assert(false);
}
void bool_space_t::set(int x, int y, int z, bool v)
{
	assert(x!=0 && y!=0 && z!=0);
	if(x>0 && y >0 && z > 0)//1
	{
		x--;y--;z--;
		bool_map[0][x][y][z] = v;
	}
	else if(x<0 && y >0 && z > 0)//2
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		bool_map[1][x][y][z] = v;
	}
	else if(x<0 && y <0 && z > 0)//3
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		bool_map[2][x][y][z] = v;
	}
	else if(x>0 && y <0 && z > 0)//4
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		bool_map[3][x][y][z] = v;
	}
	else if(x>0 && y >0 && z < 0)//5
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		bool_map[4][x][y][z] = v;
	}
	else if(x<0 && y >0 && z < 0)//6
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		bool_map[5][x][y][z] = v;
	}
	else if(x<0 && y <0 && z < 0)//7
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		bool_map[6][x][y][z] = v;
	}
	else if(x>0 && y <0 && z < 0)//8
	{
		x = abs(x);y = abs(y); z = abs(z);
		x--;y--;z--;
		bool_map[7][x][y][z] = v;
	}
	else
		assert(false);

}
