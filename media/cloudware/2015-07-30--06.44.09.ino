
#include "beta-cube-library.h"

Cube cube = Cube(); //sets the cube: this intializes a default 8x8x8 Cube

void setup() { 
  cube.begin();  //tells the Cube to start
}

void loop() {
 	cube.background(black);  //clears the Cube
	cube.setVoxel(4, 4, 4,99, 99, 99);   
  	 cube.show(); //the cube won't show any new information without this line -- always necessary
}