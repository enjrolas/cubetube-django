
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
  if ((frameCount%20)>10)    //turn the LED on for ten frames, then off for ten frames
    cube.setVoxel(voxel, color(255, 0, 0));
}