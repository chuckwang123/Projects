/**
 * Created by S. Gothard 07/18/2012 
 * For Fall 2012 CPS 350 Data Structures and Algorithms project 3
 * Very basic fragment shader. For each fragment (potential pixel), it accepts
 *    the passed-in, 4-float color variable provided by the vertex shader and
 *    sets the fragment's color to it.
 */

#version 130

in  vec4  color;
out vec4  fColor;

void main() 
{ 
    fColor = color;
} 

