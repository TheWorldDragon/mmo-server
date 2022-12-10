#ifndef BOOL_SPACE_HPP
#define BOOL_SPACE_HPP
class bool_space_t
{
	bool***bool_map[8];
	int max_x,max_y,max_z;
	public:
		bool_space_t(int max_x,int max_y,int max_z);
		~bool_space_t();
		bool get(int x,int y,int z);
		void set(int x,int y,int z,bool v);
};
#endif 
