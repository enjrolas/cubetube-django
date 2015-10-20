
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
  cube.background(blue);
  if(frame%20>10)
	  cube.setVoxel(3,3,3,black);
  cube.show();
  frame++;
}