/**
 * Created by S. Gothard 07/06/2012 
 * For Fall 2012 CPS 350 Data Structures and Algorithms project 3
 * Handles the OpenGL GUI for the maze.
 */
#include <iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include <ctime>
#include <string>
using std::string;

// The following two lines can be deleted if this header directly or indirectly
//    includes maze_cell.h.
#include "Angel.h"

#include <vector>
#include"maze.h"
typedef Angel::vec3 color3;

using namespace std;
class GUI_maze
{
   public:
      GUI_maze (int = 20, int = 20);
      ~GUI_maze ();

      void setup_rendering ();
      bool maze_solved ();
      void create_new_maze ();
      bool is_animating () const;
      unsigned int pts_count () const;
      const color3 *get_GPU_colors ();
	  

      void enter_build_mode ();
      void instant_build ();
      void do_next_step ();
      void solve_maze();
      void enter_solve_mode ();
      void enter_demo_mode ();
      void stop_animating ();
      bool connect_one ();

      void move_north ();
      void move_south ();
      void move_east ();
      void move_west ();


      int pixel_height () const;
      int pixel_width () const;
      void save_to_file () const;

	  

      static const int PIXEL_WDTH = 512, PIXEL_HT = 768;
      static const int VERTICES_PER_CELL = 6;

   private:

      // array of vertices
      vec3 *points;
	  
	  Maze newMaze;
      bool in_demo_mode, in_build_mode, in_solve_mode;
      int num_points, rooms_down, rooms_across, pixel_ht, pixel_wdth;

      void set_screen_dimensions ();
      void place_vertices ();
      void load_GPU_arrays ();
      void setup_shaders_and_vars ();
};
