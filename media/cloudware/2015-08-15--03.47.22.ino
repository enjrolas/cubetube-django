
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

  cube.setVoxel(c%8, c%8, c%8, blue);
  
  cube.show();
  
  frame++;
  if(frame % 10 == 0)
    c++;
}