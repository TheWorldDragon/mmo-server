#include "all.hpp"
int main()
{
	game_map_t g = game_map_t("test.obj");
	for(int i=-5;i<6;i++)
	{
		if(i==0)
			continue;
	g.print(i);
	}
	int a=1;
}
