#include "disjoint set.h"

sets::sets()
{
	maze_data.resize(2*rooms_down + 1);
	for(int i = 0;i < 2*rooms_down +1; i++)
	{
		maze_data.resize(2*rooms_across +1);
	}
}