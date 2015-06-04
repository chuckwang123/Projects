/**
 * Created by S. Gothard 07/18/2012 
 * For Fall 2012 CPS 350 Data Structures and Algorithms project 3
 * A maze_cell object represents a cell for use in a 2-dimentional maze.
 *    Each maze_cell may be traversable (e.g. EMPTY), or non-traversable 
 *    (e.g. WALL), may be part of the current path (TRAIL), may be previously 
 *    visited (OLD_TRAIL), may be the front of the path (TRAIL_HEAD), or
 *    may be the exit (EXIT).
 */

#include <iostream>
using std::cerr;
using std::endl;

#include "Angel.h"
typedef Angel::vec3 color3;

class maze_cell
{
   private:
      enum cell_type { EMPTY, WALL, TRAIL, OLD_TRAIL, TRAIL_HEAD, EXIT };
      static const int NUM_TYPES = 6;
      static const color3 colors [NUM_TYPES];

      cell_type type; 
      int row, column;

   public:
    
      maze_cell (int = -1, int = -1);

      void set_location (int, int);

      void make_head ();
      void make_exit ();
      void make_wall ();
	  void make_empty();
      void add_to_trail ();
      void remove_from_trail ();
      void clear_cell ();

      const color3 &get_color () const;
      int get_row () const;
      int get_column () const;
      
      bool is_old_trail () const;
      bool is_wall () const;
      bool is_exit () const;
      bool is_unvisited_room () const;
      bool in_current_path () const;
};
