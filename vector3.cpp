#include "all.hpp"
float vector3_t::operator*(vector3_t& o)
{
	return x*o.x+y*o.y+z*o.z;
}
