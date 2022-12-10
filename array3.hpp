#ifndef ARRAY3_HPP
#define ARRAY3_HPP

#include "std.hpp"
//void free_3d(void* p);
inline void**** calloc_3d(size_t x,size_t y, size_t z)
{
	void** p = (typeof(p))calloc(1,sizeof(void*)*(x+x*y+x*y*z));
	void**** one = (typeof(one))p;
	p+=x;

	for(size_t i=0;i<x;i++)
	{
		one[i] = (typeof(one[i]))p;
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
template<typename any_p_t>
class array3_t
{
	void**** map;
	public:
	int x_len,y_len,z_len;
	array3_t(int x,int y,int z)
	{
		map = calloc_3d(x,y,z);
		x_len = x;y_len=y;z_len =z;
	}
	void set(int x, int y,int z,any_p_t anyp)
	{
		map[x][y][z] = anyp;
	}
	any_p_t get(int x,int y, int z)
	{
		return (any_p_t)map[x][y][z];
	}
	~array3_t()
	{
		free(map);
	}
};
#endif
