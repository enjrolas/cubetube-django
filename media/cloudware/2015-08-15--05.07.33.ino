
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

  c = (- (frame - 27) * (frame - 27) + 729) / 100;

  cube.setVoxel(0, c%8, 0, blue);
  cube.setVoxel(0, (c+1)%8, 7, green);
  cube.setVoxel(7, (c+2)%8, 0, yellow);
  cube.setVoxel(7, (c+3)%8, 7, red);
  cube.setVoxel(2, c%8, 2, orange);
  cube.setVoxel(4, c%8, 4, purple);
  
  cube.show();
  
  frame++;
  if(frame > 54) frame = 0;
}