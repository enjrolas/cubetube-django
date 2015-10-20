
#include "beta-cube-library.h"

Cube cube = Cube();
int frame=0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.background(black);
  if(frame%20>10) {
	  cube.setVoxel(0,3,3,blue);
	  cube.setVoxel(1,3,3,blue);
	  cube.setVoxel(2,3,3,blue);
	  cube.setVoxel(3,3,3,blue);
	  cube.setVoxel(4,3,3,blue);
	  cube.setVoxel(5,3,3,blue);
	  cube.setVoxel(6,3,3,blue);
	  cube.setVoxel(7,3,3,blue);
  }
  cube.show();
  frame++;
}