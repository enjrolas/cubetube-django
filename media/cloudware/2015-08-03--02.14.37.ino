
#include "beta-cube-library.h"

Cube cube = Cube();

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  for ( int i = 0; i < 8; i ++ ) {
    cube.setVoxel(i,3,3,red);
    cube.show();
    delay(500);
    cube.background(black);
    cube.show();
    delay(500);
  }
}