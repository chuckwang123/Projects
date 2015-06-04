/**
 * Created by S. Gothard 07/06/2012 
 * For Fall 2012 CPS 350 Data Structures and Algorithms project 3
 * Handles the OpenGL GUI for the maze.
 */
#include "GUI_maze.h"

#include <vector>
using namespace std;

/**
 * Creates the maze, sets the modes, and sets the random number generators.
 */
GUI_maze::GUI_maze (int rows, int cols) : rooms_down (rows), rooms_across (cols),
          pixel_ht(PIXEL_WDTH), pixel_wdth(PIXEL_HT)
{
   srand (static_cast<unsigned int>(time(0)));
   create_new_maze ();
   stop_animating ();
   set_screen_dimensions ();
}

/**
 * Deallocates the points array.
 */
GUI_maze::~GUI_maze()
{
   if (points)
   {
      delete [] points;
      points = NULL;
   }
}

/**
 * Places the vertices, puts the vertices into GPU memory, sets up the shaders
 *    and sets the clear color.
 */
void GUI_maze::setup_rendering ()
{
   place_vertices ();
   load_GPU_arrays();
   setup_shaders_and_vars();

   // white background
   glClearColor (1.0, 1.0, 1.0, 1.0);   
}

/**
 * Returns whether the maze was solved.
 */
bool GUI_maze::maze_solved ()
{
   /****************************** FILL IN CODE ******************************/
	return (newMaze.is_end());
}

/**
 * Creates a new maze without actually connecting the rooms.
 *    Sets the appropriate modes.
 */
void GUI_maze::create_new_maze ()
{
   /************************** construct a new maze **************************/
	//newMaze.clean();
	//newMaze = Maze(rooms_down, rooms_across);
	newMaze.Maze1(rooms_down, rooms_across);
   in_solve_mode = false;
   in_build_mode = in_demo_mode;
}

/**
 * Returns whether the maze is in build mode or solve mode or demo mode.
 */
bool GUI_maze::is_animating () const
{
   return in_build_mode || in_solve_mode || in_demo_mode;
}

/**
 * Builds the maze and turns off demo, build, or solve mode.
 */
void GUI_maze::instant_build ()
{
   stop_animating ();
   /************************* connect all the rooms *************************/
   newMaze.keepcalling();
}

/**
 * Performs the next animating step.
 */
void GUI_maze::do_next_step ()
{
   if (in_build_mode && !maze_solved ())
   {
      /************* join two rooms not already in the same set *************/
	   in_build_mode = newMaze.join2rooms();
   }
   else if (in_solve_mode && !maze_solved())
   {
	   newMaze.autofind();
      /************** move to one unvisited room (or backtrack) **************/
   }
   else if (!maze_solved()) 
   {
      in_build_mode = false;
      in_solve_mode = true;
   }
}

/**
 * Returns the number of vertices describing the maze.
 */
unsigned int GUI_maze::pts_count () const
{
   return num_points;
}


/**
 * Returns a constant pointer to an array of colors corresponding to the 
 *    vertices.
 */
const color3 *GUI_maze::get_GPU_colors ()
{
   /******* return an array of color3 variables to use with rendering *******/
	color3* color_array = new color3 [(2 * rooms_down + 1) * (2 * rooms_across + 1)*6];
	
	newMaze.getcolorarray(color_array );

   return color_array;
}
 
/**
 * Sets the booleans for build mode.
 */
void GUI_maze::enter_build_mode ()
{
   in_build_mode = true;
   in_demo_mode = in_solve_mode = false;
}

/**
 * Solves the maze.
 */
void GUI_maze::solve_maze ()
{
   /*************** map the correct trail and no other trails ***************/
	newMaze.clean();

}
/**
 * Sets the booleans for solve mode.
 */
void GUI_maze::enter_solve_mode ()
{
   in_solve_mode = true;
   in_build_mode = in_demo_mode = false;
}

/**
 * Sets the booleans for demo mode.
 */
void GUI_maze::enter_demo_mode ()
{
   in_demo_mode = true;
   in_build_mode = in_solve_mode = false;
}

/**
 * Cancels any animations.
 */
void GUI_maze::stop_animating ()
{
   in_demo_mode = in_solve_mode = in_build_mode = false;
}

/**
 * Performs a single connection of the maze and clears any animating modes.
 */
bool GUI_maze::connect_one ()
{
	bool result = newMaze.join2rooms();
	
   stop_animating ();
   /*************** join two rooms not already in the same set ***************/
   /******************** Return whether the join occured *********************/
	//return false; 
	return result;
}

/**
 * Moves the trail head up only if the move is valid.
 */
void GUI_maze::move_north ()
{
   /****************************** FILL IN CODE ******************************/
	newMaze.move_north_step();
}

/**
 * Moves the trail head down only if the move is valid.
 */
void GUI_maze::move_south ()
{
   /****************************** FILL IN CODE ******************************/
	newMaze.move_south_step();
}

/**
 * Moves the trail head right only if the move is valid.
 */
void GUI_maze::move_east ()
{
   /****************************** FILL IN CODE ******************************/
	newMaze.move_east_step();
}

/**
 * Moves the trail head left only if the move is valid.
 */
void GUI_maze::move_west ()
{
   /****************************** FILL IN CODE ******************************/
	newMaze.move_west_step();
}

/**
 * Returns the number of pixels tall that the image is.
 */
int GUI_maze::pixel_height () const
{
   return pixel_ht;
}

/** 
 * Returns the number of pixels width that the image is.
 */
int GUI_maze::pixel_width () const
{
   return pixel_wdth;
}

/**
 * Saves the maze as it currently is as an image file.
 */
void GUI_maze::save_to_file () const
{
   /***************** Write maze to a PPM format image file *****************/
}

/**
 * Sets the screen dimensions based on the ratio of rows to columns.
 */
void GUI_maze::set_screen_dimensions ()
{
   if (rooms_across > rooms_down)
   {
      GLfloat ratio = rooms_down / static_cast<GLfloat>(rooms_across);
      pixel_ht = static_cast<int>(pixel_wdth * ratio);
   }
   else
   {
      GLfloat ratio = rooms_across / static_cast<GLfloat>(rooms_down);
      pixel_wdth = static_cast<int>(pixel_ht * ratio);
   }
}

/**
 * Places the vertices to represent the maze.
 */
void GUI_maze::place_vertices ()
{
   int rows = rooms_down   * 2 + 1;
   int cols = rooms_across * 2 + 1;
   num_points = cols * rows * VERTICES_PER_CELL;
   
   // The world coordinates of the screen has x,y values in the 
   //    range [-1.0, 1.0].
   GLfloat wc_width  = 2.0f / cols;
   GLfloat wc_height = 2.0f / rows;

   points = new vec3 [num_points];

   for (int i = 0; i < rows; ++i)
   {
      for (int j = 0; j < cols; ++j)
      {
         GLfloat x = -1.0f + wc_width  * j;
         GLfloat y =  1.0f - wc_height * i;
         int index = (i * cols + j) * VERTICES_PER_CELL;
         
         // 6 vertices (2 triangles) define each cell
         points[index++] = vec3 (x, y, 0.0f);
         points[index++] = vec3 (x, y - wc_height, 0.0f);
         points[index++] = vec3 (x + wc_width, y - wc_height, 0.0f);
         points[index++] = vec3 (x + wc_width, y - wc_height, 0.0f);
         points[index++] = vec3 (x + wc_width, y, 0.0f);
         points[index]   = vec3 (x, y, 0.0f);      
      }
   }
}

/**
 * Stores the vertices in GPU memory. 
 */
void GUI_maze::load_GPU_arrays ()
{
   // Create a vertex array object
   GLuint vao;
   glGenVertexArrays (1, &vao);
   glBindVertexArray (vao);

   // Create and initialize a buffer object
   GLuint buffer;
   glGenBuffers (1, &buffer);
   glBindBuffer (GL_ARRAY_BUFFER, buffer);

   // Create an empty buffer of the size we need by passing
   //   a NULL pointer for the data values
   glBufferData (GL_ARRAY_BUFFER, sizeof(vec3) * num_points + 
                 sizeof(color3) * num_points, NULL, GL_STATIC_DRAW);

   // Load vertices into GPU memory
   glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(vec3) * num_points, points);

   // Load colors into GPU memory
   glBufferSubData (GL_ARRAY_BUFFER, sizeof(vec3) * num_points, 
                   sizeof(color3) * num_points, get_GPU_colors());
}

/**
 * Loads the shaders that help draw the scene.
 */
void GUI_maze::setup_shaders_and_vars ()
{
   // Load shaders and use the resulting shader program
   GLuint program = InitShader ("vshader.glsl", "fshader.glsl");
   glUseProgram (program);

   // Initialize the vertex position attribute from the vertex shader    
   GLuint vPosition = glGetAttribLocation (program, "vPosition");
   glEnableVertexAttribArray (vPosition);
   glVertexAttribPointer (vPosition, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));

   // Initialize the color position attribute
   GLuint vColor = glGetAttribLocation (program, "vColor"); 
   glEnableVertexAttribArray (vColor);
   glVertexAttribPointer (vColor, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(vec3) * num_points));
}

