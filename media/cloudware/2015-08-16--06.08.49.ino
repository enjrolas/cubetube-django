
#include "beta-cube-library.h"

Cube cube = Cube();
int frame=0;
float x=0;
float y=0;
float z=0;
float c=0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.background(black);
  if(frame%20>10) {
      x = Rand(7);
      y = Rand(7);
      z = Rand(7);
      c = Rand(1);
      switch (c) {
        case : 0
          cube.setVoxel(x,y,z,black);
            break;
          case : 1
          	  cube.setVoxel(x,y,z,white);
          default : 
          	  cube.setVoxel(x,y,z,red);
          break;
      }
  }
  cube.show();
  frame++;
}