
#include "beta-cube-library.h"

 /*
 A Cube Classic!  3D Planes flying through each other and changing colors along their journey.
 
 created 30 June 2015
 by Shawn Frayne
 inspired by Dataway's original streaming "Wall Bounce"
 
 This example code is in the public domain.
 
 Designed to work with a 8x8x8 L3D Cube.  
 www.cubetube.org
 
 Note: I've commented up this code like crazy to help as a foundational program for new Cubers.
 
 Any additional clarifications or improvements, please post in the comments!

 */


Cube cube = Cube(); //sets the cube: this intializes a default 8x8x8 Cube
int inc=1;  //increment: this controls how many pixels the animation will jump at each step
int pos=0;  //position: tracks position and direction of the animation
int frame=0;  


void setup() { 
  cube.begin();  //tells the Cube to start

}


void loop() {
  	cube.background(black);  //clears the Cube
    cube.setVoxel(4, 4, 4, 4,0,200));  //a core function from the L3D library for setting the color of a particular voxel in the Cube: http://goo.gl/u1iVEF
	cube.show(); //the cube won't show any new information without this line -- always necessary

  }
