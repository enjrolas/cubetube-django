
#include "beta-cube-library.h"

Cube cube = Cube();
int frame = 0;
int time = 0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.background(black);
  cube.setVoxel(3, 3, 3, blue);
  cube.show();
  frame++;
}