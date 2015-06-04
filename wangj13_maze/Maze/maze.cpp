#include"maze.h"

//constructor
Maze::Maze()
{	
}

//destructor
Maze::~Maze()
{
	maze_data.clear();
	delete header;
	header = NULL;
	delete []colorarray;
	colorarray = NULL;
	root.clear();
}
	
//constructor
Maze::Maze(int row, int column)
{
	int i,j;
   int size = (2 * row + 1) * (2 * column + 1);
   room_row = row;
   room_across = column;

   maze_set.D_set1(size);
  
   maze_data.resize(2 * row + 1);
   
   for(i = 0; i < 2 * row + 1; i++)
   {
	   maze_data[i].resize(2 * column + 1);
   }

   for(i = 0;  i < 2 * row +1; i++)
   {
	   for(j = 0; j < 2 * column+1; j++)
	   {
		   maze_data[i][j].set_location(i,j);
		   if(i % 2 ==0 || j % 2 == 0)
		   {
				maze_data[i][j].make_wall();

				maze_set.setwall(flatten(i,j));
		   }
  
		   else
		   {
			   root.push_back(flatten(i,j));
		   }
		
	   }
   }  

   maze_data[1][0].make_head();
   maze_data[2*row-1 ][2 *column].make_exit();
   header = &maze_data[1][0];
   
   colorarray = new color3[size*6];
   getcolorarray(colorarray);
}


void Maze::Maze1(int row, int column)
{
	int i,j;
	room_row = row;
   room_across = column;
   root.clear();
   int size = (2 * row + 1) * (2 * column + 1);

   maze_set.D_set1(size);
   
   maze_data.resize(2 * row + 1);
   
   for(i = 0; i < 2 * row + 1; i++)
   {
	   maze_data[i].resize(2 * column + 1);
   }
   
   for(i = 0;  i < 2 * row +1; i++)
   {
	   for(j = 0; j < 2 * column+1; j++)
	   {
		   maze_data[i][j].set_location(i,j);
		   if(i % 2 ==0 || j % 2 == 0)
		   {
				maze_data[i][j].make_wall();
				
				maze_set.setwall(flatten(i,j));

		   }
  
		   else
		   {
			   maze_data[i][j].clear_cell();
			   root.push_back(flatten(i,j));
		   }

	   }
   }  

   
   maze_data[1][0].make_head();
   maze_data[2*row-1 ][2 *column].make_exit();

   header = &maze_data[1][0];
   
   colorarray = new color3[size*6];
   getcolorarray(colorarray);
}

 //get a 1D value
int Maze::flatten(int row, int column)
{
	return (row * (2 *room_across +1) + column);
}

//return a color3 array
void Maze::getcolorarray(color3 colorarray[])
{
	int size = (2 *room_across +1) * (2 *room_row +1) * 6;

	for(int i = 0; i < size/6; i ++)
	{
		color3 temp = maze_data[i/(2 * room_across + 1)][i%(2 * room_across + 1)].get_color();
		colorarray[i * 6] = temp;
		colorarray[i* 6+1] = temp;
		colorarray[i* 6+2] = temp;
		colorarray[i* 6+3] = temp;
		colorarray[i* 6+4] = temp;
		colorarray[i* 6+5] = temp;
	}

}

//join 2 rooms
bool Maze::join2rooms()
{	
	int num =maze_set.getsize();
	int root1,root2,root3;
	int randomnum = rand()% num;

	int cols = 2 *room_across + 1;
	int rows = 2 *room_row + 1;
	int random = root[randomnum];
	int move = rand()% 4;
	int j = 0;
	
	int res = 0;
	
	for(int i = 0 ;i < cols * rows; i++ )
	{
		if(maze_set[i] <0)
		{
			res ++;
		}
		
	}

	if(res == 1)
	{
		return false;
	}
	else
	{
	
	switch(move)
	{
		//left
	case 0:
		{
			if(random % cols - 2 < 0 )
			{
				return true;
			}
			else
			{ 
				if(maze_data[(random-2)/cols][(random-2)%cols].is_unvisited_room())
				{
					
					root1 = maze_set.find(random);
					root2 = maze_set.find(random - 2);
					if(root1 != root2)
					{
						if(maze_data[(random-1)/cols][(random-1)%cols].is_wall())
						{
							maze_set.setempty(random -1);
							root.push_back(random -1);
						}
						root3 = maze_set.find(random - 1);
						maze_set.unionSets(root1, root3);
						maze_data[(random-1)/cols][(random-1)%cols].make_empty();
						root3 = maze_set.find(root3);
						maze_set.unionSets(root2, root3);
						
						return true;
					}
				}
			return true;
				
			}
		}
		//right
	case 1:
		{
			if(random % cols != (random +  2) % cols )
				return true;
			else
			{
				if(maze_data[(random+2)/cols][(random+2)%cols].is_unvisited_room())
				{
					
					root1 = maze_set.find(random);
					root2 = maze_set.find(random + 2);
					if(root1 != root2)
					{
						if(maze_data[(random+1)/cols][(random+1)%cols].is_wall())
						{
							maze_set.setempty(random +1);
							root.push_back(random +1);
						}
						root3 = maze_set.find(random + 1);
						maze_set.unionSets(root1, root3);
						maze_data[(random+1)/cols][(random+1)%cols].make_empty();
						root3 = maze_set.find(root3);
						maze_set.unionSets(root2, root3);
						
						return true;
					}
				}
				return true;
			
			}
		}
		//up
	case 2:
		{
			if(random - 2 *cols < 0 )
				return true;
			else
			{
				if(maze_data[(random - (2 *cols))/cols][(random - (2 *cols))%cols].is_unvisited_room())
				{
					
					root1 = maze_set.find(random);
					root2 = maze_set.find(random  - (2 *cols));
					if(root1 != root2)
					{
						if(maze_data[(random - (cols))/cols][(random - (cols))%cols].is_wall())
						{
							maze_set.setempty(random  - (cols));
							root.push_back(random  - (cols));
						}
						root3 = maze_set.find(random - (cols));
						maze_set.unionSets(root1, root3);
						maze_data[(random - (cols))/cols][(random - (cols))%cols].make_empty();
						root3 = maze_set.find(root3);
						maze_set.unionSets(root2, root3);
						
						return true;
					}

				}
				return true;
			
			}
		}
		//down
	case 3:
			{
			if(random + 2 *cols  > maze_set.getsize() - 1)
				return true;
			else
			{
				if(maze_data[(random + 2 *cols)/cols][(random + 2 *cols)%cols].is_unvisited_room())
				{
					
					root1 = maze_set.find(random);
					root2 = maze_set.find(random+(2 * cols));
					if(root1 != root2)
					{
						if(maze_data[(random + cols)/cols][(random + cols)%cols].is_wall())
						{
							maze_set.setempty(random  + (cols));
							root.push_back(random  + (cols));
						}
						root3 = maze_set.find(random + (cols));
						maze_set.unionSets(root1, root3);
						maze_data[(random + cols)/cols][(random + cols)%cols].make_empty();
						root3 = maze_set.find(root3);
						maze_set.unionSets(root2, root3);
						
	
						return true;
					}
				}
				return true;
	
			}
		}
	}
	}
}

//terminal condition for join 2 rooms
void Maze::keepcalling()
{
	while(true)
	{
		int res = 0;
		for(int i = 0 ;i < root.size(); i++ )
		{
			for( int j = i+1; j < root.size(); j++ )
			{   int num1 = root[j];
				if(maze_set.find(root[i]) != maze_set.find(root[j]))
				{
					res ++;
				}
			}
		}

		if(res == 0)
		{
			break;
		}
		else
		{
			join2rooms();
		}
	}
}

//move to north
void Maze::move_north()
{
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	if(maze_data[row_temp - 1][column_temp].is_wall())
	{
		return;
	}

	else 
	{
		header = &maze_data[row_temp - 1][column_temp];
		if(maze_data[row_temp - 1][column_temp].is_unvisited_room())
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp - 1][column_temp].make_head();
			
		}
		else if(maze_data[row_temp - 1][column_temp].in_current_path())
		{
			maze_data[row_temp][column_temp].remove_from_trail();
			maze_data[row_temp - 1][column_temp].make_head();
		}
	}
}

//move to south
void Maze::move_south()
{
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	if(maze_data[row_temp + 1][column_temp].is_wall() || maze_data[row_temp + 1][column_temp].is_old_trail())
	{
		return;
	}
	else
	{
		header = &maze_data[row_temp + 1][column_temp];
		if(maze_data[row_temp + 1][column_temp].is_unvisited_room())
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp + 1][column_temp].make_head();
		}
		else if(maze_data[row_temp + 1][column_temp].in_current_path())
		{
			maze_data[row_temp][column_temp].remove_from_trail();
			maze_data[row_temp + 1][column_temp].make_head();
		}
	}
}

//move to west
void Maze::move_west()
{
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	if(maze_data[row_temp ][column_temp - 1].is_wall() || maze_data[row_temp ][column_temp - 1].is_old_trail())
	{
		return;
	}
	else
	{
		header = &maze_data[row_temp][column_temp - 1];
		if(maze_data[row_temp][column_temp - 1].is_unvisited_room())
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp ][column_temp- 1].make_head();
		}
		else if(maze_data[row_temp ][column_temp- 1].in_current_path())
		{
			maze_data[row_temp][column_temp].remove_from_trail();
			maze_data[row_temp][column_temp - 1].make_head();
		}
	}
}

//move to east
void Maze::move_east()
{
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	if(maze_data[row_temp ][column_temp + 1].is_wall() || maze_data[row_temp ][column_temp + 1].is_old_trail())
	{
		return;
	}
	else
	{
		header = &maze_data[row_temp][column_temp + 1];
		
		if(maze_data[row_temp][column_temp + 1].is_unvisited_room())
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp ][column_temp + 1].make_head();
		}
		else if(maze_data[row_temp ][column_temp + 1].in_current_path())
		{
			maze_data[row_temp][column_temp].remove_from_trail();
			maze_data[row_temp][column_temp + 1].make_head();
		}
	}
}

//move to north
void Maze::move_north_step()
{
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	if(maze_data[row_temp - 1][column_temp].is_wall()||maze_data[row_temp - 1][column_temp].is_exit() )
	{
		return;
	}

	else 
	{
		header = &maze_data[row_temp - 1][column_temp];
		if(maze_data[row_temp - 1][column_temp].is_unvisited_room())
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp - 1][column_temp].make_head();
			
		}
		else if(maze_data[row_temp - 1][column_temp].in_current_path())
		{
			maze_data[row_temp][column_temp].remove_from_trail();
			maze_data[row_temp - 1][column_temp].make_head();
		}
		else if(maze_data[row_temp - 1][column_temp].is_old_trail())
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp -1][column_temp].make_head();
		}
	}
}

//move to south
void Maze::move_south_step()
{
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	if(maze_data[row_temp + 1][column_temp].is_wall()||maze_data[row_temp - 1][column_temp].is_exit())
	{
		return;
	}
	else
	{
		header = &maze_data[row_temp + 1][column_temp];
		if(maze_data[row_temp + 1][column_temp].is_unvisited_room())
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp + 1][column_temp].make_head();
		}
		else if(maze_data[row_temp + 1][column_temp].in_current_path())
		{
			maze_data[row_temp][column_temp].remove_from_trail();
			maze_data[row_temp + 1][column_temp].make_head();
		}
		else
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp + 1][column_temp].make_head();
		}
	}
}

//move to west
void Maze::move_west_step()
{
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	if(maze_data[row_temp ][column_temp - 1].is_wall()||maze_data[row_temp - 1][column_temp].is_exit())
	{
		return;
	}
	else
	{
		header = &maze_data[row_temp][column_temp - 1];
		if(maze_data[row_temp][column_temp - 1].is_unvisited_room())
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp ][column_temp- 1].make_head();
		}
		else if(maze_data[row_temp ][column_temp- 1].in_current_path())
		{
			maze_data[row_temp][column_temp].remove_from_trail();
			maze_data[row_temp][column_temp - 1].make_head();
		}
		else
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp][column_temp - 1].make_head();
		}
	}
}

//move to east
void Maze::move_east_step()
{
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	if(maze_data[row_temp ][column_temp + 1].is_wall()||maze_data[row_temp - 1][column_temp].is_exit())
	{
		return;
	}
	else
	{
		header = &maze_data[row_temp][column_temp + 1];
		
		if(maze_data[row_temp][column_temp + 1].is_unvisited_room())
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp ][column_temp + 1].make_head();
		}
		else if(maze_data[row_temp ][column_temp + 1].in_current_path())
		{
			maze_data[row_temp][column_temp].remove_from_trail();
			maze_data[row_temp][column_temp + 1].make_head();
		}
		else
		{
			maze_data[row_temp][column_temp].add_to_trail();
			maze_data[row_temp][column_temp + 1].make_head();
		}
	}
}

//check ending
bool Maze::is_end()
{
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	if((row_temp == 2 * room_row-1) && column_temp == 2 *room_across)
		return true;
	else
		return false;
}

//auto find the path
void Maze::autofind()
{
//	srand ( time(NULL) );
	int row_temp = header->get_row();
	int column_temp = header->get_column();
	
	string s,s1;
	if(maze_data[row_temp][column_temp + 1].is_unvisited_room())
	{
		s.insert(s.end(),'E');
	}
	if(column_temp > 0 && maze_data[row_temp ][column_temp - 1].is_unvisited_room())
	{
		s.insert(s.end(),'W');
	}
	if(maze_data[row_temp  - 1][column_temp].is_unvisited_room())
	{
		s.insert(s.end(),'N');
	}
	if(maze_data[row_temp + 1][column_temp ].is_unvisited_room())
	{
		s.insert(s.end(),'S');
	}
	int size = s.size();
	if(size != 0)
	{
		int res = rand () % (size);
		
		switch(s[res])
		{
		case 'E':
			{
				move_east();
				break;
			}
		case 'W':
			{
				move_west();
				break;
			}
		case 'N':
			{
				move_north();
				break;
			}
		case 'S':
			{
				move_south();
				break;
			}
		}
		s.clear();
	}
	else
	{
		if(maze_data[row_temp ][column_temp + 1].in_current_path())
		{
			s1.insert(s1.end(),'E');
		}
		if(maze_data[row_temp ][column_temp - 1].in_current_path())
		{
			s1.insert(s1.end(),'W');
		}
		if(maze_data[row_temp  - 1][column_temp].in_current_path())
		{
			s1.insert(s1.end(),'N');
		}
		if(maze_data[row_temp + 1][column_temp ].in_current_path())
		{
			s1.insert(s1.end(),'S');
		}
		int res = rand()% (s1.size());
		switch(s1[res])
		{
		case 'E':
			{
				move_east();
				break;
			}
		case 'W':
			{
				move_west();
				break;
			}
		case 'N':
			{
				move_north();
				break;
			}
		case 'S':
			{
				move_south();
				break;
			}
		}
	}
	
}

//clean the old_trail
void Maze::clean()
{
	for(int i = 0; i < 2 * room_row + 1;i++)
	{
		 for(int j = 0; j < 2 * room_across+ 1; j++)
		 {
			 if(maze_data[i][j].is_old_trail())
			 {
				maze_data[i][j].clear_cell();
			 }
		 }
	}
	cout <<"finished";
}