#ifndef VECTOR3_HPP
#define VECTOR3_HPP
#include <vector>
using namespace std;
struct vector3_t
{
	float x,y,z;
	//	static vector<vector3_t>* load_obj_file_v(const char* file_name);
	//	static vector<vector3_t>* load_obj_file_vn(const char* file_name);
		float operator*(vector3_t& o);
};
#endif
