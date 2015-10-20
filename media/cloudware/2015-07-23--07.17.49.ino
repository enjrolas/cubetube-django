
#include "beta-cube-library.h"

Cube cube = cube();
PVector voxel=new PVector(3,3,3);  //this is the voxel that we'll blink

void setup()
{
  cube.begin();
}

void draw()
{
  cube.background(0);
    cube.setVoxel(voxel, color(255, 0, 0));
}