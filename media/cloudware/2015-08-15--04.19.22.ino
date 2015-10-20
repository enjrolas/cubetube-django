
#include "beta-cube-library.h"

Cube cube = Cube();
int frame = 0;
int c = 0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.background(black);

  cube.setVoxel(0, 0, c%8, blue);
  cube.setVoxel(0, 7, c%8, green);
  cube.setVoxel(7, 0, c%8, yellow);
  cube.setVoxel(7, 7, c%8, red);
  
  cube.show();
  
  frame++;
  if(frame % 10 == 0)
    c++;
}