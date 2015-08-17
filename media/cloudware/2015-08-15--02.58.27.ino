
#include "beta-cube-library.h"

Cube cube = Cube();
int frame = 0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.background(black);
  int n = frame % 8;
  cube.setVoxel(n, n, n, red);
  cube.show();
  frame++;
}