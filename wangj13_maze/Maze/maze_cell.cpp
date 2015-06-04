/**
 * Created by S. Gothard 07/18/2012 
 * For Fall 2012 CPS 350 Data Structures and Algorithms project 3
 * A maze_cell object represents a cell for use in a 2-dimentional maze.
 */
#include "maze_cell.h"
#include <iostream>
using namespace std;
/**
 * Sets the constant colors to be used with each cell type. Feel free to 
 *    set your own colors.
 */
const color3 maze_cell::colors [maze_cell::NUM_TYPES] = 
{  
   color3 (1.0, 1.0, 1.0), color3 (0.1f, 0.1f, 0.0), color3 (0.5, 0.5, 0.6f), 
   color3 (.9f, .7f, .7f), color3 (0.0f, 0.0f, 1.0), color3 (1.0, 0.0, 0.0) 
};

/**
   * Creates an empty cell for use in an n by m rectangular maze. 
   */
maze_cell::maze_cell(int r, int c) : type (EMPTY), row(r), column (c) { }

/**
 * Sets the row and column of this cell.
 */
void maze_cell::set_location (int r, int c)
{
   row = r;
   column = c;
}

/**
 * Changes the cell type to be the head of the trail.
 */
void maze_cell::make_head ()
{
   type = TRAIL_HEAD;
}

/**
 * Changes the cell type to be the exit.
 */
void maze_cell::make_exit ()
{
   type = EXIT;
}

/**
 * Changes the cell type to be a wall.
 */
void maze_cell::make_wall ()
{
   type = WALL;
}


void maze_cell::make_empty ()
{
   type = EMPTY;
}

/**
 * Changes the cell type to be part of the current trail attempt.
 */
void maze_cell::add_to_trail ()
{
   type = TRAIL;
}

/**
 * Changes the cell to no longer be part of the current trail, but instead be
 *    marked as part of a previous attempt.
 */
void maze_cell::remove_from_trail ()
{
   type = OLD_TRAIL;
}

/** 
 * Changes the cell to just be an empty room.
 */
void maze_cell::clear_cell ()
{
   type = EMPTY;
}

/**
 * Returns the appropriate display color for this maze_cell. No bounds 
 *    checking is performed.
 */
const color3 &maze_cell::get_color() const
{

   return colors[type];
}

/**
 * Returns this maze cell's row position in the overall maze.
 */
int maze_cell::get_row() const
{
   return row;
}
   
/**
 * Returns this maze cell's column position in the overall maze.
 */
int maze_cell::get_column() const
{
   return column;
}

/**
 * Returns whether this cell is part of an old trail.
 */
bool maze_cell::is_old_trail () const
{
   return type == OLD_TRAIL; 
}

/**
 * Returns whether this cell is a wall.
 */
bool maze_cell::is_wall () const
{
   return WALL == type;
}

/**
 * Returns whether this cell is the exit.
 */
bool maze_cell::is_exit () const
{
   return EXIT == type;
}

/**
 * Returns whether this cell is a room that has not been visited.
 */
bool maze_cell::is_unvisited_room () const
{
   return EMPTY == type || EXIT == type;
}

/**
 * Returns whether this cell is part of the current path being tried.
 */
bool maze_cell::in_current_path () const
{
   return TRAIL == type;
}