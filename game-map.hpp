
#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "std.hpp"
 
class vector3_t;
class bool_space_t;

class game_map_t
{
	bool_space_t* bool_space;
	//array3_t<bool*>* view[8];
	//void set_true(array3_t<bool*>* view[8],int x,int y, int z);
	public:
	int x_len,y_len,z_len;
	game_map_t(const char* obj_file_name);
	bool has_cube(int x, int y,int z);
	void print(int z);
	~game_map_t();
};
#endif
