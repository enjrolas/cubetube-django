
#include "beta-cube-library.h"

char* vizName="Axis Demo";
int vizId=134;
Cube cube = Cube();

void setup()
{
  cube.begin();
  cube.background(black);
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
  cube.background(black);
  
  cube.setVoxel(0,0,0,white);
  cube.setVoxel(3,0,0,red);
  cube.setVoxel(0,3,0,green);
  cube.setVoxel(0,0,3,blue);
  
  cube.show();
}