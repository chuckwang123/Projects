#include"d_set.h"

//constructor
D_set::D_set()
{
	size =0;
}

D_set::D_set(int numElements):maze_data(numElements)
{
	for(int j = 0;j < numElements; j++)
	{
		maze_data[j] = -1;
	}
	size = maze_data.size();
}

//reconstructor
void D_set::D_set1(int &numElements)
{

	maze_data.resize(numElements);
	for(int j = 0;j < numElements; j++)
	{
		maze_data[j] = -1;
	}
	
	size = maze_data.size();
}

//find the root
int D_set::find(int x)
{
	if(maze_data[x] < 0)
	{
		return x;
	}
	else
	{
		return maze_data[x] = find(maze_data[x]);
	}
}

//union
void D_set::unionSets(int root1, int root2)
{
	if(maze_data[root2] < maze_data[root1])
	{
		maze_data[root1] = root2;
	}
	else
	{
		if(maze_data[root2] == maze_data[root1])
		{
			maze_data[root1]--;
		}
		maze_data[root2] = root1;
		
	}
}

//set value is wall and descrease size
void D_set::setwall(int x)
{
	maze_data[x] = INT_MAX;
	size--;
}

//set value is empty and increase size
void D_set::setempty(int x)
{
	maze_data[x] = -1;
	size++;
}

//return the size
int D_set::getsize()
{
	return size;
}

// display the whole disjoint set
void D_set::display()
{
	for(int i = 0; i < maze_data.size(); i++)
	{
		
		cout << maze_data[i] <<"   ";
	}
	cout <<endl;
}

//overload operator []
int& D_set::operator[] (const int x)
{
    return maze_data[x];
}