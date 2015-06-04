#include <vector>
#include"d_set.h"
#include <stdlib.h>
#include <time.h>
using namespace std;

class Maze
{
public:
	Maze();
	Maze(int row, int column);
	void Maze1(int row, int column);
	~Maze();
	int flatten(int row, int column);
	void getcolorarray(color3[]);
	bool join2rooms();
	void keepcalling();
	void move_north();
	void move_south();
	void move_east();
	void move_west();
	void move_north_step();
	void move_south_step();
	void move_east_step();
	void move_west_step();
	bool is_end();
	void autofind();
	void clean();

private:
	int room_row;
	int room_across;
	vector<vector<maze_cell> > maze_data;
	D_set maze_set;
	vector <int> root;
	
	maze_cell * header;
    color3* colorarray;
};