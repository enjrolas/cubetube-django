
#include "beta-cube-library.h"

Cube cube = Cube();
int frame=0;

void setup()
{
  cube.begin();
  cube.background(blue);
}

void loop()
{
  cube.background(black);
  if(frame%20>10) {
	  cube.setVoxel(3,3,3,red);
  }
  switch (frame%8) {
      1 : 
      	cube.setVoxel(3,3,3,blue);
        break;
      	
  }
  
  cube.show();
  frame++;
}