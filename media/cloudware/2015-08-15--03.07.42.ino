
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
  cube.setVoxel(time % 8, time % 8, time % 8, red);
  cube.show();
  if(frame % 10 == 0)
    time++;
  frame++;
}