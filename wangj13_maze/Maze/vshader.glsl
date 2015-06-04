/**
 * Created by S. Gothard 07/18/2012 
 * For Fall 2012 CPS 350 Data Structures and Algorithms project 3
 * Very basic vertex shader. For each vertex in the scene, it accepts the 
 *    passed-in, 4-float vertex position and 4-float color and sets up the
 *    scene appropriately. Position is set by this shader, and color by the
 *    fragment shader, so the color must be passed to the fragment shader.
 */

#version 130

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

void main() 
{
  gl_Position = vPosition;
  color = vColor;
} 
