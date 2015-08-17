
#include "beta-cube-library.h"

char* vizName="        Blink green, yellow, red";
int vizId=151;
Cube cube = Cube();
int frame=0;

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
  if(frame%20>10)
	  cube.setVoxel(4,5,3,green);
  if(frame%20>10)
  	  cube.setVoxel(6,5,6,yellow);
  if(frame%20>10)
    cube.setVoxel(0,1,1,red);
  cube.show();
  frame++;
}