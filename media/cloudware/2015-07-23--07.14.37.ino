
#include "beta-cube-library.h"

L3D cube;
PVector voxel=new PVector(3,3,3);  //this is the voxel that we'll blink

void setup()
{
  cube=new L3D(this);
}

void draw()
{
  cube.background(0);
    cube.setVoxel(voxel, color(255, 0, 0));
}