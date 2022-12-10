#include "all.hpp"
int main()
{
#if 0
	game_map_t g = game_map_t("test.obj");
	int a=1;
	for(int x=-(g.x_len);x<=g.x_len;x++)
	{
		for(int y=-(g.y_len);y<=g.y_len;y++)
		{
			if(x==0)
				continue;
			if(y==0)
				continue;
			printf("%d ",g.has_cube(y,x,3));
		}
		printf("\n");
	}
#endif
}
