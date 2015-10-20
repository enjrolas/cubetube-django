
#include "beta-cube-library.h"

char* vizName="Test";
int vizId=579;
L3D cube;
PVector voxel=new PVector(3,3,3);  //this is the voxel that we'll blink

void setup()
{
  size(displayWidth, displayHeight, P3D);
  cube=new L3D(this);
//  cube=new L3D(this, "your@spark.email", "your password", "cube name");
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void draw()
{
  background(0);
  cube.background(0);
  if ((frameCount%20)>10)    //turn the LED on for ten frames, then off for ten frames
    cube.setVoxel(voxel, color(255, 0, 0));
}
