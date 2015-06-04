/**
 * Created by S. Gothard 07/06/2012 
 * For Fall 2012 CPS 350 Data Structures and Algorithms project 3
 * Handles the OpenGL GUI for the maze.
 */

#include "GUI_maze.h"

// One global variable is necessary because all the UI callbacks must be
//    stand-alone functions with no user-defined parameters.
//    The GUI_maze is dynamically allocated so that the constructor can be
//    more easily called later. 
GUI_maze *gui_maze;

/**
 * Provides user instructions and creates the GUI maze of the default or 
 *    specified dimensions.
 */
void create_gui_maze ()
{
   string input;
   int rooms_across, rooms_down;
   cout << "******************************************************************"
        << "********"  << endl << "                          Random Maze Gener"
        << "ator" << endl << "************************************************"
        << "**************************" << endl << "Controls:\n\tNavigate with"
        << " the arrow keys" << endl << "\tg Go: solve the maze" << endl 
        << "\tf Finish solving the maze instantly" << endl << "\td Demo mode: "
        << "solve and generate new mazes to repeatedly solve" << endl << "\ts "
        << "Step: connect one room to build the maze" << endl << "\tc Continue"
        << ": connect all rooms" << endl << "\tb Build the maze by connecting "
        << "all rooms instantly" << endl << "\tn New maze" << endl << "\tp "
        << "Print the maze as a PPM-format image in the current directory" 
        << endl;

   cout << "Do you want to set the maze dimensions? ";
   getline (cin, input);

   if (input.size() > 0 && 'Y' == toupper (input[0]))
   {
      cout << "Enter width: ";
      cin >> rooms_across;
      cout << "Enter height: ";
      cin >> rooms_down;
      gui_maze = new GUI_maze (rooms_down, rooms_across);
   }
   else gui_maze = new GUI_maze ();
}

/**
 * Performs maze animations, including maze generating, solving, and a 
 *    combination of both in demo mode.
 */
void idle ()
{
   static const int FINISH_PAUSE = 3000;
   static const int PAUSE = 5;
   static clock_t prev = clock();
   static bool win_already_noted = false;
   clock_t current = clock();

   // If the maze has been solved, wait at least 3 seconds before generating a 
   //    new maze.
   if (gui_maze->maze_solved())
   {
      if (!win_already_noted)
      {
         // Reset clock for a pause after the win.
         win_already_noted = true;
         prev = current;
         glutPostRedisplay();
      }
      else if (current - prev >= FINISH_PAUSE)
      {
         gui_maze->create_new_maze();
         glutPostRedisplay();
         prev = current;
         win_already_noted = false;
      }
   }

   // if the maze has not been solved but is animating, performs the 
   //    appropriate step: building or solving.
   else if (gui_maze->is_animating() && current - prev >= PAUSE)
   {
      gui_maze->do_next_step ();
      prev = current;
      glutPostRedisplay();
   }
}

/**
 * All rendering is performed in the display callback.
 */ 
void display () 
{
   // The text to print upon maze completion
   const char * const win_message = "You win!";
   
   // clear screen and depth buffer
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Update the colors of the vertices to show any changes to the maze.
   glBufferSubData (GL_ARRAY_BUFFER, sizeof(vec3) * gui_maze->pts_count(), 
                     sizeof(color3) * gui_maze->pts_count(), 
                     gui_maze->get_GPU_colors());

   // Draw the vertices as triangles using the specified colors.
   glDrawArrays(GL_TRIANGLES, 0, gui_maze->pts_count());

   // If the maze has been solved, show the winning message.
   if (gui_maze->maze_solved())
   {
      // Set the color to be yellow using deprecated OpenGL code.
      glColor3f (0.8f, 0.8f, 0.0);

      // Set the position of the text.
      glRasterPos3f (0.6f, -0.8f, 0);
      for (unsigned int i = 0; i < strlen(win_message); ++i)
      {
         // Write the message
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, win_message[i]);
      }
   }

   // Swap the buffer written to to the front (to be visible)
   glutSwapBuffers();
}

/**
 * Handles key strokes that are non-control keys.
 */
void keyboard (unsigned char key, int x, int y)
{
	cout << key <<endl;
   switch (key) 
   {
      case 033:
         exit (EXIT_SUCCESS);
         break;

      case 'p':
         gui_maze->save_to_file ();
         break;

      case 's':
         gui_maze->connect_one();
         glutPostRedisplay();
         break;

      case 'c':
         gui_maze->enter_build_mode ();
         break;

      case 'b':
         gui_maze->instant_build();
         glutPostRedisplay();
         break;

      case 'g':
         gui_maze->enter_solve_mode();
         break;

      case 'f':
         gui_maze->solve_maze();
         glutPostRedisplay();
         break;

      case 'd':
         gui_maze->enter_demo_mode();
         break;

      case 'n':
         gui_maze->create_new_maze();
         glutPostRedisplay();
         break;
   }
}

/**
 * Handles control keys, specifically arrow keys
 */
void special_keys (int key, int x, int y)
{
   if (GLUT_KEY_UP == key)         gui_maze->move_north();
   else if (GLUT_KEY_DOWN == key)  gui_maze->move_south();
   else if (GLUT_KEY_LEFT == key)  gui_maze->move_west();
   else if (GLUT_KEY_RIGHT == key) gui_maze->move_east();
   
   gui_maze->stop_animating ();
   glutPostRedisplay();
}

/**
 * The main that runs the program. Sets up the GUI maze handler, the GL Utility 
 *    Toolkit (GLUT), the GL Extension Wrangler (GLEW), OpenGL GPU vertex data, 
 *    shaders, and all the interaction callbacks. Finally, enters the 
 *    interactive loop.
 */
int main (int argc, char **argv)
{
   create_gui_maze();

   glutInit (&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (gui_maze->pixel_width(), gui_maze->pixel_height());
   glutInitContextVersion (3, 3);
   glutInitContextProfile (GLUT_CORE_PROFILE);
   glutCreateWindow ("GUI Maze");
    
   glewExperimental = TRUE;
   glewInit();
   gui_maze->setup_rendering();
    
   // set callbacks
   glutDisplayFunc (display);
   
   glutKeyboardFunc (keyboard);
   
   glutSpecialFunc (special_keys);
   
   glutIdleFunc (idle);

   glutMainLoop();

   // Deallocated allocated memory
   delete gui_maze;
   gui_maze = NULL;

   return EXIT_SUCCESS;
}
