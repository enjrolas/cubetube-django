
#include "beta-cube-library.h"

Cube cube = Cube();
int frame=0;
int random=0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.background(black);
  if(frame%20>10) {
      random = cube.rand();
	  cube.setVoxel(0,3,3,white);
	  cube.setVoxel(1,3,3,white);
	  cube.setVoxel(2,3,3,white);
	  cube.setVoxel(3,3,3,white);
	  cube.setVoxel(4,3,3,white);
	  cube.setVoxel(5,3,3,white);
	  cube.setVoxel(6,3,3,white);
	  cube.setVoxel(7,3,3,white);
	  cube.setVoxel(3,0,3,blue);
	  cube.setVoxel(4,1,3,blue);
	  cube.setVoxel(3,2,3,blue);
	  cube.setVoxel(4,3,3,blue);
	  cube.setVoxel(3,4,3,blue);
	  cube.setVoxel(4,5,3,blue);
	  cube.setVoxel(3,6,3,blue);
	  cube.setVoxel(4,7,3,blue);
  }
  cube.show();
  frame++;
}